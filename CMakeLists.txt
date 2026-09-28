cmake_minimum_required(VERSION 3.10)
project(my_matrix_project C)

set(CMAKE_C_STANDARD 11)

add_executable(main 
    main.c 
    matrix.c 
    numbers.c
)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_C_FLAGS_RELEASE "-O3 -march=native")