cmake_minimum_required(VERSION 3.14)

project(openCvInQtTest LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Widgets REQUIRED)
find_package(OpenCV REQUIRED)

set(PROJECT_HEADERS
  huecontroller.h
)

set(PROJECT_SOURCES
  main.cpp
  huecontroller.cpp
  qml.qrc
)

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(openCvInQtTest
  ${PROJECT_SOURCES}
  ${PROJECT_HEADERS}
)


target_link_libraries(openCvInQtTest
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Quick ${OpenCV_LIBS})
