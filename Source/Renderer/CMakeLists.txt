cmake_minimum_required(VERSION 3.1)

project(Renderer)

################################################################################
# Source files
################################################################################
file(GLOB COMMON_FILES
	${PROJECT_SOURCE_DIR}/*.cpp
	${PROJECT_SOURCE_DIR}/*.h
	${PROJECT_SOURCE_DIR}/*.hpp
	)

file(GLOB WINDOWS_FILES
	${PROJECT_SOURCE_DIR}/Windows/*.cpp
	${PROJECT_SOURCE_DIR}/Windows/*.h
	${PROJECT_SOURCE_DIR}/Windows/*.hpp
	)

set_source_files_properties(${COMMON_SOURCE_FILES} PROPERTIES HEADER_FILE_ONLY FALSE)
set_source_files_properties(${WINDOWS_RESOURCE_FILES} PROPERTIES HEADER_FILE_ONLY FALSE)

set(ALL_FILES
    	${COMMON_FILES}
	${WINDOWS_FILES}
)

foreach(FILE ${ALL_FILES})
    # make rel path
	file(RELATIVE_PATH REL_PATH "${PROJECT_SOURCE_DIR}" "${FILE}")

    # get the base path
    get_filename_component(BASE_PATH "${REL_PATH}" PATH)

    # changes /'s to \\'s
    string(REPLACE "/" "\\" GROUP "${BASE_PATH}")

    source_group("${GROUP}" FILES "${FILE}")
endforeach()

################################################################################
# Include directory
################################################################################	
include_directories(
    ${PROJECT_SOURCE_DIR}
    ${MODULE_DIR}/Math
	)

################################################################################
# Excutable Output Setting
################################################################################
add_executable(${PROJECT_NAME} WIN32 ${ALL_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
	PREFIX ""
	OUTPUT_NAME ${PROJECT_NAME}
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Output/Binary
)
	
################################################################################
# Import Library
################################################################################
target_link_libraries(${PROJECT_NAME} Math)


