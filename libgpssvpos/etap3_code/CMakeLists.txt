cmake_minimum_required(VERSION 2.8)

project(etap3_code)

add_executable(${PROJECT_NAME}
    header/head.h
    src/otpravit.cpp
    src/data.cpp
    src/toint.cpp
    src/Kepler.cpp
    src/coordinate.cpp
    src/main.cpp
    )
