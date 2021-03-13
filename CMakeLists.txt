cmake_minimum_required(VERSION 3.5)

project(snp_reproduce LANGUAGES C)

add_executable(snp_reproduce main.c)

target_link_libraries(snp_reproduce -pthread)
