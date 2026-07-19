cmake_minimum_required(VERSION 3.20)
project(yummy-control-center VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)

find_package(Qt6 REQUIRED COMPONENTS Widgets Quick Qml)
find_package(KF6 REQUIRED COMPONENTS
    CoreAddons
    I18n
    Config
    KIO
    IconThemes
    DBusAddons
    WindowSystem
    Plasma
)

feature_summary(WHAT ALL FATAL_ON_PACKAGE_REQUIRED_FOUND)

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/sidebarwidget.cpp
    src/pages/appearancepage.cpp
    src/pages/performancepage.cpp
    src/pages/gamingpage.cpp
    src/pages/privacypage.cpp
    src/pages/systeminfopage.cpp
    src/pages/audiopage.cpp
    src/pages/displaypage.cpp
    src/pages/powerpage.cpp
    src/pages/startupappspage.cpp
    src/pages/packagespage.cpp
    src/pages/networkpage.cpp
    src/pages/updatespage.cpp
    src/pages/themespage.cpp
    src/pages/wallpaperspage.cpp
    src/pages/backuppage.cpp
    src/pages/bluetoothpage.cpp
    src/pages/driverspage.cpp
    src/pages/useraccountspage.cpp
    src/utils/configmanager.cpp
    src/utils/systemutils.cpp
)

set(HEADERS
    src/mainwindow.h
    src/sidebarwidget.h
    src/pages/appearancepage.h
    src/pages/performancepage.h
    src/pages/gamingpage.h
    src/pages/privacypage.h
    src/pages/systeminfopage.h
    src/pages/audiopage.h
    src/pages/displaypage.h
    src/pages/powerpage.h
    src/pages/startupappspage.h
    src/pages/packagespage.h
    src/pages/networkpage.h
    src/pages/updatespage.h
    src/pages/themespage.h
    src/pages/wallpaperspage.h
    src/pages/backuppage.h
    src/pages/bluetoothpage.h
    src/pages/driverspage.h
    src/pages/useraccountspage.h
    src/utils/configmanager.h
    src/utils/systemutils.h
)

set(QML_FILES
    src/qml/theme.qml
)

qt_add_resources(QRC_SOURCES resources.qrc)

add_executable(yummy-control-center ${SOURCES} ${HEADERS})

target_include_directories(yummy-control-center PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(yummy-control-center PRIVATE
    Qt6::Widgets
    Qt6::Quick
    Qt6::Qml
    KF6::CoreAddons
    KF6::I18n
    KF6::Config
    KF6::KIO
    KF6::IconThemes
    KF6::DBusAddons
    KF6::WindowSystem
    KF6::Plasma
)

install(TARGETS yummy-control-center DESTINATION ${KDE_INSTALL_BINDIR})
install(FILES src/qml/theme.qml DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/yummy/controlcenter)
