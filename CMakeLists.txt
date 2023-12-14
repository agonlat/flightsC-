cmake_minimum_required(VERSION 3.5)

project(PPP VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
cmake_policy(SET CMP0074 NEW)
set (BOOST_ROOT "C:/Users/agon_/Downloads/boost_1_83_0/boost_1_83_0")
find_package(Boost REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Boost not found. Please install Boost.")
endif()


#include(FetchContent)

#FetchContent_Declare(
 # Catch2
 # GIT_REPOSITORY https://github.com/catchorg/Catch2.git
 #GIT_TAG        v3.0.1 # or a later release
#)

#FetchContent_MakeAvailable(Catch2)


find_package(Doxygen)

if(DOXYGEN_FOUND)
    # Set input and output directories
    set(DOXYGEN_INPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile")
    set(DOXYGEN_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
    message(${DOXYGEN_INPUT_DIR})

    configure_file(${DOXYGEN_INPUT_DIR} ${DOXYGEN_OUTPUT_DIR}/Doxyfile @ONLY)

    # Create a target for Doxygen
    add_custom_target(
        docs ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUTPUT_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
else()
    message(WARNING "Doxygen not found, unable to generate documentation.")
endif()

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(PPP
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        TimeHandler.cpp
        customitemdelegate.cpp
        FileHandler.cpp
        DataManipulator.cpp
        customitemdelegate.h
        TimeHandler.h
        FileHandler.h
        DataManipulator.h
        Validator.h
        Validator.cpp




        icons.qrc
        utilitymethods.h utilitymethods.cpp
        Data.h




    )

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET PPP APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(PPP SHARED
            ${PROJECT_SOURCES}

        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(PPP
            ${PROJECT_SOURCES}

        )
    endif()
endif()

target_link_libraries(PPP PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${Boost_LIBRARIES})

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.PPP)
endif()
set_target_properties(PPP PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS PPP
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(PPP)
endif()
