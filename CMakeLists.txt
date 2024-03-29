cmake_minimum_required(VERSION 3.24)
project(no_sleep)

# personal config for Qt. You only need to modify this if you are on windows.
set(QT6_INSTALL_PATH D:/pkg/Qt/6.3.1/msvc2019_64)
set(CMAKE_PREFIX_PATH ${QT6_INSTALL_PATH})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_FLAGS_RELEASE "/O2")

if (MSVC)
    add_compile_options(/Zc:__cplusplus)
endif()

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Svg
        REQUIRED)

add_executable(no_sleep
        main.cpp
        core_instance.cpp
        core_instance.h
        sleep_controller.cpp
        sleep_controller.h
        icons.qrc
        appicon.o)

target_link_libraries(no_sleep
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Svg)

if (WIN32)
    target_link_libraries(no_sleep powrprof)
    add_executable(GuiApplication WIN32 main.cpp)
    set_target_properties(no_sleep PROPERTIES WIN32_EXECUTABLE TRUE)
endif ()

set_target_properties(no_sleep PROPERTIES OUTPUT_NAME "No Sleep")


if (WIN32)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    if (NOT EXISTS "${QT6_INSTALL_PATH}/bin")
        set(QT6_INSTALL_PATH "${QT6_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT6_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT6_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT6_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT6_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB core gui Widgets Svg)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT6_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
