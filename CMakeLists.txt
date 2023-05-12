cmake_minimum_required(VERSION 3.24)
project(HexEditorSecondTry C)

set(CMAKE_C_STANDARD 11)

add_executable(HexEditorSecondTry main.c signatures.h signatures.c png_analyse.c png_analyse.h event_handler.c event_handler.h menu.c menu.h globals.h file_work.c stack.c)
