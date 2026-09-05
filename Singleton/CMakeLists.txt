project(Singleton)

add_executable(Singleton main.cpp)

target_link_libraries(Singleton PRIVATE
        Qt6::Core
        Qt6::Sql
)
