cmake_minimum_required(VERSION 3.16)

project(SEM-Image-Viewer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_CXX_COMPILER "/usr/bin/clang++")


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

set(OpenCV_DIR $ENV{OpenCV})

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(BOOST_ROOT $ENV{Boost})


find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui

        widgets/image_widget.h
        widgets/image_widget.cpp

        widgets/LoggerWidget.h
        widgets/LoggerWidget.cpp
        assets/errors_icon.png assets/information_icon.png assets/micro-electronic-sed.jpg assets/warnings_icon.png



        core/data/ImageMetadata.cpp core/data/ImageMetadata.h
        core/data/ImageFormat.h

        core/data/ErrorMessage.cpp core/data/ErrorMessage.h core/data/ImageFormat.h core/data/ImageMetadata.cpp core/data/ImageMetadata.h core/data/IMessage.cpp core/data/IMessage.h core/data/InfoMessage.cpp core/data/InfoMessage.h core/data/WarningMessage.cpp core/data/WarningMessage.h     core/engines/Logger.cpp core/engines/Logger.h
        core/filters/EdgeDetectionFilter.cpp core/filters/EdgeDetectionFilter.h core/filters/NoiseReductionFilter.cpp core/filters/NoiseReductionFilter.h core/filters/GrayScaleFilter.cpp core/filters/GrayScaleFilter.h core/filters/ImageFilter.h
        core/filters/SharpenFilter.cpp core/filters/SharpenFilter.h
        core/data/Image.cpp core/data/Image.h
        core/engines/Workspace.cpp core/engines/Workspace.h

        core/data/ImageColorSpace.h

        widgets/MiniGrid.cpp widgets/MiniGrid.h
        widgets/GridView.cpp widgets/GridView.h
        widgets/zoom_widget.h widgets/zoom_widget.cpp
        widgets/image_info_bar.h widgets/image_info_bar.cpp
        resources.qrc

        icons/zoom-in.svg icons/zoom-out.svg
        icons/redo-icon.svg icons/undo-icon.svg
        icons/undo-icon-dark-mode.svg
        icons/redo-icon-dark-mode.svg
        icons/play-icon-dark-mode.svg
        icons/zoom-in-dark-mode.svg
        icons/zoom-out-dark-mode.svg
        widgets/image_widget.h widgets/image_widget.cpp
        widgets/sharpen_filter_widget.h
        widgets/sharpen_filter_widget.cpp
        # core/filters/noise_reduction_filter.cpp core/filters/noise_reduction_filter.h
        widgets/image_info_bar.h widgets/image_info_bar.cpp
        widgets/savedialogwidget.cpp
        widgets/savedialogwidget.h
        widgets/bottommiddlewidget.cpp widgets/bottommiddlewidget.h widgets/image_info_bar.cpp widgets/image_info_bar.h widgets/image_widget.cpp widgets/image_widget.h widgets/leftsidebarwidget.cpp widgets/leftsidebarwidget.h widgets/menubarwidget.cpp widgets/menubarwidget.h widgets/rightsidebarwidget.cpp widgets/rightsidebarwidget.h widgets/sharpen_filter_widget.cpp widgets/sharpen_filter_widget.h widgets/topmiddlewidget.cpp widgets/topmiddlewidget.h widgets/zoom_widget.cpp widgets/zoom_widget.h
        models/ImageDataModel.cpp
        models/ImageDataModel.h
        models/MessageDataModel.cpp
        models/MessageDataModel.h
        widgets/edge_extraction_wigdet.h widgets/edge_extraction_wigdet.cpp
        widgets/controllerWidget.h
        widgets/controllerWidget.cpp
        widgets/ThumbnailDelegate.h
        core/imageeditor.h core/imageeditor.cpp
        widgets/historywidget.h widgets/historywidget.cpp
        core/data/ImageState.cpp core/data/ImageState.h
        core/data/ImageRepository.cpp core/data/ImageRepository.h core/data/SessionData.cpp core/data/SessionData.h
        widgets/grayscalewidget.h widgets/grayscalewidget.cpp
        widgets/noisereductionwidget.h widgets/noisereductionwidget.cpp
        widgets/actionListWidget.cpp
        widgets/actionlistwidget.h

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SEM-Image-Viewer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        widgets/progressbar.h
        widgets/progressbar.cpp
        icons/play-icon.svg
        widgets/savedialogwidget.h widgets/savedialogwidget.cpp





        widgets/FileBrowserWidget.cpp widgets/FileBrowserWidget.h widgets/FileIconHider.cpp widgets/FileIconHider.h widgets/ImageFilterProxyModel.cpp widgets/ImageFilterProxyModel.h
        widgets/ImageDialog.cpp widgets/ImageDialog.h
        widgets/WidgetViewController.cpp widgets/WidgetViewController.h



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
