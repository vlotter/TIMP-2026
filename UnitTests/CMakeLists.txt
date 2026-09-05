project(UnitTests)

add_executable(UnitTests
    main.cpp
    ../Server/functionsforserver.cpp
    ../Server/functionsforserver.h
    ../Server/database.cpp
    ../Server/database.h
    ../Server/customsha512.cpp
    ../Server/customsha512.h
)

target_link_libraries(UnitTests PRIVATE Qt6::Core Qt6::Test Qt6::Network Qt6::Sql OpenSSL::SSL OpenSSL::Crypto)

target_include_directories(UnitTests PRIVATE ../Server)
