cmake_minimum_required(VERSION 3.10)
project(Module10_Elevator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

cmake_policy(SET CMP0167 NEW)

# Search for Boost.Log and Boost.System
find_package(Boost 1.70 REQUIRED COMPONENTS log system)

if(Boost_FOUND)
    message(STATUS "Boost found! Include directories: ${Boost_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "Boost not found.")
endif()

# Create the executable
add_executable(Module10_Elevator
        main.cpp
        ElevatorSimulation.cpp
        ElevatorSimulation.h
        Elevator.cpp
        Elevator.h
        Floor.cpp
        Floor.h
        Passenger.cpp
        Passenger.h
)

# Link Boost libraries
target_link_libraries(Module10_Elevator
        ${Boost_LIBRARIES}
        pthread
)

# Define BOOST_LOG_DYN_LINK
add_definitions(-DBOOST_LOG_DYN_LINK)