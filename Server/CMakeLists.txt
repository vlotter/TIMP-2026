project(Server)

add_executable(Server
    main.cpp
    mytcpserver.cpp
    mytcpserver.h
    functionsforserver.cpp
    functionsforserver.h
    database.cpp
    database.h
    customsha512.cpp
    customsha512.h
)

target_link_libraries(Server PRIVATE Qt6::Core Qt6::Network Qt6::Sql OpenSSL::SSL OpenSSL::Crypto)
