cmake_minimum_required (VERSION 2.8.10)

project(cpp_actors)

set(CMAKE_CXX_FLAGS "-pthread" ${CMAKE_CXX_FLAGS})

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(SOURCES 
	actors/ActorContext.cpp
)

add_library(${PROJECT_NAME}
	${SOURCES}
)

add_executable(${PROJECT_NAME}_test
	main.cpp
	${SOURCES}
)
