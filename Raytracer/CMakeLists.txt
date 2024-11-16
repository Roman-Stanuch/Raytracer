cmake_minimum_required(VERSION "3.29.5")

project("Raytracer")
add_executable("Raytracer" "src/main.cpp" "src/classes/ray.h")
target_include_directories("Raytracer" PUBLIC "src/classes")