# CMakeList.txt: CMake_GRPC에 대한 CMake 프로젝트, 여기에 소스를 포함하고
# 프로젝트 특정 논리를 정의합니다.
#
cmake_minimum_required (VERSION 3.8)
message(STATUS "CmakeList at ${CMAKE_CURRENT_SOURCE_DIR}")

# Add include directories & libraries
if (WIN32)   

    # GRPC
    include_directories("C:\\mslmlib\\grpc_1.38\\include")
    link_directories("C:\\mslmlib\\grpc_1.38\\lib\\Debug")
    set(GRPC_LIBS "C:\\mslmlib\\grpc_1.38\\lib\\Debug\\*.lib")

    # OPENSSL
    include_directories("C:\\mslmlib\\OpenSSL\\include")
    link_directories("C:\\mslmlib\\OpenSSL\\lib")

    set ( PROJECT_LINK_LIBS ${GRPC_LIBS} libcrypto.lib libssl.lib ws2_32.lib)

elseif(UNIX)

    find_package(Threads REQUIRED)

    # Protobuf
    find_package(Protobuf CONFIG REQUIRED)
    message(STATUS "Using protobuf ${Protobuf_VERSION}")

    # GRPC
    find_package(gRPC CONFIG REQUIRED)
    message(STATUS "Using grpc ${gRPC_VERSION}")

    # External
    #link_directories(/usr/lib/x86_64-linux-gnu)
    #include_directories(/usr/include)

    set ( PROJECT_LINK_LIBS gRPC::grpc++ gRPC::grpc++_reflection protobuf::libprotobuf protobuf::libprotoc)

endif()


# Compile
add_executable(grpc_keepalive_server "greeter_server.cc" "helloworld.grpc.pb.cc" "helloworld.grpc.pb.h" "helloworld.pb.cc" "helloworld.pb.h")

if(MSVC)
    #add_definitions(-DGRPC_CALLBACK_API_NONEXPERIMENTAL)
    #add_definitions(-DNOMINMAX)
endif()

if(UNIX)
    #add_definitions(-DGRPC_CALLBACK_API_NONEXPERIMENTAL)
endif()


# Add to linker
target_link_libraries(grpc_keepalive_server ${PROJECT_LINK_LIBS} )

