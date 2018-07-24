cmake_minimum_required(VERSION 2.11)

set (CMAKE_AUTOMOC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

set (PROJECT_NAME IrcBookFinder)
set (EXE_NAME bookfinder)

set(CMAKE_CXX_FLAGS "-Wall")

project(${PROJECT_NAME})

find_package( Qt5Core )
find_package( Qt5Widgets )

set (f_src src/main.cpp src/mainwindow.cpp)
set (f_inc inc/mainwindow.h)
include_directories(inc)


qt5_wrap_cpp(f_moc_inc ${f_inc})
add_executable(${EXE_NAME} ${f_src} ${f_moc_inc}) 
qt5_use_modules(${EXE_NAME} Widgets)


# generates the compilation database for YouCompleteMe to use
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


