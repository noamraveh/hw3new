#include "ParkingLot.h"
#include "Vehicle.h"
#include "ParkingLotPrinter.h"

namespace MtmParkingLot {

    //c'tor
    ParkingLot::ParkingLot(unsigned int *parkingBlockSizes) :
            motorbike_parking(parkingBlockSizes[0]),
            handicapped_parking(parkingBlockSizes[1]),
            car_parking(parkingBlockSizes[2]) {
    }

    //enterParking
    ParkingResult ParkingLot::enterParking(VehicleType vehicleType,
                                           LicensePlate licensePlate,
                                           Time entranceTime) {
        Vehicle vehicle(vehicleType, licensePlate, entranceTime);
        //parked somewhere in lot
        ParkingResult result;
        if (findVehicleInLot(vehicle.getLicensePlate())!= NULL) {
            ParkingLotPrinter::printEntryFailureAlreadyParked(std::cout,
                                                              vehicle.getParkingSpot());
            return VEHICLE_ALREADY_PARKED;
        }
        //full/insert
        switch (vehicleType) {
            case MOTORBIKE: {
                result = ParkingLot::entryHelper(motorbike_parking, vehicle, MOTORBIKE);
                break;
            }
            case CAR: {
                result = ParkingLot::entryHelper(car_parking, vehicle, CAR);
                break;
            }
            case HANDICAPPED: {
                ParkingResult handicapped_result = ParkingLot::entryHelper(
                        handicapped_parking, vehicle, HANDICAPPED);
                if (handicapped_result == NO_EMPTY_SPOT) {
                    result = ParkingLot::entryHelper(car_parking, vehicle, CAR);
                    break;
                }
                result = handicapped_result;
                break;
            }
            default:
                result = SUCCESS;
                break;
        }
        return result;
    }
    //exitParking
    ParkingResult ParkingLot::exitParking(LicensePlate licensePlate,
                                          Time exitTime) {
        Vehicle* wanted_vehicle = findVehicleInLot(licensePlate);
        if (wanted_vehicle == NULL){
            ParkingLotPrinter::printExitFailure(std::cout,licensePlate);
            return VEHICLE_NOT_FOUND;
        }
        ParkingLotPrinter::printVehicle(std::cout, wanted_vehicle->getVehicleType(),wanted_vehicle->getLicensePlate(),wanted_vehicle->getEntranceTime());
        UniqueArray wanted_vehicle_block = vehicleTypetoUniqueArray(wanted_vehicle->getParkingSpot().getParkingBlock());
        int price = Vehicle::calculatingPrice(wanted_vehicle->getVehicleType(),wanted_vehicle->getEntranceTime(),exitTime);
        wanted_vehicle_block.remove(*wanted_vehicle);
        ParkingLotPrinter::printExitSuccess(std::cout, wanted_vehicle->getParkingSpot(),exitTime,price);
        return SUCCESS;
    }
    //entryHelper
    ParkingResult ParkingLot::entryHelper(
            UniqueArray<Vehicle, std::equal_to> parking_block,
            Vehicle vehicle, const VehicleType parking_block_type) {

        try {
            int index = parking_block.insert(vehicle);
        } catch (UniqueArray::UniqueArrayIsFullException);
        return NO_EMPTY_SPOT;
        }
        vehicle.updateParkingSpot(parking_block_type, index);
        ParkingLotPrinter::printVehicle(std::cout, vehicle.getVehicleType(),
                                        vehicle.getLicensePlate(),
                                        vehicle.getEntranceTime());
        ParkingLotPrinter::printEntrySuccess(std::cout,
                                             vehicle.getParkingSpot());
        return SUCCESS;
    }

    //findVehicleInBlock
    Vehicle* ParkingLot::findVehicleInBlock(
            const UniqueArray<Vehicle, VehicleCompare> parking_block,
            const LicensePlate licensePlate) {
        Time fictive_time(0,0,0);
        Vehicle fictive_vehicle(CAR,licensePlate,fictive_time);
        return parking_block[fictive_vehicle];
    }

    //findVehicleInLot
    Vehicle* ParkingLot::findVehicleInLot(const LicensePlate license_plate ) const{
        Vehicle* found_vehicle = ParkingLot::findVehicleInBlock(motorbike_parking,licensePlate);
        if (found_vehicle != NULL){
            return found_vehicle;
        }
        found_vehicle = ParkingLot::findVehicleInBlock(car_parking,licensePlate);
        if (found_vehicle != NULL) {
            return found_vehicle;
        }
        found_vehicle = ParkingLot::findVehicleInBlock(handicapped_parking,licensePlate);
        return found_vehicle;
    }

    //vehicleTypetoUniqueArray
    UniqueArray<Vehicle,VehicleCompare> ParkingLot::vehicleTypetoUniqueArray(
            VehicleType vehicle_block_type) const{
        if (vehicle_block_type == MOTORBIKE) {
            return motorbike_parking;
        }
        if (vehicle_block_type == CAR){
            return car_parking;
        }
        return handicapped_parking;
    }
}

