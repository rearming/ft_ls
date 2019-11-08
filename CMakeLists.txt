cmake_minimum_required(VERSION 3.14)
project(ft_ls C)

set(CMAKE_C_STANDARD 11)

add_compile_options(-Wall -Wextra)

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_compile_options(-O2 -Werror)
endif()

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
        sources/ft_ls.c
        sources/ls_recursive.c
        sources/error.c
        sources/dir_tree_getter.c
        sources/compare_functions.c
        sources/filestruct_functions.c
        sources/options_parser.c
        sources/format_print.c
        sources/rights_field_getter.c
        sources/coloring.c
        sources/format_print_utils.c
        sources/size_print_utils.c
        sources/dir_tree_utils.c sources/args_processor.c sources/utils.c)

set(test
#        tests/profiling_test.c
        tests/test_R.c)


add_executable(ft_ls
        ${main}
        )

target_link_libraries(ft_ls
        "ft"
        "ftprintf"
        )

add_executable(test
        ${test}
        )


target_link_libraries(test
        "ft"
        "ftprintf"
        )

