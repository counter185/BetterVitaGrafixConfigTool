cmake_minimum_required(VERSION 3.2)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(bvgx_config)
include("${VITASDK}/share/vita.cmake" REQUIRED)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

set(VITA_APP_NAME "BetterVitaGrafixConfig")
set(VITA_TITLEID  "CNTRPL005")
set(VITA_VERSION  "01.00")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -DVITASDK -fstack-protector-all -Wall -g")
set(CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES "${VITASDK}/arm-vita-eabi/include/")

include_directories(
  ../common
)

add_executable(${PROJECT_NAME}
src/io.cpp
src/main.cpp
src/sidebar.cpp
src/textrenderer.cpp
src/utils.cpp
src/panel_gameconfig.cpp
src/dropdown.cpp
src/popup.cpp
)

target_link_libraries(${PROJECT_NAME}
  SDL2_image
  SDL2_ttf
  SDL2::SDL2
  SDL2main
  freetype
  png
  z
  bz2
)

vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME} UNSAFE)
vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
  FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
  FILE app_font.ttf app_font.ttf
  FILE app_font_jp.ttf app_font_jp.ttf
  FILE OFL.txt OFL.txt
)
