# CMakeList.txt: CMake_GRPC에 대한 CMake 프로젝트, 여기에 소스를 포함하고
# 프로젝트 특정 논리를 정의합니다.
#
cmake_minimum_required (VERSION 3.8)


# Add include directories & libraries
if (WIN32)   

    # GRPC
    include_directories("C:\\mslmlib\\grpc_1.38\\include")
    link_directories("C:\\mslmlib\\grpc_1.38\\lib\\Debug")

    # OPENSSL
    include_directories("C:\\mslmlib\\OpenSSL\\include")
    link_directories("C:\\mslmlib\\OpenSSL\\lib")

    set ( PROJECT_LINK_LIBS "C:\\mslmlib\\grpc_1.38\\lib\\Debug\\*.lib" libcrypto.lib libssl.lib ws2_32.lib)

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

    set ( PROJECT_LINK_LIBS gRPC::grpc++ protobuf::libprotobuf protobuf::libprotoc)

endif()


# Compile
add_executable (CMake_GRPC "helper.cc" "helper.h"
							"route_guide.grpc.pb.cc" "route_guide.grpc.pb.h" "route_guide.pb.cc" "route_guide.pb.h"												
							"route_guide_callback_server.cc"
				)

if(MSVC)
    add_definitions(-DGRPC_CALLBACK_API_NONEXPERIMENTAL)
endif()

if(UNIX)
    add_definitions(-DGRPC_CALLBACK_API_NONEXPERIMENTAL)
endif()


# Add to linker
target_link_libraries(CMake_GRPC ${PROJECT_LINK_LIBS} )


