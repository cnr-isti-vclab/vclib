find_package(OpenGL QUIET)

if (OpenGL_FOUND)
	message(STATUS "VCLIB using OpenGL")
	target_link_libraries(vclib INTERFACE OpenGL::GL)
else()
	message(FATAL_ERROR "OpenGL not found. Unset VCLIB_USES_OPENGL option or install it in your system.")
endif()
