cmake_minimum_required(VERSION 3.10)
project(MotionDetectionProject)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

add_executable(MotionDetectionProject main.cpp)
target_link_libraries(MotionDetectionProject ${OpenCV_LIBS})
