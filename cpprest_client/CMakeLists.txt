# CMakeList.txt: CMake_GRPC에 대한 CMake 프로젝트, 여기에 소스를 포함하고
# 프로젝트 특정 논리를 정의합니다.
#
cmake_minimum_required (VERSION 3.8)
message(STATUS "CmakeList at ${CMAKE_CURRENT_SOURCE_DIR}")

# Add include directories & libraries
if (WIN32)   


    # OPENSSL
    include_directories("C:\\mslmlib\\OpenSSL\\include")
    link_directories("C:\\mslmlib\\OpenSSL\\lib")

    # CPPREST
    find_package(cpprestsdk CONFIG REQUIRED)

    set ( PROJECT_LINK_LIBS cpprestsdk::cpprest)

elseif(UNIX)

    find_package(Threads REQUIRED)

    find_package(cpprestsdk CONFIG REQUIRED)

    # External
    link_directories(/usr/lib/x86_64-linux-gnu)
    include_directories(/usr/include)


    set ( PROJECT_LINK_LIBS cpprestsdk::cpprest boost_system)

endif()


# Compile
add_executable (cpprest_client "cpprest_client.cpp" "cpprest_client.h")

if(MSVC)
    #add_definitions()
endif()

if(UNIX)
    #add_definitions()
endif()


# Add to linker
target_link_libraries(cpprest_client ${PROJECT_LINK_LIBS} )

