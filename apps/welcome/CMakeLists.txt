cmake_minimum_required(VERSION 3.20)

project(yummy-welcome VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Quick Qml Svg)
find_package(KF6 REQUIRED COMPONENTS CoreAddons I18n Config DBusAddons IconThemes WindowSystem)

set(SOURCES
    src/main.cpp
    src/backend/backend.cpp
)

set(HEADERS
    src/backend/backend.h
)

set(QML_FILES
    src/qml/main.qml
    src/qml/components/StepIndicator.qml
    src/qml/components/PageCard.qml
    src/qml/components/NetworkPage.qml
    src/qml/components/DriversPage.qml
    src/qml/components/UpdatesPage.qml
    src/qml/components/GamingPage.qml
    src/qml/components/DeveloperPage.qml
    src/qml/components/AppearancePage.qml
    src/qml/components/ImportPage.qml
    src/qml/components/DonePage.qml
)

set(RESOURCES
    resources.qrc
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${RESOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Widgets
    Qt6::Quick
    Qt6::Qml
    Qt6::Svg
    KF6::CoreAddons
    KF6::I18n
    KF6::Config
    KF6::DBusAddons
    KF6::IconThemes
    KF6::WindowSystem
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

install(TARGETS ${PROJECT_NAME} DESTINATION /usr/bin)
