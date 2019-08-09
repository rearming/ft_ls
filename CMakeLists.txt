cmake_minimum_required(VERSION 3.14)
project(ft_ls C)

set(CMAKE_C_STANDARD 11)

set(libft_includes
        libft/includes
        libft/ft_printf/includes)

include_directories(
        ${libft_includes}
        includes
)

link_directories(
        libft
        libft/ft_printf
)

set(main
        sources/ft_ls.c)

add_executable(ft_ls
        ${main})

target_link_libraries(ft_ls
        "ft"
        "ftprintf"
)
