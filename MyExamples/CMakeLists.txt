# Copyright 2019 Proyectos y Sistemas de Mantenimiento SL (eProsima).
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 2.8.12)

if(NOT CMAKE_VERSION VERSION_LESS 3.0)
    cmake_policy(SET CMP0048 NEW)
endif()

project(LearnDDS)

# Find requirements
if(NOT fastcdr_FOUND)
    find_package(fastcdr REQUIRED)
endif()

if(NOT fastrtps_FOUND)
    find_package(fastrtps REQUIRED)
endif()

#Check C++11
include(CheckCXXCompilerFlag)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG OR
        CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    check_cxx_compiler_flag(-std=c++11 SUPPORTS_CXX11)
    if(NOT SUPPORTS_CXX11)
        message(FATAL_ERROR "Compiler doesn't support C++11")
    endif()
endif()

###################################################################################

message(STATUS "Configuring HelloWorld example...")
include_directories(include/HelloWorldMyExample)

file(GLOB HELLOWORLD_EXAMPLE_SOURCES_CXX "src/HelloWorldMyExample/*.cxx")
file(GLOB HELLOWORLD_EXAMPLE_SOURCES_CPP "src/HelloWorldMyExample/*.cpp")


# Publisher Executables
add_executable(HelloWorldPublisher src/HelloWorldMyExample/HelloWorldPublisher.cpp ${HELLOWORLD_EXAMPLE_SOURCES_CXX})
target_link_libraries(HelloWorldPublisher fastrtps fastcdr)

# Subscriber Executables
add_executable(HelloWorldSubscriber src/HelloWorldMyExample/HelloWorldSubscriber.cpp ${HELLOWORLD_EXAMPLE_SOURCES_CXX})
target_link_libraries(HelloWorldSubscriber fastrtps fastcdr)


###################################################################################

message(STATUS "Configuring IIT test example...")
include_directories(include/IITCommunicationExample)

file(GLOB IIT_TEST_EXAMPLE_SOURCES_CXX "src/IITCommunicationExample/*.cxx")
file(GLOB IIT_TEST_EXAMPLE_SOURCES_CPP "src/IITCommunicationExample/*.cpp")


# IIT-Publisher Executables
# Publisher Executable
add_executable(MapElementPublisher src/IITCommunicationExample/MapElementPublisher.cpp ${IIT_TEST_EXAMPLE_SOURCES_CXX})
target_link_libraries(MapElementPublisher fastrtps fastcdr)

# Publisher Executable
add_executable(MapElementSubscriber src/IITCommunicationExample/MapElementSubscriber.cpp ${IIT_TEST_EXAMPLE_SOURCES_CXX})
target_link_libraries(MapElementSubscriber fastrtps fastcdr)

