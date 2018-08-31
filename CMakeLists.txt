cmake_minimum_required(VERSION 3.8)
project(Swarm_Robotics_tracking_with_rectangle_detection)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
find_package( OpenCV REQUIRED )

set(SOURCE_FILES main.cpp FieldMapper.cpp FieldMapper.h Line.cpp Line.h MathCalc.cpp MathCalc.h Blob.cpp Blob.h Comperators.h BlobToFile.cpp BlobToFile.h)
add_executable(Swarm_Robotics_tracking_with_rectangle_detection ${SOURCE_FILES})

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(Swarm_Robotics_tracking_with_rectangle_detection ${OpenCV_LIBS})