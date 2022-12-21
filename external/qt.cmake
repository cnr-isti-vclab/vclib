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

find_package(Qt5 COMPONENTS Core Xml OpenGL Gui Widgets QUIET)
find_package(Qt6 COMPONENTS Core Xml OpenGL Gui Widgets OpenGLWidgets QUIET)

if (Qt6_FOUND OR Qt5_FOUND)
	set(QT_FOUND TRUE)
	if(Qt6_FOUND)
		set(QT_VER 6)
	else()
		set(QT_VER 5)
	endif()
endif()

if (VCLIB_ALLOW_SYSTEM_QT)
	if (QT_FOUND)
		message(STATUS "- Qt${QT_VER} - using system-provided library")

		add_library(vclib-external-qt INTERFACE)
		target_compile_definitions(vclib-external-qt INTERFACE
			VCLIB_WITH_QT)

		# prefer Qt6
		if (Qt6_FOUND)
			target_link_libraries(vclib-external-qt INTERFACE
				Qt6::Core
				Qt6::OpenGLWidgets
				Qt6::Widgets
				Qt6::Xml)

			if (OpenGL_FOUND)
				target_link_libraries(vclib-external-qt INTERFACE Qt6::OpenGL)
			endif()
		endif()

		if (Qt5_FOUND AND NOT Qt6_FOUND)
			target_link_libraries(vclib-external-qt INTERFACE
				Qt5::Core
				Qt5::Widgets
				Qt5::Xml)

			if (OpenGL_FOUND)
				target_link_libraries(vclib-external-qt INTERFACE Qt5::OpenGL)
			endif()
		endif()

		list(APPEND VCLIB_EXTERNAL_LIBRARIES vclib-external-qt)
	else()
		message(STATUS "- Qt - not found, skipping")
	endif()
else()

endif()


