project(cbtde)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")
find_package(OpenGL REQUIRED)
set(GL3W_LIBRARY "-lglfw")
SET(LIBRARIES ${GL3W_LIBRARY} ${OPENGL_LIBRARY})
ADD_DEFINITIONS("-std=c++11 -g")

add_library(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME}
${LIBRARIES}
)
