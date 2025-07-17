cmake_minimum_required(VERSION 3.16.0)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(app)
idf_build_set_property(COMPILE_OPTIONS "-DBLUETOOTH_NIMBLE" APPEND)
