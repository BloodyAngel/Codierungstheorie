project(Praktika1)
cmake_minimum_required(VERSION 2.8)

include(functions.cmake)
include_directories(include)    # optinal: foreach

find_package(OpenCV REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)

set(directories_to_include
    include
)

set(sources_to_include
    src
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)

_add_lib(sfml SFML_LIBS)
_add_cxxFlag(" -std=c++1y ")

FIND_PACKAGE(OpenMP REQUIRED)
if(OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

install(DIRECTORY images DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) # copy directory "images" after calling "make install"


# add OS specific include folders (can be found in include/<OS>)
message(your\ system\ name\ is\ \"${CMAKE_SYSTEM_NAME}\"\ you\ may\ add\ OS\ specific\ code\ in\ <include/src>/${CMAKE_SYSTEM_NAME}\n\n)
set(    sources_to_include      ${sources_to_include}       src/${CMAKE_SYSTEM_NAME})
set(directories_to_include  ${directories_to_include}   include/${CMAKE_SYSTEM_NAME})

# find all .h and .hpp files
FOREACH(dir ${directories_to_include})
    file(GLOB HEADERS_FILES ${HEADERS_FILES} ${dir}/*.h ${dir}/*.hpp)
ENDFOREACH()

# find all .cpp files
FOREACH(dir ${sources_to_include})
    file(GLOB SRC_FILES ${SRC_FILES} ${dir}/*.cpp ${dir}/*.c)
ENDFOREACH()


add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES} ${HEADERS_FILES})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBS})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

