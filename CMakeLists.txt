#set(CMAKE_CXX_STANDARD 14)
#SET(CMAKE_CXX_STANDARD_REQUIRED ON)
####
# Set minimum version of CMake. Since command 'project' use
# VERSION sub-option we need at least 3.0.
# Note: If you use 2.6 or 2.4, God kills a kitten. Seriously.
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project( RMInternalGpSCV )
#####
# Enables link_directories() treat paths relative
# to the source dir.
if(POLICY CMP0015)
    cmake_policy(SET CMP0015 NEW)
endif(POLICY CMP0015)

#####
# Enables project() command manages VERSION variables.
if(POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
endif(POLICY CMP0048)


include_directories("${PROJECT_SOURCE_DIR}")
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/source SRC) 

find_package(OpenCV REQUIRED)
#find_package( CUDA QUIET REQUIRED)
#INCLUDE(FindCUDA)
#INCLUDE_DIRECTORIES(/usr/local/cuda/include/)
# set  (CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS}; -03 -gencode arch=compute_22,code=sm_22)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
add_executable( main  ${SRC})
target_link_libraries( main -lpthread -lrt  ${OpenCV_LIBS})
