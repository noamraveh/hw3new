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

    //findVehicleInBlock
    Vehicle* ParkingLot::findVehicleInBlock(
            const UniqueArray<Vehicle, VehicleCompare> parking_block,
            const LicensePlate licensePlate) {
        Time fictive_time(0,0,0);
        Vehicle fictive_vehicle(CAR,licensePlate,fictive_time);
        return parking_block[fictive_vehicle];
    }

    //findVehicleInLot
    Vehicle* ParkingLot::findVehicleInLot(const LicensePlate licensePlate ) const{
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
    }

    //enter_parking
    ParkingResult ParkingLot::enterParking(VehicleType vehicleType,
                                           LicensePlate licensePlate,
                                           Time entranceTime) {
        Vehicle vehicle(vehicleType, licensePlate, entranceTime);
        //parked somewhere in lot
        if (findVehicleInLot(vehicle.getLicensePlate())) {
            ParkingLotPrinter::printEntryFailureAlreadyParked(std::cout,
                                                              vehicle.getParkingSpot());
            return VEHICLE_ALREADY_PARKED;
        }
        //full/insert
        switch (vehicleType) {
            case MOTORBIKE: {
                return ParkingLot::entryHelper(motorbike_parking, vehicle, MOTORBIKE);
            }
            case CAR: {
                return ParkingLot::entryHelper(car_parking, vehicle, CAR);
            }
            case HANDICAPPED: {
                ParkingResult handicapped_result = ParkingLot::entryHelper(
                        handicapped_parking, vehicle, HANDICAPPED);
                if (handicapped_result == NO_EMPTY_SPOT) {
                    return ParkingLot::entryHelper(car_parking, vehicle, CAR);
                }
                return handicapped_result;
            }
            default:
                break;
        }
        // shouldn't reach here
        return SUCCESS;
    }

    // entryHelper
    ParkingResult ParkingLot::entryHelper(
            UniqueArray<Vehicle, std::equal_to> parking_block,
            Vehicle vehicle, const VehicleType parking_block_type) {

        //current block is full
        if (parking_block.getCount() == parking_block.getSize()) {
            if (!(parking_block == handicapped_parking)) {
                ParkingLotPrinter::printEntryFailureNoSpot(std::cout);
            }
            return NO_EMPTY_SPOT;
        }
        //block is not full
        int index = parking_block.insert(vehicle);
        vehicle.UpdateParkingSpot(parking_block_type, index);
        ParkingLotPrinter::printVehicle(std::cout, vehicle.getVehicleType(),
                                        vehicle.getLicensePlate(),
                                        vehicle.getEntranceTime());
        ParkingLotPrinter::printEntrySuccess(std::cout,
                                             vehicle.getParkingSpot());
        return SUCCESS;
    }


*/
}