cmake_minimum_required(VERSION 2.8.11)
project(Final)

set(CMAKE_CXX_FLAGS "-g -Wall")

#required CMake options for QT Apps
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#for window widgets
find_package(OpenGL REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5OpenGL REQUIRED)
add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

#grrr. poorly documented
include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS})


#get symlink_data macro
include(${CMAKE_SOURCE_DIR}/cmake_modules/helpers.cmake)

# below code from example make file
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} .)

#QT headers requiring MOC (meta object compiler)
set(QTHDRS
  mainwindow.h
  mypanelopengl.h
  )

#window design files
set(QTFORMS mainwindow.ui)

#Run qt tools to auto generate C++ compatible files
QT5_WRAP_CPP(QTHDRS_MOC ${QTHDRS})
QT5_WRAP_UI(QTFORMS_HDRS ${QTFORMS})
SET(MYQTFILES ${QTHDRS_MOC} ${QTFORMS_HDRS})

#normal source files
set(SRC
  main.cpp
  mainwindow.cpp
  mypanelopengl.cpp
)

set(SHADERS
  vshader.glsl
  fshader.glsl
  vfraglight.glsl
  ffraglight.glsl
)
ADD_SUBDIRECTORY(common)
ADD_EXECUTABLE(final ${SRC} ${SHDRS} ${MYQTFILES} ${SHADERS})
qt5_use_modules(final OpenGL Widgets)

SET(EXT_LIBS ${OPENGL_LIBRARY} ${GLEW_LIBRARY})
TARGET_LINK_LIBRARIES(final ${EXT_LIBS} libcommon)

symlink_data(final_shaders *.glsl)
