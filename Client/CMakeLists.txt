project(Client)

add_executable(Client
    main.cpp
    clientconnection.cpp
    clientconnection.h
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    logindialog.cpp
    logindialog.h
    logindialog.ui
)

target_link_libraries(Client PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network)
