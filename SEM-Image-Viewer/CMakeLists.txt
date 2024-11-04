cmake_minimum_required(VERSION 3.16)

project(SEM-Image-Viewer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (PROJECT_IS_TOP_LEVEL AND UNIX)
    # Create symlink to compile_commands.json for IDE to pick it up
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
    )
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(PROJECT_SOURCES
        main.cpp
        mainwindow.h
        mainwindow.cpp
        mainwindow.ui





        core/image.h
        core/image.cpp
        core/image_utils.h
        core/image_utils.cpp

        core/filters/image_filter.h
        core/filters/sharpen_filter.h
        core/filters/sharpen_filter.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SEM-Image-Viewer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        core/filters/gray_scale_filter.h
        widgets/zoom_widget.h widgets/zoom_widget.cpp
        resources.qrc
        icons/zoom-in.svg icons/zoom-out.svg
        widgets/image_widget.h widgets/image_widget.cpp
        widgets/sharpen_filter_widget.h
        widgets/sharpen_filter_widget.cpp
        core/filters/noise_reduction_filter.cpp core/filters/noise_reduction_filter.h
        widgets/image_info_bar.h widgets/image_info_bar.cpp
        widgets/bottommiddlewidget.cpp widgets/bottommiddlewidget.h widgets/image_info_bar.cpp widgets/image_info_bar.h widgets/image_widget.cpp widgets/image_widget.h widgets/leftsidebarwidget.cpp widgets/leftsidebarwidget.h widgets/menubarwidget.cpp widgets/menubarwidget.h widgets/rightsidebarwidget.cpp widgets/rightsidebarwidget.h widgets/sharpen_filter_widget.cpp widgets/sharpen_filter_widget.h widgets/topmiddlewidget.cpp widgets/topmiddlewidget.h widgets/zoom_widget.cpp widgets/zoom_widget.h
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET SEM-Image-Viewer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(SEM-Image-Viewer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(SEM-Image-Viewer
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(SEM-Image-Viewer PRIVATE ${OpenCV_LIBS} ${Boost_LIBRARIES} Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.SEM-Image-Viewer)
endif()
set_target_properties(SEM-Image-Viewer PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS SEM-Image-Viewer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SEM-Image-Viewer)
endif()
