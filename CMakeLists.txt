cmake_minimum_required(VERSION 3.15)
project(hw3)

set(CMAKE_CXX_STANDARD 11)

add_executable(hw3 MtmParkingLot.cpp ParkingLot.cpp ParkingLotPrinter.cpp ParkingSpot.cpp Time.cpp Vehicle.cpp)