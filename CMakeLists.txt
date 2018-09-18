cmake_minimum_required(VERSION 3.0)
set (QT_MIN_VERSION "5.7.0")
set (KF5_MIN_VERSION "5.30.0")

project( amor )

find_package(ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

# Search KDE installation
#find_package( KDE4 REQUIRED )
find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED
    Core
    Widgets
    DBus
    X11Extras
)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED
    Config
    Crash
    DocTools
    I18n
    TextWidgets
    CoreAddons
)

#include( KDE4Defaults )
#include( MacroLibrary )
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)
include(ECMInstallIcons)

add_definitions( -DQT_NO_CAST_FROM_ASCII )
add_definitions( -DQT_NO_CAST_TO_ASCII )
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

add_definitions(-DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT)
add_definitions(-DQT_NO_URL_CAST_FROM_STRING)
add_definitions(-DQT_USE_QSTRINGBUILDER)
# The FindKDE4.cmake module sets _KDE4_PLATFORM_DEFINITIONS with
# definitions like _GNU_SOURCE that are needed on each platform.
# Put configure checks below this line.

add_subdirectory( data )
add_subdirectory( src )
add_subdirectory( doc )

feature_summary(WHAT ALL
    INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
