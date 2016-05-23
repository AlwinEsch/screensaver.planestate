project(screensaver.planestate)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

enable_language(CXX)

find_package(Kodi REQUIRED)
find_package(SOIL REQUIRED)

include_directories(${XBMC_INCLUDE_DIR}
                    ${SOIL_INCLUDE_DIRS})

set(PLANESTATE_SOURCES src/animatorfloat.cpp
                       src/background.cpp
                       src/main.cpp
                       src/planestate.cpp)

set(DEPLIBS ${SOIL_LIBRARIES})

build_addon(screensaver.planestate PLANESTATE DEPLIBS)

include(CPack)
