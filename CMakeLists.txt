﻿# CMakeList.txt : 최상위 CMake 프로젝트 파일, 전역 구성을 수행하고
# 여기에 하위 프로젝트를 포함합니다.
#
cmake_minimum_required (VERSION 3.8)

project ("CMake_GRPC")

# 3rdPaty Plog
include_directories(${PROJECT_SOURCE_DIR}/3rdParty)

add_subdirectory ("rg_callback_server")

add_subdirectory("rg_callback_client")

add_subdirectory("cpprest_client")

add_subdirectory("grpc_keepalive_client")

add_subdirectory("grpc_keepalive_server")

add_subdirectory("Cloudein_API_Client")

add_subdirectory("Cloudein_API_Server")

add_subdirectory("Cpprest_API")
