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
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_EXT_OPENGL2_DRAWABLE_MESH_H
#define VCL_EXT_OPENGL2_DRAWABLE_MESH_H

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <iostream>

#include <vclib/render/generic_drawable_mesh.h>
#include <vclib/render/mesh_render_buffers.h>

#include "draw_objects3.h"

namespace vcl::gl2 {

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
class DrawableMesh : public GenericDrawableMesh
{
    MeshRenderBuffers<MeshType> mrb;

    std::vector<uint> textID;

public:
    DrawableMesh() = default;

    DrawableMesh(const MeshType& m)
    {
        if constexpr (HasName<MeshType>) {
            name() = m.name();
        }
        updateBuffers(m);
        mrs.setDefaultSettingsFromCapability();
    }

    void updateBuffers(const MeshType& m)
    {
        if constexpr (HasName<MeshType>) {
            name() = m.name();
        }
        unbindTextures();
        mrb = MeshRenderBuffers<MeshType>(m);
        mrs.setRenderCapabilityFrom(m);
        bindTextures();
    }

    // DrawableObject implementation

    void init() { bindTextures(); }

    void draw() const
    {
        if (mrs.isVisible()) {
            if (mrs.isWireframeVisible()) {
                if (mrs.isPointCloudVisible()) {
                    glDisable(GL_LIGHTING);
                    glShadeModel(GL_FLAT);
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    glDepthRange(0.0, 1.0);
                    renderPass();
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
                if (mrs.isSurfaceVisible()) {
                    if (mrs.isSurfaceShadingFlat()) {
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
                    else if (mrs.isSurfaceShadingSmooth()) {
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
                if (mrs.isPointCloudVisible()) {
                    glDisable(GL_LIGHTING);
                    renderPass();
                }
                if (mrs.isSurfaceVisible()) {
                    if (mrs.isSurfaceShadingFlat()) {
                        glEnable(GL_LIGHTING);
                        glShadeModel(GL_FLAT);
                        renderPass();
                    }
                    else if (mrs.isSurfaceShadingSmooth()) {
                        glEnable(GL_LIGHTING);
                        glShadeModel(GL_SMOOTH);
                        renderPass();
                    }
                }
            }
            if (mrs.isBboxEnabled()) {
                drawBox3(mrb.bbMin(), mrb.bbMax(), vcl::Color(0, 0, 0));
            }
        }
    }

    vcl::Point3d center() const { return (mrb.bbMin() + mrb.bbMax()) / 2; }

    double radius() const { return (mrb.bbMax() - mrb.bbMin()).norm() / 2; }

    DrawableMesh* clone() const { return new DrawableMesh(*this); }

private:
    void renderPass() const
    {
        uint nv = mrb.vertexNumber();
        uint nt = mrb.triangleNumber();

        const float*    coords          = mrb.vertexBufferData();
        const uint32_t* triangles       = mrb.triangleBufferData();
        const float*    vertexNormals   = mrb.vertexNormalBufferData();
        const uint32_t* vertexColors    = mrb.vertexColorBufferData();
        const float*    triangleNormals = mrb.triangleNormalBufferData();
        const uint32_t* triangleColors  = mrb.triangleColorBufferData();
        const float*    vertTexCoords   = mrb.vertexTexCoordsBufferData();
        const float*    wedgTexCoords   = mrb.wedgeTexCoordsBufferData();

        if (mrs.isPointCloudVisible()) {
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_FLOAT, 0, coords);

            if (mrs.isPointCloudColorPerVertex()) {
                glEnableClientState(GL_COLOR_ARRAY);
                glColorPointer(4, GL_UNSIGNED_BYTE, 0, vertexColors);
            }
            else if (mrs.isPointCloudColorPerMesh()) {
                glColor4fv(mrb.meshColorBufferData());
            }
            else if (mrs.isPointCloudColorUserDefined()) {
                glColor4fv(mrs.pointCloudUserColorData());
            }

            glPointSize(mrs.pointWidth());

            glDrawArrays(GL_POINTS, 0, nv);

            glDisableClientState(GL_COLOR_ARRAY);
            glDisableClientState(GL_VERTEX_ARRAY);
        }

        if (mrs.isSurfaceVisible()) {
            // Old fashioned, verbose and slow rendering.
            if (mrs.isSurfaceColorPerFace()) {
                int n_tris = nt;
                for (int tid = 0; tid < n_tris; ++tid) {
                    int tid_ptr  = 3 * tid;
                    int vid0     = triangles[tid_ptr + 0];
                    int vid1     = triangles[tid_ptr + 1];
                    int vid2     = triangles[tid_ptr + 2];
                    int vid0_ptr = 3 * vid0;
                    int vid1_ptr = 3 * vid1;
                    int vid2_ptr = 3 * vid2;

                    if (mrs.isSurfaceShadingSmooth()) {
                        glBegin(GL_TRIANGLES);
                        glColor4ubv((GLubyte*)&(triangleColors[tid]));
                        glNormal3fv(&(vertexNormals[vid0_ptr]));
                        glVertex3fv(&(coords[vid0_ptr]));
                        glNormal3fv(&(vertexNormals[vid1_ptr]));
                        glVertex3fv(&(coords[vid1_ptr]));
                        glNormal3fv(&(vertexNormals[vid2_ptr]));
                        glVertex3fv(&(coords[vid2_ptr]));
                        glEnd();
                    }
                    else {
                        glBegin(GL_TRIANGLES);
                        glColor4ubv((GLubyte*)&(triangleColors[tid]));
                        glNormal3fv(&(triangleNormals[tid_ptr]));
                        glVertex3fv(&(coords[vid0_ptr]));
                        glNormal3fv(&(triangleNormals[tid_ptr]));
                        glVertex3fv(&(coords[vid1_ptr]));
                        glNormal3fv(&(triangleNormals[tid_ptr]));
                        glVertex3fv(&(coords[vid2_ptr]));
                        glEnd();
                    }
                }
            }
            else if (mrs.isSurfaceColorPerVertex()) {
                if (mrs.isSurfaceShadingSmooth()) {
                    glEnableClientState(GL_VERTEX_ARRAY);
                    glVertexPointer(3, GL_FLOAT, 0, coords);

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
                        glColor4ubv((GLubyte*)&(vertexColors[vid0]));
                        glNormal3fv(&(triangleNormals[tid_ptr]));
                        glVertex3fv(&(coords[vid0_ptr]));
                        glColor4ubv((GLubyte*)&(vertexColors[vid1]));
                        glNormal3fv(&(triangleNormals[tid_ptr]));
                        glVertex3fv(&(coords[vid1_ptr]));
                        glColor4ubv((GLubyte*)&(vertexColors[vid2]));
                        glNormal3fv(&(triangleNormals[tid_ptr]));
                        glVertex3fv(&(coords[vid2_ptr]));
                        glEnd();
                    }
                }
            }
            else if (
                mrs.isSurfaceColorPerMesh() || mrs.isSurfaceColorUserDefined())
            {
                if (mrs.isSurfaceShadingSmooth()) {
                    glEnableClientState(GL_VERTEX_ARRAY);
                    glVertexPointer(3, GL_FLOAT, 0, coords);

                    glEnableClientState(GL_NORMAL_ARRAY);
                    glNormalPointer(GL_FLOAT, 0, vertexNormals);

                    if (mrs.isSurfaceColorPerMesh()) {
                        glColor4fv(mrb.meshColorBufferData());
                    }
                    else {
                        glColor4fv(mrs.surfaceUserColorData());
                    }

                    glDrawElements(
                        GL_TRIANGLES, nt * 3, GL_UNSIGNED_INT, triangles);

                    glDisableClientState(GL_COLOR_ARRAY);
                    glDisableClientState(GL_NORMAL_ARRAY);
                    glDisableClientState(GL_VERTEX_ARRAY);
                }
                else {
                    if (mrs.isSurfaceColorPerMesh()) {
                        glColor4fv(mrb.meshColorBufferData());
                    }
                    else {
                        glColor4fv(mrs.surfaceUserColorData());
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
                        glVertex3fv(&(coords[vid0_ptr]));
                        glNormal3fv(&(triangleNormals[tid_ptr]));
                        glVertex3fv(&(coords[vid1_ptr]));
                        glNormal3fv(&(triangleNormals[tid_ptr]));
                        glVertex3fv(&(coords[vid2_ptr]));
                        glEnd();
                    }
                }
            }
            else if (mrs.isSurfaceColorPerVertexTexcoords()) {
                if (mrs.isSurfaceShadingSmooth()) {
                    short texture = textID[0];
                    glBindTexture(GL_TEXTURE_2D, texture);
                    glColor4f(1, 1, 1, 1);
                    glEnableClientState(GL_VERTEX_ARRAY);
                    glVertexPointer(3, GL_FLOAT, 0, coords);

                    glEnableClientState(GL_NORMAL_ARRAY);
                    glNormalPointer(GL_FLOAT, 0, vertexNormals);

                    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                    glTexCoordPointer(2, GL_FLOAT, 0, vertTexCoords);

                    glDrawElements(
                        GL_TRIANGLES, nt * 3, GL_UNSIGNED_INT, triangles);

                    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                    glDisableClientState(GL_NORMAL_ARRAY);
                    glDisableClientState(GL_VERTEX_ARRAY);
                    glBindTexture(GL_TEXTURE_2D, 0);
                }
                else {
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
                        short texture  = textID[0];
                        glBindTexture(GL_TEXTURE_2D, texture);
                        glBegin(GL_TRIANGLES);
                        glColor4f(1, 1, 1, 1);
                        glTexCoord2f(
                            vertTexCoords[vid0 * 2 + 0],
                            vertTexCoords[vid0 * 2 + 1]);
                        glNormal3fv(&(vertexNormals[vid0_ptr]));
                        glVertex3fv(&(coords[vid0_ptr]));
                        glTexCoord2f(
                            vertTexCoords[vid1 * 2 + 0],
                            vertTexCoords[vid1 * 2 + 1]);
                        glNormal3fv(&(vertexNormals[vid1_ptr]));
                        glVertex3fv(&(coords[vid1_ptr]));
                        glTexCoord2f(
                            vertTexCoords[vid2 * 2 + 0],
                            vertTexCoords[vid2 * 2 + 1]);
                        glNormal3fv(&(vertexNormals[vid2_ptr]));
                        glVertex3fv(&(coords[vid2_ptr]));
                        glEnd();
                        glBindTexture(GL_TEXTURE_2D, 0);
                    }
                }
            }
            else if (mrs.isSurfaceColorPerWedgeTexcoords()) {
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
                        textID[mrb.wedgeTextureIDsBufferData()[tid]];
                    glBindTexture(GL_TEXTURE_2D, texture);
                    glBegin(GL_TRIANGLES);
                    glColor4f(1, 1, 1, 1);
                    glTexCoord2f(
                        wedgTexCoords[tid * 6 + 0], wedgTexCoords[tid * 6 + 1]);
                    glNormal3fv(&(vertexNormals[vid0_ptr]));
                    glVertex3fv(&(coords[vid0_ptr]));
                    glTexCoord2f(
                        wedgTexCoords[tid * 6 + 2], wedgTexCoords[tid * 6 + 3]);
                    glNormal3fv(&(vertexNormals[vid1_ptr]));
                    glVertex3fv(&(coords[vid1_ptr]));
                    glTexCoord2f(
                        wedgTexCoords[tid * 6 + 4], wedgTexCoords[tid * 6 + 5]);
                    glNormal3fv(&(vertexNormals[vid2_ptr]));
                    glVertex3fv(&(coords[vid2_ptr]));
                    glEnd();
                    glBindTexture(GL_TEXTURE_2D, 0);
                }
            }
        }

        if (mrs.isWireframeVisible()) {
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_FLOAT, 0, coords);

            glLineWidth(mrs.wireframeWidth());

            if (mrs.isWireframeColorPerMesh()) {
                glColor4fv(mrb.meshColorBufferData());
            }
            else {
                glColor4fv(mrs.wireframeUserColorData());
            }

            glDrawElements(GL_TRIANGLES, nt * 3, GL_UNSIGNED_INT, triangles);

            glDisableClientState(GL_VERTEX_ARRAY);
        }
    }

    void bindTextures()
    {
        textID.resize(mrb.textureNumber());
        glEnable(GL_TEXTURE_2D);
        glGenTextures(mrb.textureNumber(), textID.data());

        for (uint i = 0; i < mrb.textureNumber(); i++) {
            glBindTexture(GL_TEXTURE_2D, textID[i]);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RGB,
                mrb.textureSize(i).x(),
                mrb.textureSize(i).y(),
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                mrb.textureBufferData(i));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
    }

    void unbindTextures()
    {
        if (textID.size() > 0) {
            glDeleteTextures(textID.size(), textID.data());
            textID.clear();
        }
    }
};

} // namespace vcl::gl2

#endif // VCL_EXT_OPENGL2_DRAWABLE_MESH_H
