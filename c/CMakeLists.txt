CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

PROJECT(ups_c_examples C)

ADD_EXECUTABLE(01_client 01_basic_tcp/client.c)
ADD_EXECUTABLE(01_server 01_basic_tcp/server.c)

FIND_PACKAGE(Threads REQUIRED)

ADD_EXECUTABLE(02_client 02_thread_tcp/client.c)
ADD_EXECUTABLE(02_server 02_thread_tcp/server.c)
TARGET_LINK_LIBRARIES(02_server PRIVATE Threads::Threads)

ADD_EXECUTABLE(03_client 03_select_tcp/client.c)
ADD_EXECUTABLE(03_server 03_select_tcp/server.c)

ADD_EXECUTABLE(04_client 04_poll_tcp/client.c)
ADD_EXECUTABLE(04_server 04_poll_tcp/server.c)

ADD_EXECUTABLE(05_client 05_epoll_tcp/client.c)
ADD_EXECUTABLE(05_server 05_epoll_tcp/server.c)

ADD_EXECUTABLE(06_client 06_udp/client.c)
ADD_EXECUTABLE(06_server 06_udp/server.c)

ADD_EXECUTABLE(07_client 07_unix/client.c)
ADD_EXECUTABLE(07_server 07_unix/server.c)
