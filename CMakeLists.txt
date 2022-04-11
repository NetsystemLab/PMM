cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(missdn LANGUAGES CXX)

add_subdirectory(brew_retrieve)

set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
set(SUBDIRS "data2.cpp" "data_AS.cpp" "data.cpp" "data_random_generate.cpp" 
    "inner_module_0.cpp" "inner_module_1.cpp" "inter_module_0.cpp" "inter_module_1.cpp")
# message(STATUS "source dir is ${missdn_SOURCE_DIR}/source")

foreach(CURRENT_FILE ${SUBDIRS})
    get_filename_component(file_name ${CURRENT_FILE} NAME_WE)
    # message(STATUS "file name is ${file_name}, current file is ${missdn_SOURCE_DIR}/source/${CURRENT_FILE}")
    add_executable(${file_name} ${missdn_SOURCE_DIR}/source/${CURRENT_FILE})
    target_compile_options(${file_name} PRIVATE "-std=c++11" "-O0")
endforeach()


