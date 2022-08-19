cmake_minimum_required(VERSION 3.10)

project(locenv CXX)

add_library(locenv STATIC
	src/api.cpp
	src/context.cpp)

target_compile_features(locenv PUBLIC cxx_std_17)
target_include_directories(locenv PUBLIC include)
