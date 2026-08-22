cmake_minimum_required(VERSION 4.3)
project(pdi_app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
find_package(OpenCV REQUIRED)
add_executable(pdi_app src/main.cpp
)
target_link_libraries(pdi_app PRIVATE ${OpenCV_LIBS})