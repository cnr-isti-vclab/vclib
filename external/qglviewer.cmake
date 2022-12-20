#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2022                                                    *
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
	if (QT_FOUND)
		if (QGLViewer_FOUND)
			message(STATUS "- QGLViewer - using system-provided library")

			add_library(vclib-external-qglviewer INTERFACE)

			target_include_directories(vclib-external-qglviewer INTERFACE ${QGLVIEWER_INCLUDE_DIR})
			target_link_libraries(vclib-external-qglviewer INTERFACE ${QGLVIEWER_LIBRARY})

			list(APPEND VCLIB_EXTERNAL_LIBRARIES vclib-external-qglviewer)
		else()
			message(STATUS "- QGLViewer - not found, skipping")
		endif()
	else()
		message(STATUS "- QGLViewer - ignored, missing Qt")
	endif()
endif()
