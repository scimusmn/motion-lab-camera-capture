project(motion-lab-camera)
cmake_minimum_required(VERSION 3.2)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR};${CMAKE_MODULE_PATH})
find_package(VIS_64 REQUIRED)

add_executable(camera-capture main.cpp error.cpp toojpeg/toojpeg.cpp)
target_include_directories(camera-capture PUBLIC ${VIS_INCLUDE_DIR})
target_link_libraries(camera-capture PUBLIC ${VIS_GIGE_LIB})
