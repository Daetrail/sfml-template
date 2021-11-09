cmake_minimum_required(VERSION 3.10.2)

project(Template)

# Directories
set(SOURCES_DIR      "src")
set(INCLUDE_DIR      "include")
set(SFML_INCLUDE_DIR "${INCLUDE_DIR}/SFML")
set(SFML_LIB_DIR     "lib")
set(SFML_CONFIG      "${SFML_LIB_DIR}/cmake/SFML")

set(CMAKE_PREFIX_PATH ${SFML_LIB_DIR})

# Source Files
file(GLOB SOURCE_FILES ${SOURCES_DIR}/*.cc)

# SFML Libraries
find_package(SFML 2.5.1 COMPONENTS graphics window audio system network REQUIRED PATHS ${SFML_CONFIG} ${SFML_LIB_DIR})

include_directories(${INCLUDE_DIR} ${SFML_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-system sfml-network sfml-window sfml-window)
