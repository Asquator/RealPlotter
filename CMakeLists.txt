cmake_minimum_required(VERSION 3.5)

project(RealPlotter VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        src/main.cpp
        src/plotwidget.cpp
        src/function_list_model.cpp
        src/function_entry_delegate.cpp
        src/function_list_view.cpp
        src/function_entry.cpp
        src/function_table.cpp
        src/line_editor.cpp

        src/line_editor.h
        src/function_table.h
        src/function_list_view.h
        src/function_entry_delegate.h
        src/function_list_model.h
        src/plotwidget.h
        src/function_entry.h

        src/Graphics/plot_scene.cpp
        src/Graphics/plot_view.cpp

        src/Graphics/plot_view.h
        src/Graphics/plot_scene.h

        src/Parser/real_parser.h
        src/Parser/parse_worker.h

        src/Parser/real_parser.cpp
        src/Parser/parse_worker.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(RealPlotter
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET RealPlotter APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(RealPlotter SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_
add_executable(RealPlotter
    ${PROJECT_SOURCES}DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(RealPlotter
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(RealPlotter PRIVATE ${CMAKE_SOURCE_DIR}/src
                                        PRIVATE ${CMAKE_SOURCE_DIR}/src/Parser
                                        PRIVATE ${CMAKE_SOURCE_DIR}/src/Graphics)

target_link_libraries(RealPlotter PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                                    PRIVATE RealFunctionLib)

set_target_properties(RealPlotter PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS RealPlotter
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(RealPlotter)
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/lib/RealFunctionLib)

#add_subdirectory(${CMAKE_SOURCE_DIR}/tests)


