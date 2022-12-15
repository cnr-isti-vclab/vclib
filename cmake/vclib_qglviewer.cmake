# to install QGLViewer on Mac:
# - download libQGLViewer
# - be sure to have a Qt environment set up
# - cd libQGLViewer/QGLViewer
# - qmake LIB_DIR=/Library/Frameworks
# - make
# - sudo make install

find_package(QGLViewer QUIET)

if (QGLViewer_FOUND)
	message(STATUS "VCLIB using libQGLViewer")

	target_include_directories(vclib INTERFACE ${QGLVIEWER_INCLUDE_DIR})
	target_link_libraries(vclib INTERFACE ${QGLVIEWER_LIBRARY})

	list(APPEND HEADERS
		include/vclib/ext/qglviewer/viewer.h)

	list(APPEND SOURCES
		include/vclib/ext/qglviewer/viewer.cpp)
else()
	message(FATAL_ERROR "libQGLViewer not found. Unset VCLIB_USES_LIBQGLVIEWER option or install it in your system.")
endif()
