cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -ffast-math -mavx -O3")

project(placebo)
link_directories(/usr/lib /usr/local/lib)
add_executable(placebo placebo.c)
target_include_directories(placebo SYSTEM PUBLIC /usr/include /usr/local/include )
target_link_libraries(placebo util)
install(TARGETS placebo DESTINATION /usr/local/bin)
install(PROGRAMS placebo DESTINATION /usr/local/etc/rc.d)
