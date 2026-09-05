cmake_minimum_required(VERSION 4.2)
project(TIMP26 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Network Sql Gui Test Widgets REQUIRED)
find_package(OpenSSL REQUIRED)

add_subdirectory(Client)
add_subdirectory(Server)
add_subdirectory(DataBase)
add_subdirectory(Singleton)
add_subdirectory(UnitTests)
