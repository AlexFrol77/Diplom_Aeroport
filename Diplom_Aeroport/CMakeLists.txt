cmake_minimum_required(VERSION 3.5)

project(Diplom_Aeroport VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets Charts Sql Concurrent PrintSupport REQUIRED)


set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        database.h
        database.cpp
        statisticflight.h
        statisticflight.cpp
        statisticflight.ui
)

add_executable(Diplom_Aeroport WIN32
               ${PROJECT_SOURCES}
)

add_compile_definitions(QCUSTOMPLOT_USE_LIBRARY)
target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/PG_Libs)
target_link_libraries(Diplom_Aeroport PRIVATE  Qt6::PrintSupport
                                               qcustomplot2.dll
                                               Qt6::Widgets
                                               Qt6::Charts
                                               Qt6::Sql
                                               Qt6::Concurrent
                                               libcrypto-3-x64
                                               libiconv-2
                                               libintl-9
                                               libpq
                                               libssl-3-x64)

qt_finalize_executable(Diplom_Aeroport)
