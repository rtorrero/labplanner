cmake_minimum_required(VERSION 3.16)

project(LabPlanner VERSION 0.1 LANGUAGES CXX)

# Enable Qt's automatic handling of .ui, .moc, and .rcc files.
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt (either Qt6 or Qt5) with Widgets
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# List ALL your sources here (CPP, H, UI), so both Qt5 and Qt6 builds use them.
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    csvparser.h
    processor.h
    processor.cpp

)

# For Qt6:
#  Use qt_add_executable(...) with MANUAL_FINALIZATION, then qt_finalize_executable()
# For Qt5 or older:
#  Use add_executable(...)
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(LabPlanner
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    # For Android with Qt6, you might set QT_ANDROID_PACKAGE_SOURCE_DIR here if needed
else()
    if(ANDROID)
        add_library(LabPlanner SHARED
            ${PROJECT_SOURCES}
        )
        # For Android with Qt5, you might set ANDROID_PACKAGE_SOURCE_DIR here
    else()
        add_executable(LabPlanner
            ${PROJECT_SOURCES}
            csvparser.h
            processor.h
            processor.cpp
        )
    endif()
endif()

# Link Widgets
target_link_libraries(LabPlanner PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# For older Qt versions on macOS/iOS, set bundle identifiers (optional).
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.LabPlanner)
endif()

set_target_properties(LabPlanner PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Install rules
include(GNUInstallDirs)
install(TARGETS LabPlanner
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Qt6 needs qt_finalize_executable to apply final settings
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(LabPlanner)
endif()
