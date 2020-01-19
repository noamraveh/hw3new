cmake_minimum_required(VERSION 3.15)
project(hw3)

set(CMAKE_CXX_STANDARD 11)

add_executable(hw3 ParkingLot.cpp ParkingLotPrinter.cpp ParkingSpot.cpp Vehicle.cpp Time.cpp MtmParkingLot.cpp)