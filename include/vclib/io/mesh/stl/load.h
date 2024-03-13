/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_IO_STL_LOAD_H
#define VCL_IO_STL_LOAD_H

#include <vclib/io/read.h>
#include <vclib/mesh/utils/mesh_info.h>
#include <vclib/misc/logger.h>

namespace vcl {

namespace detail {

inline bool isBinStlMalformed(
    const std::string& filename,
    bool&              isBinary,
    std::size_t&       fsize)
{
    fsize    = FileInfo::fileSize(filename);
    isBinary = FileInfo::isFileBinary(filename);

    if (isBinary) {
        // we can check if the size of the file is the expected one
        std::ifstream fp = openInputFileStream(filename);
        fp.seekg(80); // size of the header
        uint        fnum = io::readUInt<uint>(fp);
        std::size_t expectedFileSize =
            80 + 4 +                     // header and number of faces
            fnum *                       // for each face
                (3 * sizeof(float) +     // 3 floats for the face normal
                 3 * 3 * sizeof(float) + // 3 floats for each vertex of the face
                 sizeof(unsigned short)); // a short containing attributes
        if (expectedFileSize != fsize) {
            // sometimes the size is a bit wrong
            std::size_t diff =
                std::abs((long int) expectedFileSize - (long int) fsize);
            if (diff > fsize / 20)
                return true;
        }
    }

    return false;
}

inline bool isStlColored(std::istream& fp, bool& magicsMode)
{
    bool colored = false;

    char buf[80];
    fp.read(buf, 80);
    std::string s(buf);
    size_t      cInd = s.rfind("COLOR=");
    size_t      mInd = s.rfind("MATERIAL=");
    if (cInd != std::string::npos && mInd != std::string::npos)
        magicsMode = true;
    else
        magicsMode = false;
    uint              fnum = io::readUInt<uint>(fp);
    static const uint fmax = 1000;
    // 3 floats for normal and 9 for vcoords
    static const uint fdataSize = 12 * sizeof(float);

    for (uint i = 0; i < std::min(fnum, fmax); ++i) {
        fp.seekg(fdataSize, std::ios::cur);
        unsigned short attr = io::readShort<unsigned short>(fp);
        Color          c;
        c.setBgr5(attr);
        if (c != Color::White)
            colored = true;
    }
    return colored;
}

template<MeshConcept MeshType, LoggerConcept LogType>
void readStlBin(
    MeshType&     m,
    std::istream& fp,
    MeshInfo&     loadedInfo,
    LogType&      log,
    bool          enableOptionalComponents)
{
    bool magicsMode, colored;
    colored = isStlColored(fp, magicsMode);

    if (enableOptionalComponents) {
        if (colored)
            loadedInfo.setFaceColors();
        enableOptionalComponentsFromInfo(loadedInfo, m);
    }
    else if (colored) {
        if constexpr (HasPerFaceColor<MeshType>) {
            if (isPerFaceColorAvailable(m))
                loadedInfo.setFaceColors();
        }
    }

    fp.seekg(80); // size of the header
    uint fnum = io::readUInt<uint>(fp);

    log.startProgress("Loading STL file", fnum);

    m.addVertices(fnum * 3);
    if constexpr (HasFaces<MeshType>) {
        m.reserveFaces(fnum);
    }

    uint vi = 0;
    for (uint i = 0; i < fnum; ++i) {
        Point3f norm;
        norm.x() = io::readFloat<float>(fp);
        norm.y() = io::readFloat<float>(fp);
        norm.z() = io::readFloat<float>(fp);

        for (uint j = 0; j < 3; ++j) {
            m.vertex(vi + j).coord().x() = io::readFloat<float>(fp);
            m.vertex(vi + j).coord().y() = io::readFloat<float>(fp);
            m.vertex(vi + j).coord().z() = io::readFloat<float>(fp);
        }

        unsigned short attr = io::readShort<unsigned short>(fp);

        if constexpr (HasFaces<MeshType>) {
            using FaceType = MeshType::FaceType;

            uint      fi = m.addFace();
            FaceType& f  = m.face(fi);
            for (uint j = 0; j < 3; ++j)
                f.vertex(j) = &m.vertex(vi + j);
            if (HasPerFaceNormal<MeshType>) {
                using ST = FaceType::NormalType::ScalarType;
                if (isPerFaceNormalAvailable(m)) {
                    f.normal() = norm.cast<ST>();
                }
            }
            if (HasPerFaceColor<MeshType>) {
                if (isPerFaceColorAvailable(m) && colored) {
                    Color c;
                    if (magicsMode)
                        c.setBgr5(attr);
                    else
                        c.setRgb5(attr);
                    f.color() = c;
                }
            }
        }

        vi += 3;

        log.progress(i);
    }
    log.endProgress();
}

template<MeshConcept MeshType, LoggerConcept LogType>
void readStlAscii(
    MeshType&     m,
    std::istream& fp,
    MeshInfo&     loadedInfo,
    LogType&      log,
    bool          enableOptionalComponents)
{
    if (enableOptionalComponents) {
        enableOptionalComponentsFromInfo(loadedInfo, m);
    }

    if constexpr (vcl::isLoggerValid<LogType>()) {
        fp.seekg(0, fp.end);
        std::size_t fsize = fp.tellg();
        fp.seekg(0, fp.beg);
        log.startProgress("Loading STL file", fsize);
    }

    vcl::Tokenizer tokens = readAndTokenizeNextNonEmptyLineNoThrow(fp);
    if (fp) {
        // cycle that reads a face starting from the actual tokenized line
        do {
            vcl::Tokenizer::iterator token = tokens.begin();
            if (token != tokens.end() && *token == "facet") {
                ++token; // skip the "facet" word
                ++token; // skip the "normal" word

                // add 3 vertices for the face
                uint vi = m.addVertices(3);

                // read the normal of the face
                Point3f normal;

                normal.x() = io::readFloat<float>(token);
                normal.y() = io::readFloat<float>(token);
                normal.z() = io::readFloat<float>(token);

                readAndTokenizeNextNonEmptyLine(fp); // outer loop
                // vertex x y z
                tokens = readAndTokenizeNextNonEmptyLine(fp);

                for (uint i = 0; i < 3; i++) { // read the three vertices
                    token = tokens.begin();
                    ++token; // skip the "vertex" word

                    m.vertex(vi + i).coord().x() = io::readFloat<float>(token);
                    m.vertex(vi + i).coord().y() = io::readFloat<float>(token);
                    m.vertex(vi + i).coord().z() = io::readFloat<float>(token);

                    // next vertex
                    tokens = readAndTokenizeNextNonEmptyLine(fp);
                }
                readAndTokenizeNextNonEmptyLine(fp); // endfacet

                if constexpr (HasFaces<MeshType>) {
                    using FaceType = MeshType::FaceType;
                    uint fi        = m.addFace();

                    FaceType& f = m.face(fi);
                    for (uint j = 0; j < 3; ++j)
                        f.vertex(j) = &m.vertex(vi + j);
                    if (HasPerFaceNormal<MeshType>) {
                        using ST = FaceType::NormalType::ScalarType;
                        if (isPerFaceNormalAvailable(m)) {
                            f.normal() = normal.cast<ST>();
                        }
                    }
                }
            }
            tokens = readAndTokenizeNextNonEmptyLineNoThrow(fp);

            log.progress(fp.tellg());
        } while (fp);
    }

    log.endProgress();
}

} // namespace detail

/**
 * @brief Loads from the given input stl stream and puts the content into the
 * mesh m.
 *
 * Since the STL format does not provide any information whether the file is
 * binary or ascii, the user must specify it. If the user specifies the wrong
 * format, the behaviour is undefined.
 *
 * The function will fill all the components read into the stream that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the stream, will be enabled before loading the stream.
 *
 * The info about what elements and components have been loaded from the stream
 * will be stored into the loadedInfo argument.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] m: the mesh to fill
 * @param[in] inputStlStream: the stream to read from
 * @param[out] loadedInfo: the info about what elements and components have been
 * loaded from the stream
 * @param[in] isBinary: if true, the stream is considered binary, otherwise it
 * is considered ascii
 * @param[in] log: the logger to use
 * @param[in] enableOptionalComponents: if true, some eventual optional
 * components of the mesh that were not enabled and that can be loaded from the
 * stream, will be enabled before loading the stream.
 *
 * @ingroup load
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadStl(
    MeshType&     m,
    std::istream& inputStlStream,
    MeshInfo&     loadedInfo,
    bool          isBinary                 = false,
    LogType&      log                      = nullLogger,
    bool          enableOptionalComponents = true)
{
    loadedInfo = MeshInfo();
    loadedInfo.setVertices();
    loadedInfo.setVertexCoords();

    if constexpr (HasFaces<MeshType>) {
        loadedInfo.setFaces();
        loadedInfo.setFaceVRefs();
        loadedInfo.setFaceNormals();
    }

    log.log(0, "Loading STL file");

    if (isBinary)
        detail::readStlBin(
            m, inputStlStream, loadedInfo, log, enableOptionalComponents);
    else
        detail::readStlAscii(
            m, inputStlStream, loadedInfo, log, enableOptionalComponents);

    log.log(100, "STL file loaded");
}

/**
 * @brief Loads from the given input stl stream and puts the content into the
 * mesh m.
 *
 * Since the STL format does not provide any information whether the file is
 * binary or ascii, the user must specify it. If the user specifies the wrong
 * format, the behaviour is undefined.
 *
 * The function will fill all the components read into the stream that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the stream, will be enabled before loading the stream.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] m: the mesh to fill
 * @param[in] inputStlStream: the stream to read from
 * @param[in] isBinary: if true, the stream is considered binary, otherwise it
 * is considered ascii
 * @param[in] log: the logger to use
 * @param[in] enableOptionalComponents: if true, some eventual optional
 * components of the mesh that were not enabled and that can be loaded from the
 * stream, will be enabled before loading the stream.
 *
 * @ingroup load
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadStl(
    MeshType&     m,
    std::istream& inputStlStream,
    bool          isBinary                 = false,
    LogType&      log                      = nullLogger,
    bool          enableOptionalComponents = true)
{
    MeshInfo loadedInfo;
    loadStl(
        m, inputStlStream, loadedInfo, isBinary, log, enableOptionalComponents);
}

/**
 * @brief Loads from the given input stl stream and puts the content into the
 * returned mesh m.
 *
 * Since the STL format does not provide any information whether the file is
 * binary or ascii, the user must specify it. If the user specifies the wrong
 * format, the behaviour is undefined.
 *
 * The function will fill all the components read into the stream that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the stream, will be enabled before loading the stream.
 *
 * The info about what elements and components have been loaded from the stream
 * will be stored into the loadedInfo argument.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] inputStlStream: the stream to read from
 * @param[out] loadedInfo: the info about what elements and components have been
 * loaded from the stream
 * @param[in] isBinary: if true, the stream is considered binary, otherwise it
 * is considered ascii
 * @param[in] log: the logger to use
 * @param[in] enableOptionalComponents: if true, some eventual optional
 * components of the mesh that were not enabled and that can be loaded from the
 * stream, will be enabled before loading the stream.
 * @returns the mesh loaded from the stream.
 *
 * @ingroup load
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType loadStl(
    std::istream& inputStlStream,
    MeshInfo&     loadedInfo,
    bool          isBinary                 = false,
    LogType&      log                      = nullLogger,
    bool          enableOptionalComponents = true)
{
    MeshType m;
    loadStl(
        m, inputStlStream, loadedInfo, isBinary, log, enableOptionalComponents);
    return m;
}

/**
 * @brief Loads from the given input stl stream and puts the content into the
 * returned mesh m.
 *
 * Since the STL format does not provide any information whether the file is
 * binary or ascii, the user must specify it. If the user specifies the wrong
 * format, the behaviour is undefined.
 *
 * The function will fill all the components read into the stream that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the stream, will be enabled before loading the stream.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] inputStlStream: the stream to read from
 * @param[in] isBinary: if true, the stream is considered binary, otherwise it
 * is considered ascii
 * @param[in] log: the logger to use
 * @param[in] enableOptionalComponents: if true, some eventual optional
 * components of the mesh that were not enabled and that can be loaded from the
 * stream, will be enabled before loading the stream.
 * @returns the mesh loaded from the stream.
 *
 * @ingroup load
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType loadStl(
    std::istream& inputStlStream,
    bool          isBinary                 = false,
    LogType&      log                      = nullLogger,
    bool          enableOptionalComponents = true)
{
    MeshInfo loadedInfo;
    return loadStl<MeshType>(
        inputStlStream, loadedInfo, isBinary, log, enableOptionalComponents);
}

/**
 * @brief Loads from the given input stl file and puts the content into the
 * mesh m.
 *
 * The function will fill all the components read into the file that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the file, will be enabled before loading the file.
 *
 * The info about what elements and components have been loaded from the file
 * will be stored into the loadedInfo argument.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] m: the mesh to fill
 * @param[in] filename: the name of the file to read from
 * @param[out] loadedInfo: the info about what elements and components have been
 * loaded from the stream
 * @param[in] log: the logger to use
 * @param[in] enableOptionalComponents: if true, some eventual optional
 * components of the mesh that were not enabled and that can be loaded from the
 * stream, will be enabled before loading the stream.
 *
 * @ingroup load
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadStl(
    MeshType&          m,
    const std::string& filename,
    MeshInfo&          loadedInfo,
    LogType&           log                      = nullLogger,
    bool               enableOptionalComponents = true)
{
    log.log(0, "Checking STL file");

    bool        isBinary;
    std::size_t filesize;
    if (detail::isBinStlMalformed(filename, isBinary, filesize))
        throw MalformedFileException(filename + " is malformed.");

    log.log(0, "Opening STL file");

    std::ifstream fp = openInputFileStream(filename);

    if constexpr (HasName<MeshType>) {
        m.name() = FileInfo::fileNameWithoutExtension(filename);
    }

    loadStl(m, fp, loadedInfo, isBinary, log, enableOptionalComponents);
}

/**
 * @brief Loads from the given input stl file and puts the content into the
 * mesh m.
 *
 * The function will fill all the components read into the file that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the file, will be enabled before loading the file.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] m: the mesh to fill
 * @param[in] filename: the name of the file to read from
 * @param[in] log: the logger to use
 * @param[in] enableOptionalComponents: if true, some eventual optional
 * components of the mesh that were not enabled and that can be loaded from the
 * stream, will be enabled before loading the stream.
 *
 * @ingroup load
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadStl(
    MeshType&          m,
    const std::string& filename,
    LogType&           log                      = nullLogger,
    bool               enableOptionalComponents = true)
{
    MeshInfo loadedInfo;
    loadStl(m, filename, loadedInfo, log, enableOptionalComponents);
}

/**
 * @brief Loads from the given input stl file and puts the content into the
 * returned mesh m.
 *
 * The function will fill all the components read into the file that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the file, will be enabled before loading the file.
 *
 * The info about what elements and components have been loaded from the file
 * will be stored into the loadedInfo argument.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] filename: the name of the file to read from
 * @param[out] loadedInfo: the info about what elements and components have been
 * loaded from the stream
 * @param[in] log: the logger to use
 * @param[in] enableOptionalComponents: if true, some eventual optional
 * components of the mesh that were not enabled and that can be loaded from the
 * stream, will be enabled before loading the stream.
 * @returns the mesh loaded from the file.
 *
 * @ingroup load
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType loadStl(
    const std::string& filename,
    MeshInfo&          loadedInfo,
    LogType&           log                      = nullLogger,
    bool               enableOptionalComponents = true)
{
    MeshType m;
    loadStl(m, filename, loadedInfo, log, enableOptionalComponents);
    return m;
}

/**
 * @brief Loads from the given input stl file and puts the content into the
 * returned mesh m.
 *
 * The function will fill all the components read into the file that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the file, will be enabled before loading the file.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] filename: the name of the file to read from
 * @param[in] log: the logger to use
 * @param[in] enableOptionalComponents: if true, some eventual optional
 * components of the mesh that were not enabled and that can be loaded from the
 * stream, will be enabled before loading the stream.
 * @returns the mesh loaded from the file.
 *
 * @ingroup load
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType loadStl(
    const std::string& filename,
    LogType&           log                      = nullLogger,
    bool               enableOptionalComponents = true)
{
    MeshInfo loadedInfo;
    return loadStl<MeshType>(
        filename, loadedInfo, log, enableOptionalComponents);
}

} // namespace vcl

#endif // VCL_IO_STL_LOAD_H
