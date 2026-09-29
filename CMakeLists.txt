cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(debug_flpr_ipc)

# Collect all application sources under src (flat or nested) and add them to the app target.
file(GLOB_RECURSE app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})

# Add the public include directory for headers in inc.
target_include_directories(app PRIVATE inc)
