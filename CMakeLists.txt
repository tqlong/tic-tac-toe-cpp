cmake_minimum_required(VERSION 3.19)
project(tictactoe VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
if (APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmacosx-version-min=13.0")
endif()

find_package(SFML COMPONENTS system window graphics network audio CONFIG REQUIRED)
set(SFML_LIBRARIES sfml-system sfml-window sfml-graphics sfml-network sfml-audio)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE)
target_link_libraries(${PROJECT_NAME} PRIVATE ${SFML_LIBRARIES})
