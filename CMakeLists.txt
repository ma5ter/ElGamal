cmake_minimum_required(VERSION 3.23)
project(ElGamal C)

set(CMAKE_C_STANDARD 99)

add_executable(ElGamal main.c elgamal.c prime.c)
