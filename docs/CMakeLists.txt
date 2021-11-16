find_package(Doxygen REQUIRED)

# Find all the public headers
get_target_property(
	MGP_PUBLIC_HEADER_DIR
	mgp
	INTERFACE_INCLUDE_DIRECTORIES)
file(
	GLOB_RECURSE
	MGP_PUBLIC_HEADERS
	${MGP_PUBLIC_HEADER_DIR}/*.h)

#This will be the main output of our command
set(DOXYGEN_INPUT_DIR ${MGP_INCLUDE_DIR})
set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/_doxygen)
set(DOXYGEN_INDEX_FILE ${CMAKE_CURRENT_SOURCE_DIR}/html/index.html)

add_custom_command(
	OUTPUT ${DOXYGEN_INDEX_FILE}
	DEPENDS ${MGP_PUBLIC_HEADERS}
	COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	MAIN_DEPENDENCY Doxyfile
	COMMENT "Generating docs"
	VERBATIM)

add_custom_target(Doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})

find_package(Sphinx REQUIRED)

set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(SPHINX_BUILD ${CMAKE_CURRENT_SOURCE_DIR}/_build/html)
set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)

# Only regenerate Sphinx when:
# - Doxygen has rerun
# - Our doc files have been updated
# - The Sphinx config has been updated
add_custom_command(OUTPUT ${SPHINX_INDEX_FILE}
	COMMAND
	${SPHINX_EXECUTABLE} -b html
	# Tell Breathe where to find the Doxygen output
	-Dbreathe_projects.mgplib=${DOXYGEN_OUTPUT_DIR}/xml
	${SPHINX_SOURCE} ${SPHINX_BUILD}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	DEPENDS
	# Other docs files you want to track should go here (or in some variable)
	${CMAKE_CURRENT_SOURCE_DIR}/index.rst
	${DOXYGEN_INDEX_FILE}
	MAIN_DEPENDENCY ${SPHINX_SOURCE}/conf.py
	COMMENT "Generating documentation with Sphinx")

# Nice named target so we can run the job easily
add_custom_target(Sphinx ALL DEPENDS ${SPHINX_INDEX_FILE})
