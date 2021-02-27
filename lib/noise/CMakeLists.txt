cmake_minimum_required(VERSION 3.13)
project(noise)

add_library(noise ./noise1234.c)
target_include_directories(noise PUBLIC ./include)

