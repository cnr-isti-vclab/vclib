#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2023                                                    *
#* Alessandro Muntoni                                                        *
#* VCLab - ISTI - Italian National Research Council                          *
#*                                                                           *
#* All rights reserved.                                                      *
#****************************************************************************/

# to install QGLViewer on Mac:
# - download libQGLViewer
# - be sure to have a Qt environment set up
# - cd libQGLViewer/QGLViewer
# - qmake LIB_DIR=/Library/Frameworks
# - make
# - sudo make install

find_package(QGLViewer QUIET)

if (VCLIB_ALLOW_SYSTEM_QGLVIEWER)
    if (VCLIB_ALLOW_SYSTEM_QT AND QT_FOUND AND OpenGL_FOUND)
        if (QGLViewer_FOUND)
            message(STATUS "- QGLViewer - using system-provided library")

            add_library(vclib-external-qglviewer INTERFACE)

            target_include_directories(vclib-external-qglviewer INTERFACE ${QGLVIEWER_INCLUDE_DIR})
            target_link_libraries(vclib-external-qglviewer INTERFACE ${QGLVIEWER_LIBRARY})
            target_compile_definitions(vclib-external-qglviewer INTERFACE VCLIB_WITH_QGLVIEWER)

            list(APPEND VCLIB_RENDER_EXTERNAL_LIBRARIES vclib-external-qglviewer)
        else()
            message(STATUS "- QGLViewer - not found, skipping")
        endif()
    else()
        # message indicating why we jumped QGLViewer
        set (MISSING_LIB)
        if (NOT OpenGL_FOUND)
            list(APPEND MISSING_LIB OpenGL)
        endif()
        if (NOT QT_FOUND OR NOT VCLIB_ALLOW_SYSTEM_QT)
            list(APPEND MISSING_LIB Qt)
        endif()

        message(STATUS "- QGLViewer - ignored: missing ${MISSING_LIB}")
    endif()
endif()
