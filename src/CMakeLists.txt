cmake_minimum_required (VERSION 2.8.10)

project(cpp_actors)

set(CMAKE_CXX_FLAGS "-pthread" ${CMAKE_CXX_FLAGS})

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(
	${PROJECT_NAME}
	main.cpp
)
