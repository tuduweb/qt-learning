cmake_minimum_required(VERSION 3.5)

project(FFmpeg-OpenGL LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

include_directories("./3rdparty/ffmpeg/include")
link_directories("./3rdparty/ffmpeg/lib")

if(ANDROID)
  add_library(FFmpeg-OpenGL SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
  )
else()
  add_executable(FFmpeg-OpenGL
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    core/remotestreaminterface.cpp
    core/remotestreaminterface.h
    core/videoshowopenglwidget.cpp
    core/videoshowopenglwidget.h

    core/nv12render.cpp
    core/nv12render.h
    core/yuvrender.cpp
    core/yuvrender.h

    core/widget.cpp
    core/widget.h
    core/videodata.cpp
    core/videodata.h
  )
endif()

target_link_libraries(FFmpeg-OpenGL PRIVATE Qt5::Widgets
    avutil avcodec avfilter avdevice avformat postproc swresample)
