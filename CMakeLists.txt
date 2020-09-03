cmake_minimum_required(VERSION 3.16)
project(student_select_class_system C)

set(CMAKE_C_STANDARD 99)

add_executable(student_select_class_system main.c course_select_system.h course_select_system.c json.h cJSON.h cJSON.c)