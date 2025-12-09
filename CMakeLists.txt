cmake_minimum_required(VERSION 3.17)
project(HyprLauncher LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)
pkg_check_modules(GTK4_LAYER_SHELL REQUIRED gtk4-layer-shell-0)

add_executable(launcher
    src/main.cpp
    src/AppModel.cpp
    src/LauncherWindow.cpp
)

target_include_directories(launcher PRIVATE src)

target_link_libraries(launcher PRIVATE
    ${GTK4_LIBRARIES}
    ${GTK4_LAYER_SHELL_LIBRARIES}
)

target_compile_options(launcher PRIVATE
    ${GTK4_CFLAGS_OTHER}
    ${GTK4_LAYER_SHELL_CFLAGS_OTHER}
)

target_include_directories(launcher PRIVATE
    ${GTK4_INCLUDE_DIRS}
    ${GTK4_LAYER_SHELL_INCLUDE_DIRS}
)
