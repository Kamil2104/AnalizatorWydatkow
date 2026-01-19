cmake_minimum_required(VERSION 3.16)
project(ExpenseManager)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

set(SOURCES
    main.cpp
    CSVreader.cpp
    mainwindow.cpp
)

set(HEADERS
    CSVreader.h
    mainwindow.h
)

add_executable(ExpenseManager ${SOURCES} ${HEADERS})

target_link_libraries(ExpenseManager Qt5::Core Qt5::Widgets)

