add_executable(httpserver)

target_sources(httpserver PUBLIC
    tcpserver.cpp
    main.cpp
)

target_compile_options(httpserver PUBLIC
    -Wall
    -Wextra
    -Wpedantic
    -std=c++20
    -g
)

target_link_libraries(httpserver PRIVATE ws2_32)