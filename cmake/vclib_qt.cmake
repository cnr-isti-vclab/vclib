find_package(Qt5 COMPONENTS Core Xml OpenGL Gui Widgets QUIET)
find_package(Qt6 COMPONENTS Core Xml OpenGL Gui Widgets OpenGLWidgets QUIET)

list(APPEND HEADERS
	include/vclib/ext/opengl2/draw_objects2.h
	include/vclib/ext/opengl2/draw_objects3.h
	include/vclib/ext/opengl2/drawable_mesh.h)

list(APPEND SOURCES
	include/vclib/ext/opengl2/draw_objects2.cpp
	include/vclib/ext/opengl2/draw_objects3.cpp
	include/vclib/ext/opengl2/drawable_mesh.cpp)

if (NOT Qt5_FOUND AND NOT Qt6_FOUND)
	message(FATAL_ERROR "Qt not found. Unset VCLIB_USES_QT option or install it in your system.")
endif()

if (Qt6_FOUND)
	message(STATUS "VCLIB using Qt6")
	target_link_libraries(vclib INTERFACE
		Qt6::Core
		Qt6::OpenGLWidgets
		Qt6::Widgets
		Qt6::Xml)

	if (OpenGL_FOUND)
		target_link_libraries(vclib INTERFACE Qt6::OpenGL)

		if(VCLIB_USES_LIBQGLVIEWER)
			include(cmake/vclib_qglviewer.cmake)
		endif()
	endif()
endif()

if (Qt5_FOUND AND NOT Qt6_FOUND) # prefer Qt6
	message(STATUS "VCLIB using Qt5")
	target_link_libraries(vclib INTERFACE
		Qt5::Core
		Qt5::Widgets
		Qt5::Xml)

	if (OpenGL_FOUND)
		target_link_libraries(vclib INTERFACE Qt5::OpenGL)

		if(VCLIB_USES_LIBQGLVIEWER)
			include(cmake/vclib_qglviewer.cmake)
		endif()
	endif()
endif()
