# Try to find libQGLViewer lib
#
# This module reads hints about search locations from the following
# environment or cmake variables:
#
# QGLVIEWER_ROOT
# QGLVIEWER_ROOT_DIR

find_path(QGLVIEWER_INCLUDE_DIR qglviewer.h
	/usr/include/QGLViewer
	/usr/local/include/QGLViewer
	/opt/local/include/QGLViewer
	/usr/local/include/QGLViewer
	/Library/Frameworks/QGLViewer.framework/Headers
	/sw/include/QGLViewer
	${QGLVIEWER_ROOT}
	${QGLVIEWER_ROOT_DIR}
	ENV QGLVIEWER_ROOT
	ENV QGLVIEWER_ROOT_DIR)

if (Qt6_FOUND)
	set(QT_SUFFIX -qt6)
else()
	set(QT_SUFFIX -qt5)
endif()

find_library(QGLVIEWER_LIBRARY_RELEASE
	NAMES qglviewer QGLViewer qglviewer${QT_SUFFIX} QGLViewer${QT_SUFFIX}
	PATHS
		/usr/lib
		/usr/local/lib
		/opt/local/lib
		/sw/lib
		${QGLVIEWER_ROOT}
		${QGLVIEWER_ROOT_DIR}
		ENV QGLVIEWER_ROOT
		ENV QGLVIEWER_ROOT_DIR
		ENV LD_LIBRARY_PATH
		ENV LIBRARY_PATH
	PATH_SUFFIXES QGLViewer QGLViewer/release)

find_library(QGLVIEWER_LIBRARY_DEBUG
	NAMES dqglviewer dQGLViewer dqglviewer${QT_SUFFIX} dQGLViewer${QT_SUFFIX} QGLViewerd2
	PATHS
		/usr/lib
		/usr/local/lib
		/opt/local/lib
		/sw/lib
		${QGLVIEWER_ROOT}
		${QGLVIEWER_ROOT_DIR}
		ENV QGLVIEWER_ROOT
		ENV QGLVIEWER_ROOT_DIR
		ENV LD_LIBRARY_PATH
		ENV LIBRARY_PATH
	PATH_SUFFIXES QGLViewer QGLViewer/debug
)

if(QGLVIEWER_LIBRARY_RELEASE)
	if(QGLVIEWER_LIBRARY_DEBUG)
		set(QGLVIEWER_LIBRARY optimized ${QGLVIEWER_LIBRARY_RELEASE} debug ${QGLVIEWER_LIBRARY_DEBUG})
	else()
		set(QGLVIEWER_LIBRARY ${QGLVIEWER_LIBRARY_RELEASE})
	endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(QGLViewer DEFAULT_MSG QGLVIEWER_INCLUDE_DIR QGLVIEWER_LIBRARY)
