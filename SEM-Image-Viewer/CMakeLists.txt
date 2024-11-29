cmake_minimum_required(VERSION 3.16)

project(
  SEM-Image-Viewer
  VERSION 0.1
  LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

if(PROJECT_IS_TOP_LEVEL AND UNIX)
  # Create symlink to compile_commands.json for IDE to pick it up
  execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_BINARY_DIR}/compile_commands.json
                          ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json)
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Concurrent)

message(STATUS "Qt Version: ${Qt${QT_VERSION_MAJOR}_VERSION}")
message(STATUS "Qt Widgets Include Dir: ${Qt${QT_VERSION_MAJOR}_Widgets_INCLUDE_DIRS}")
message(STATUS "Qt Widgets Library: ${Qt${QT_VERSION_MAJOR}_Widgets_LIBRARIES}")
message(STATUS "Qt Core Include Dir: ${Qt${QT_VERSION_MAJOR}_Core_INCLUDE_DIRS}")
message(STATUS "Qt Core Library: ${Qt${QT_VERSION_MAJOR}_Core_LIBRARIES}")
message(STATUS "Qt Plugins Dir: ${Qt${QT_VERSION_MAJOR}_PLUGINS_DIR}")

set(OpenCV_DIR $ENV{OpenCV})
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(BOOST_ROOT $ENV{Boost})
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Qt6 REQUIRED COMPONENTS Charts)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui


    core/data/image.cpp
    core/data/image.h
    core/data/image_meta_data.cpp
    core/data/image_meta_data.h
    core/data/image_repository.cpp
    core/data/image_repository.h
    core/data/image_state.cpp
    core/data/image_state.h
    core/data/imessage.cpp
    core/data/imessage.h


    core/data/session_data.cpp
    core/data/session_data.h


    core/engines/json_vsitor.cpp
    core/engines/json_vsitor.h
    core/engines/logger.cpp
    core/engines/logger.h
    core/engines/workspace.cpp
    core/engines/workspace.h
    core/filters/edge_detection_filter.cpp
    core/filters/edge_detection_filter.h
    core/filters/gray_scale_filter.cpp
    core/filters/gray_scale_filter.h
    core/filters/image_filter.cpp
    core/filters/image_filter.h
    core/filters/noise_reduction_filter.cpp
    core/filters/noise_reduction_filter.h
    core/filters/sharpen_filter.cpp
    core/filters/sharpen_filter.h
    core/image_editor.h
    core/image_editor.cpp
    core/utils.cpp
    core/utils.h
    core/data/image_format.cpp
    core/data/image_format.h
    core/data/image_color_space.cpp
    core/data/image_color_space.h


    # core/filters/noise_reduction_filter.cpp core/filters/noise_reduction_filter.h
    widgets/action_list_widget.cpp
    widgets/action_list_widget.h
    widgets/bottom_middle_widget.cpp
    widgets/bottom_middle_widget.h
    widgets/controller_widget.cpp
    widgets/controller_widget.h
    widgets/diff_view_widget.cpp
    widgets/diff_view_widget.h
    widgets/edge_extraction_wigdet.h
    widgets/edge_extraction_wigdet.cpp
    widgets/grid_view_widget.cpp
    widgets/grid_view_widget.h
    widgets/history_widget.h
    widgets/history_widget.cpp
    widgets/image_info_bar.cpp
    widgets/image_info_bar.h
    widgets/image_info_bar.h
    widgets/image_info_bar.cpp
    widgets/image_widget_core.cpp
    widgets/image_widget_core.h
    widgets/image_widget.cpp
    widgets/image_widget.h
    widgets/image_widget.cpp
    widgets/image_widget.h
    widgets/left_side_bar_widget.cpp
    widgets/left_side_bar_widget.h
    widgets/logger_widget.cpp
    widgets/logger_widget.h
    widgets/menu_bar_widget.cpp
    widgets/menu_bar_widget.h
    widgets/mini_grid_widget.cpp
    widgets/mini_grid_widget.h
    widgets/progress_bar.h
    widgets/progress_bar.cpp
    widgets/right_side_bar_widget.cpp
    widgets/right_side_bar_widget.h


    widgets/exit_dialog_widget.cpp
    widgets/exit_dialog_widget.h
    widgets/grayscale_widget.cpp
    widgets/grayscale_widget.h
    widgets/noise_reduction_widget.cpp
    widgets/noise_reduction_widget.h
    widgets/sharpen_filter_widget.cpp
    widgets/sharpen_filter_widget.h
    widgets/top_middle_widget.cpp
    widgets/top_middle_widget.h
    widgets/zoom_widget.cpp
    widgets/zoom_widget.h
    widgets/thumbnail_delegate.h
    widgets/file_browser_widget.cpp
    widgets/file_browser_widget.h
    widgets/file_icon_hider.cpp
    widgets/file_icon_hider.h
    widgets/image_filter_proxy_model.cpp
    widgets/image_filter_proxy_model.h
    widgets/image_dialog.cpp
    widgets/image_dialog.h
    widgets/toolbar_widget.cpp
    widgets/toolbar_widget.h


    models/image_data_model.cpp
    models/image_data_model.h

    # app_icon.rc
    resources.qrc

    widgets/log_card_widget.h
    widgets/log_card_widget.cpp

    widgets/logger_controller.h
    widgets/logger_controller.cpp



    widgets/heat_map_widget.cpp
    widgets/heat_map_widget.h
    widgets/heatmap_intensity_widget.h
    widgets/heatmap_intensity_widget.cpp

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(SEM-Image-Viewer MANUAL_FINALIZATION ${PROJECT_SOURCES}
      core/filters/batch_filter.cpp core/filters/batch_filter.h
      widgets/horizontal_intensity_plot.h widgets/horizontal_intensity_plot.cpp
      core/data/image_cache_pool.cpp core/data/image_cache_pool.h core/data/image_cache_task.cpp core/data/image_cache_task.h core/data/image_cacheless.cpp core/data/image_cacheless.h core/data/image_state_cacheless.cpp core/data/image_state_cacheless.h image_cache_config.h






      )
  # Define target properties for Android with Qt 6 as: set_property(TARGET SEM-Image-Viewer APPEND PROPERTY
  # QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android) For more information, see
  # https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
  if(ANDROID)
    add_library(SEM-Image-Viewer SHARED ${PROJECT_SOURCES})
    # Define properties for Android with Qt 5 after find_package() calls as: set(ANDROID_PACKAGE_SOURCE_DIR
    # "${CMAKE_CURRENT_SOURCE_DIR}/android")
  else()
    add_executable(SEM-Image-Viewer ${PROJECT_SOURCES})
  endif()
endif()

target_link_libraries(SEM-Image-Viewer PRIVATE Qt6::Charts ${OpenCV_LIBS} ${Boost_LIBRARIES} Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Concurrent)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1. If you are developing for iOS or macOS you
# should consider setting an explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.SEM-Image-Viewer)
endif()
set_target_properties(
  SEM-Image-Viewer
  PROPERTIES ${BUNDLE_ID_OPTION} MACOSX_BUNDLE_BUNDLE_VERSION
             ${PROJECT_VERSION} MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} MACOSX_BUNDLE
             TRUE WIN32_EXECUTABLE
             TRUE)

include(GNUInstallDirs)
install(
  TARGETS SEM-Image-Viewer
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(SEM-Image-Viewer)
endif()
