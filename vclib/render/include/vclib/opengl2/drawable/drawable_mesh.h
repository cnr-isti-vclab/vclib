/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_OPENGL2_DRAWABLE_DRAWABLE_MESH_H
#define VCL_OPENGL2_DRAWABLE_DRAWABLE_MESH_H

#include "mesh/mesh_render_vectors.h"

#include <vclib/algorithms/mesh/stat/bounding_box.h>
#include <vclib/render/drawable/abstract_drawable_mesh.h>

#include <vclib/opengl2/drawable/draw_objects3.h>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <iostream>

namespace vcl {

// From:
// https://blog.nobel-joergensen.com/2013/01/29/debugging-opengl-using-glgeterror/
inline void _check_gl_error(const char* file, int line)
{
    GLenum err(glGetError());

    while (err != GL_NO_ERROR) {
        std::string error;

        switch (err) {
        case GL_INVALID_OPERATION: error = "INVALID_OPERATION"; break;
        case GL_INVALID_ENUM: error = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE: error = "INVALID_VALUE"; break;
        case GL_OUT_OF_MEMORY: error = "OUT_OF_MEMORY"; break;
#ifdef unix
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "INVALID_FRAMEBUFFER_OPERATION";
            break;
#endif
        }

        std::cerr << "GL_" << error.c_str() << " - " << file << ":" << line
                  << std::endl;
        err = glGetError();
    }
}

///
/// Usage
/// [... some opengl calls]
/// glCheckError();
///
#define check_gl_error() _check_gl_error(__FILE__, __LINE__)

template<MeshConcept MeshType>
class DrawableMeshOpenGL2 : public AbstractDrawableMesh, public MeshType
{
    using MRI = MeshRenderInfo;

    Box3d mBoundingBox;

    MeshRenderVectors<MeshType> mMRD;

    std::vector<uint> mTextID;

public:
    DrawableMeshOpenGL2() = default;

    DrawableMeshOpenGL2(const MeshType& mesh) :
            AbstractDrawableMesh(mesh), MeshType(mesh)
    {
        updateBuffers();
        mMRS.setDefaultSettingsFromCapability();
    }

    DrawableMeshOpenGL2(MeshType&& mesh) :
            AbstractDrawableMesh(mesh), MeshType(std::move(mesh))
    {
        updateBuffers();
        mMRS.setDefaultSettingsFromCapability();
    }

    ~DrawableMeshOpenGL2() = default;

    void swap(DrawableMeshOpenGL2& other)
    {
        using std::swap;
        AbstractDrawableMesh::swap(other);
        MeshType::swap(other);
        swap(mBoundingBox, other.mBoundingBox);
        swap(mMRD, other.mMRD);
        swap(mTextID, other.mTextID);
    }

    friend void swap(DrawableMeshOpenGL2& a, DrawableMeshOpenGL2& b)
    {
        a.swap(b);
    }

    // AbstractDrawableMesh implementation

    void updateBuffers(
        MRI::BuffersBitSet buffersToUpdate = MRI::BUFFERS_ALL) override
    {
        if constexpr (HasName<MeshType>) {
            AbstractDrawableMesh::name() = MeshType::name();
        }

        bool bbToInitialize = !vcl::HasBoundingBox<MeshType>;
        if constexpr (vcl::HasBoundingBox<MeshType>) {
            if (this->MeshType::boundingBox().isNull()) {
                bbToInitialize = true;
            }
            else {
                mBoundingBox =
                    this->MeshType::boundingBox().template cast<double>();
            }
        }

        if (bbToInitialize) {
            mBoundingBox = vcl::boundingBox(*this);
        }

        unbindTextures();
        mMRD.update(*this, buffersToUpdate);
        mMRS.setRenderCapabilityFrom(*this);
        bindTextures();
    }

    uint vertexNumber() const override { return MeshType::vertexNumber(); }

    uint faceNumber() const override
    {
        if constexpr (HasFaces<MeshType>)
            return MeshType::faceNumber();
        else
            return 0;
    }

    uint edgeNumber() const override
    {
        if constexpr (HasEdges<MeshType>)
            return MeshType::edgeNumber();
        else
            return 0;
    }

    vcl::Matrix44d transformMatrix() const override
    {
        if constexpr (HasTransformMatrix<MeshType>) {
            return MeshType::transformMatrix().template cast<double>();
        }
        else {
            return vcl::Matrix44d::Identity();
        }
    }

    std::vector<std::string> textures() const override
    {
        std::vector<std::string> txs;
        if constexpr (HasTexturePaths<MeshType>) {
            txs.reserve(MeshType::textureNumber());
            for (const auto& tpath : MeshType::texturePaths()) {
                txs.push_back(tpath);
            }
        }
        return txs;
    }

    // DrawableObject implementation

    void init() override { bindTextures(); }

    void draw(uint) const override
    {
        if (mMRS.isVisible()) {
            if (mMRS.isWireframe(MRI::Wireframe::VISIBLE)) {
                if (mMRS.isPoints(MRI::Points::VISIBLE)) {
                    glDisable(GL_LIGHTING);
                    glShadeModel(GL_FLAT);
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    glDepthRange(0.0, 1.0);
                    renderPass();
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
                if (mMRS.isSurface(MRI::Surface::VISIBLE)) {
                    if (mMRS.isSurface(MRI::Surface::SHADING_FLAT)) {
                        glEnable(GL_LIGHTING);
                        glShadeModel(GL_FLAT);
                        glDepthRange(0.01, 1.0);
                        renderPass();

                        glDisable(GL_LIGHTING);
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                        glDepthRange(0.0, 1.0);
                        glDepthFunc(GL_LEQUAL);
                        renderPass();
                        glDepthFunc(GL_LESS);
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    }
                    else if (mMRS.isSurface(MRI::Surface::SHADING_SMOOTH)) {
                        glEnable(GL_LIGHTING);
                        glShadeModel(GL_SMOOTH);
                        glDepthRange(0.01, 1.0);
                        renderPass();

                        glDisable(GL_LIGHTING);
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                        glDepthRange(0.0, 1.0);
                        glDepthFunc(GL_LEQUAL);
                        renderPass();
                        glDepthFunc(GL_LESS);
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    }
                }
                else {
                    glDisable(GL_LIGHTING);
                    glShadeModel(GL_FLAT);
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    glDepthRange(0.0, 1.0);
                    renderPass();
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
            }
            else { // no wireframe
                if (mMRS.isPoints(MRI::Points::VISIBLE)) {
                    glDisable(GL_LIGHTING);
                    renderPass();
                }
                if (mMRS.isSurface(MRI::Surface::VISIBLE)) {
                    if (mMRS.isSurface(MRI::Surface::SHADING_FLAT)) {
                        glEnable(GL_LIGHTING);
                        glShadeModel(GL_FLAT);
                        renderPass();
                    }
                    else if (mMRS.isSurface(MRI::Surface::SHADING_SMOOTH)) {
                        glEnable(GL_LIGHTING);
                        glShadeModel(GL_SMOOTH);
                        renderPass();
                    }
                }
            }
        }
    }

    Box3d boundingBox() const override { return mBoundingBox; }

    std::shared_ptr<DrawableObject> clone() const& override
    {
        return std::make_shared<DrawableMeshOpenGL2>(*this);
    }

    std::shared_ptr<DrawableObject> clone() && override
    {
        return std::make_shared<DrawableMeshOpenGL2>(std::move(*this));
    }

    std::string& name() override { return MeshType::name(); }

    const std::string& name() const override { return MeshType::name(); }

private:
    void renderPass() const
    {
        uint nv = mMRD.vertexNumber();
        uint nt = mMRD.triangleNumber();

        const float*    positions       = mMRD.vertexBufferData();
        const uint32_t* triangles       = mMRD.triangleBufferData();
        const float*    vertexNormals   = mMRD.vertexNormalBufferData();
        const uint32_t* vertexColors    = mMRD.vertexColorBufferData();
        const float*    triangleNormals = mMRD.triangleNormalBufferData();
        const uint32_t* triangleColors  = mMRD.triangleColorBufferData();
        const float*    vertTexCoords   = mMRD.vertexTexCoordsBufferData();
        const float*    wedgTexCoords   = mMRD.wedgeTexCoordsBufferData();

        if (mMRS.isPoints(MRI::Points::VISIBLE)) {
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_FLOAT, 0, positions);

            if (mMRS.isPoints(MRI::Points::COLOR_VERTEX)) {
                glEnableClientState(GL_COLOR_ARRAY);
                glColorPointer(4, GL_UNSIGNED_BYTE, 0, vertexColors);
            }
            else if (mMRS.isPoints(MRI::Points::COLOR_MESH)) {
                glColor4fv(mMRD.meshColorBufferData());
            }
            else if (mMRS.isPoints(MRI::Points::COLOR_USER)) {
                glColor4fv(mMRS.pointUserColorData());
            }

            glPointSize(mMRS.pointWidth());

            glDrawArrays(GL_POINTS, 0, nv);

            glDisableClientState(GL_COLOR_ARRAY);
            glDisableClientState(GL_VERTEX_ARRAY);
        }

        if (mMRS.isSurface(MRI::Surface::VISIBLE)) {
            // Old fashioned, verbose and slow rendering.
            if (mMRS.isSurface(MRI::Surface::COLOR_FACE)) {
                int n_tris = nt;
                for (int tid = 0; tid < n_tris; ++tid) {
                    int tid_ptr  = 3 * tid;
                    int vid0     = triangles[tid_ptr + 0];
                    int vid1     = triangles[tid_ptr + 1];
                    int vid2     = triangles[tid_ptr + 2];
                    int vid0_ptr = 3 * vid0;
                    int vid1_ptr = 3 * vid1;
                    int vid2_ptr = 3 * vid2;

                    if (mMRS.isSurface(MRI::Surface::SHADING_SMOOTH)) {
                        glBegin(GL_TRIANGLES);
                        glColor4ubv((GLubyte*) &(triangleColors[tid]));
                        glNormal3fv(&(vertexNormals[vid0_ptr]));
                        glVertex3fv(&(positions[vid0_ptr]));
                        glNormal3fv(&(vertexNormals[vid1_ptr]));
                        glVertex3fv(&(positions[vid1_ptr]));
                        glNormal3fv(&(vertexNormals[vid2_ptr]));
                        glVertex3fv(&(positions[vid2_ptr]));
                        glEnd();
                    }
                    else {
                        glBegin(GL_TRIANGLES);
                        glColor4ubv((GLubyte*) &(triangleColors[tid]));
                        glNormal3fv(&(triangleNormals[tid_ptr]));
                        glVertex3fv(&(positions[vid0_ptr]));
                        glNormal3fv(&(triangleNormals[tid_ptr]));
                        glVertex3fv(&(positions[vid1_ptr]));
                        glNormal3fv(&(triangleNormals[tid_ptr]));
                        glVertex3fv(&(positions[vid2_ptr]));
                        glEnd();
                    }
                }
            }
            else if (mMRS.isSurface(MRI::Surface::COLOR_VERTEX)) {
                if (mMRS.isSurface(MRI::Surface::SHADING_SMOOTH)) {
                    glEnableClientState(GL_VERTEX_ARRAY);
                    glVertexPointer(3, GL_FLOAT, 0, positions);

                    glEnableClientState(GL_NORMAL_ARRAY);
                    glNormalPointer(GL_FLOAT, 0, vertexNormals);

                    glEnableClientState(GL_COLOR_ARRAY);
                    glColorPointer(4, GL_UNSIGNED_BYTE, 0, vertexColors);

                    glDrawElements(
                        GL_TRIANGLES, nt * 3, GL_UNSIGNED_INT, triangles);

                    glDisableClientState(GL_COLOR_ARRAY);
                    glDisableClientState(GL_NORMAL_ARRAY);
                    glDisableClientState(GL_VERTEX_ARRAY);
                }
                else {
                    glShadeModel(GL_SMOOTH);
                    int n_tris = nt;
                    for (int tid = 0; tid < n_tris; ++tid) {
                        int tid_ptr  = 3 * tid;
                        int vid0     = triangles[tid_ptr + 0];
                        int vid1     = triangles[tid_ptr + 1];
                        int vid2     = triangles[tid_ptr + 2];
                        int vid0_ptr = 3 * vid0;
                        int vid1_ptr = 3 * vid1;
                        int vid2_ptr = 3 * vid2;

                        glBegin(GL_TRIANGLES);
                        glColor4ubv((GLubyte*) &(vertexColors[vid0]));
                        glNormal3fv(&(triangleNormals[tid_ptr]));
                        glVertex3fv(&(positions[vid0_ptr]));
                        glColor4ubv((GLubyte*) &(vertexColors[vid1]));
                        glNormal3fv(&(triangleNormals[tid_ptr]));
                        glVertex3fv(&(positions[vid1_ptr]));
                        glColor4ubv((GLubyte*) &(vertexColors[vid2]));
                        glNormal3fv(&(triangleNormals[tid_ptr]));
                        glVertex3fv(&(positions[vid2_ptr]));
                        glEnd();
                    }
                }
            }
            else if (
                mMRS.isSurface(MRI::Surface::COLOR_MESH) ||
                mMRS.isSurface(MRI::Surface::COLOR_USER)) {
                if (mMRS.isSurface(MRI::Surface::SHADING_SMOOTH)) {
                    glEnableClientState(GL_VERTEX_ARRAY);
                    glVertexPointer(3, GL_FLOAT, 0, positions);

                    glEnableClientState(GL_NORMAL_ARRAY);
                    glNormalPointer(GL_FLOAT, 0, vertexNormals);

                    if (mMRS.isSurface(MRI::Surface::COLOR_MESH)) {
                        glColor4fv(mMRD.meshColorBufferData());
                    }
                    else {
                        glColor4ubv((GLubyte*) mMRS.surfaceUserColorData());
                    }

                    glDrawElements(
                        GL_TRIANGLES, nt * 3, GL_UNSIGNED_INT, triangles);

                    glDisableClientState(GL_COLOR_ARRAY);
                    glDisableClientState(GL_NORMAL_ARRAY);
                    glDisableClientState(GL_VERTEX_ARRAY);
                }
                else {
                    if (mMRS.isSurface(MRI::Surface::COLOR_MESH)) {
                        glColor4fv(mMRD.meshColorBufferData());
                    }
                    else {
                        glColor4ubv((GLubyte*) mMRS.surfaceUserColorData());
                    }
                    int n_tris = nt;
                    for (int tid = 0; tid < n_tris; ++tid) {
                        int tid_ptr  = 3 * tid;
                        int vid0     = triangles[tid_ptr + 0];
                        int vid1     = triangles[tid_ptr + 1];
                        int vid2     = triangles[tid_ptr + 2];
                        int vid0_ptr = 3 * vid0;
                        int vid1_ptr = 3 * vid1;
                        int vid2_ptr = 3 * vid2;

                        glBegin(GL_TRIANGLES);
                        glNormal3fv(&(triangleNormals[tid_ptr]));
                        glVertex3fv(&(positions[vid0_ptr]));
                        glNormal3fv(&(triangleNormals[tid_ptr]));
                        glVertex3fv(&(positions[vid1_ptr]));
                        glNormal3fv(&(triangleNormals[tid_ptr]));
                        glVertex3fv(&(positions[vid2_ptr]));
                        glEnd();
                    }
                }
            }
            else if (mMRS.isSurface(MRI::Surface::COLOR_VERTEX_TEX)) {
                glShadeModel(GL_SMOOTH);
                int n_tris = nt;
                for (int tid = 0; tid < n_tris; ++tid) {
                    int   tid_ptr  = 3 * tid;
                    int   vid0     = triangles[tid_ptr + 0];
                    int   vid1     = triangles[tid_ptr + 1];
                    int   vid2     = triangles[tid_ptr + 2];
                    int   vid0_ptr = 3 * vid0;
                    int   vid1_ptr = 3 * vid1;
                    int   vid2_ptr = 3 * vid2;
                    short texture =
                        mTextID[mMRD.vertexTextureIDsBufferData()[tid]];
                    glBindTexture(GL_TEXTURE_2D, texture);
                    glBegin(GL_TRIANGLES);
                    glColor4f(1, 1, 1, 1);
                    glTexCoord2f(
                        vertTexCoords[vid0 * 2 + 0],
                        vertTexCoords[vid0 * 2 + 1]);
                    glNormal3fv(&(vertexNormals[vid0_ptr]));
                    glVertex3fv(&(positions[vid0_ptr]));
                    glTexCoord2f(
                        vertTexCoords[vid1 * 2 + 0],
                        vertTexCoords[vid1 * 2 + 1]);
                    glNormal3fv(&(vertexNormals[vid1_ptr]));
                    glVertex3fv(&(positions[vid1_ptr]));
                    glTexCoord2f(
                        vertTexCoords[vid2 * 2 + 0],
                        vertTexCoords[vid2 * 2 + 1]);
                    glNormal3fv(&(vertexNormals[vid2_ptr]));
                    glVertex3fv(&(positions[vid2_ptr]));
                    glEnd();
                    glBindTexture(GL_TEXTURE_2D, 0);
                }
            }
            else if (mMRS.isSurface(MRI::Surface::COLOR_WEDGE_TEX)) {
                int n_tris = nt;
                for (int tid = 0; tid < n_tris; ++tid) {
                    int   tid_ptr  = 3 * tid;
                    int   vid0     = triangles[tid_ptr + 0];
                    int   vid1     = triangles[tid_ptr + 1];
                    int   vid2     = triangles[tid_ptr + 2];
                    int   vid0_ptr = 3 * vid0;
                    int   vid1_ptr = 3 * vid1;
                    int   vid2_ptr = 3 * vid2;
                    short texture =
                        mTextID[mMRD.wedgeTextureIDsBufferData()[tid]];
                    glBindTexture(GL_TEXTURE_2D, texture);
                    glBegin(GL_TRIANGLES);
                    glColor4f(1, 1, 1, 1);
                    glTexCoord2f(
                        wedgTexCoords[vid0 * 2 + 0],
                        wedgTexCoords[vid0 * 2 + 1]);
                    glNormal3fv(&(vertexNormals[vid0_ptr]));
                    glVertex3fv(&(positions[vid0_ptr]));
                    glTexCoord2f(
                        wedgTexCoords[vid1 * 2 + 0],
                        wedgTexCoords[vid1 * 2 + 1]);
                    glNormal3fv(&(vertexNormals[vid1_ptr]));
                    glVertex3fv(&(positions[vid1_ptr]));
                    glTexCoord2f(
                        wedgTexCoords[vid2 * 2 + 0],
                        wedgTexCoords[vid2 * 2 + 1]);
                    glNormal3fv(&(vertexNormals[vid2_ptr]));
                    glVertex3fv(&(positions[vid2_ptr]));
                    glEnd();
                    glBindTexture(GL_TEXTURE_2D, 0);
                }
            }
        }

        if (mMRS.isWireframe(MRI::Wireframe::VISIBLE)) {
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_FLOAT, 0, positions);

            glLineWidth(mMRS.wireframeWidth());

            if (mMRS.isWireframe(MRI::Wireframe::COLOR_MESH)) {
                glColor4fv(mMRD.meshColorBufferData());
            }
            else {
                glColor4fv(mMRS.wireframeUserColorData());
            }

            glDrawElements(GL_TRIANGLES, nt * 3, GL_UNSIGNED_INT, triangles);

            glDisableClientState(GL_VERTEX_ARRAY);
        }
    }

    void bindTextures()
    {
        mTextID.resize(mMRD.textureNumber());
        glEnable(GL_TEXTURE_2D);
        glGenTextures(mMRD.textureNumber(), mTextID.data());

        for (uint i = 0; i < mMRD.textureNumber(); i++) {
            glBindTexture(GL_TEXTURE_2D, mTextID[i]);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RGBA,
                mMRD.textureSize(i).x(),
                mMRD.textureSize(i).y(),
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                mMRD.textureBufferData(i));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
    }

    void unbindTextures()
    {
        if (mTextID.size() > 0) {
            glDeleteTextures(mTextID.size(), mTextID.data());
            mTextID.clear();
        }
    }
};

} // namespace vcl

#endif // VCL_OPENGL2_DRAWABLE_DRAWABLE_MESH_H
