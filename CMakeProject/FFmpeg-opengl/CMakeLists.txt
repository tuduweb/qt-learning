# cmake 的最小版本要求
cmake_minimum_required(VERSION 3.2)
# 这个CMakeLists管理的工程名称
project(FFmpegOpenGL)
set(CMAKE_CXX_STANDARD 14)
#生成可执行程序 语法:add_executable(可执行程序名 要编译的cpp)

#add_executable(demo demo.cpp) # 生成可执行文件
#add_library(common STATIC util.cpp) # 生成静态库
#add_library(common SHARED util.cpp) # 生成动态库或共享库
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

IF (WIN32)
set(Qt5_DIR  "C:/Qt/5.15.0/msvc2019_64/lib/cmake/Qt5")
ELSEIF (APPLE)
set(Qt5_DIR  "/opt/homebrew/opt/qt5/lib/cmake/qt5")
ENDIF ()

# Find the QtWidgets library
find_package(Qt5 5.3 COMPONENTS
             Core
             Widgets
             Gui)

#include_directories(./core/)
#qt_wrap_ui(ui_FILES ./core/mainwindow.ui)
#aux_source_directory(./core/ DIR_CORE_SRCS)
#aux_source_directory(./core/component/ DIR_CORE_SRCS1)
#aux_source_directory(./core/SONY265/ DIR_CORE_SRCS2)


add_executable(ffmpegOpenGL main.cpp)#${ui_FILES} ${DIR_CORE_SRCS} ${DIR_CORE_SRCS1} ${DIR_CORE_SRCS2}


target_link_libraries(
    ffmpegOpenGL
               Qt5::Core
               Qt5::Widgets
               Qt5::Gui
           )

