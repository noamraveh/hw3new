#include <vector>
#include <algorithm>
#include "ParkingLot.h"

using std::vector;
using namespace MtmParkingLot;
using namespace ParkingLotUtils;
namespace MtmParkingLot {

    //c'tor
    ParkingLot::ParkingLot(unsigned int *parkingBlockSizes) :
            motorbike_parking(parkingBlockSizes[0]),
            handicapped_parking(parkingBlockSizes[1]),
            car_parking(parkingBlockSizes[2]) {
    }

    ParkingLot::~ParkingLot() = default;

    //enterParking
    ParkingResult ParkingLot::enterParking(VehicleType vehicleType,
                                           LicensePlate licensePlate,
                                           Time entranceTime) {
        Vehicle vehicle(vehicleType, licensePlate, entranceTime);


        //parked somewhere in lot
        ParkingResult result;
        const Vehicle *found_vehicle = findVehicleInLot(
                vehicle.getLicensePlate());
        if (found_vehicle != nullptr) {
            ParkingLotPrinter::printVehicle(std::cout,
                                            found_vehicle->getVehicleType(),
                                            found_vehicle->getLicensePlate(),
                                            found_vehicle->getEntranceTime());
            ParkingLotPrinter::printEntryFailureAlreadyParked(std::cout,
                                                              found_vehicle->getParkingSpot());
            return VEHICLE_ALREADY_PARKED;
        }
        //full/insert
        switch (vehicleType) {
            case MOTORBIKE: {
                result = ParkingLot::entryHelper(motorbike_parking, vehicle,
                                                 MOTORBIKE);
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
    ParkingResult ParkingLot::exitParking(const LicensePlate& licensePlate,
                                          Time exitTime) {
        const Vehicle *wanted_vehicle = findVehicleInLot(licensePlate);

        if (wanted_vehicle == nullptr) {
            ParkingLotPrinter::printExitFailure(std::cout, licensePlate);
            return VEHICLE_NOT_FOUND;
        }
        ParkingLotPrinter::printVehicle(std::cout,
                                        wanted_vehicle->getVehicleType(),
                                        wanted_vehicle->getLicensePlate(),
                                        wanted_vehicle->getEntranceTime());
        UniqueArray<Vehicle, VehicleCompare> *wanted_vehicle_block = vehicleTypetoUniqueArray(
                wanted_vehicle->getParkingSpot().getParkingBlock());
        int price = Vehicle::calculatingPrice(wanted_vehicle->getVehicleType(),
                                              wanted_vehicle->getEntranceTime(),
                                              exitTime,
                                              wanted_vehicle->getFined());
        ParkingLotPrinter::printExitSuccess(std::cout,
                                            wanted_vehicle->getParkingSpot(),
                                            exitTime, price);
        wanted_vehicle_block->remove(*wanted_vehicle);

        return SUCCESS;
    }

    //get parking spot
    ParkingResult ParkingLot::getParkingSpot(const LicensePlate& licensePlate,
                                             ParkingSpot &parkingSpot) const {
        const Vehicle *found_vehicle = ParkingLot::findVehicleInLot(
                licensePlate);
        if (found_vehicle == nullptr) {
            return VEHICLE_NOT_FOUND;
        }
        parkingSpot = found_vehicle->getParkingSpot();
        return SUCCESS;

    }

    //print lot
    ostream &operator<<(ostream &os, const ParkingLot &parkingLot) {
        ParkingLotPrinter::printParkingLotTitle(os);
        std::vector<Vehicle> all_vehicles;
        for (int i = 0; i < parkingLot.motorbike_parking.getSize(); i++) {
            if (parkingLot.motorbike_parking.getElementbyIndex(i) != nullptr) {
                Vehicle current_vehicle = *(parkingLot.motorbike_parking.getElementbyIndex(
                        i));
                all_vehicles.push_back(current_vehicle);
            }
        }

        for (int i = 0; i < parkingLot.handicapped_parking.getSize(); i++) {
            if (parkingLot.handicapped_parking.getElementbyIndex(i) != nullptr) {
                Vehicle current_vehicle = *(parkingLot.handicapped_parking.getElementbyIndex(
                        i));
                all_vehicles.push_back(current_vehicle);
            }
        }

        for (int i = 0; i < parkingLot.car_parking.getSize(); i++) {
            if (parkingLot.car_parking.getElementbyIndex(i) != nullptr) {
                Vehicle current_vehicle = *(parkingLot.car_parking.getElementbyIndex(
                        i));
                all_vehicles.push_back(current_vehicle);
            }

        }
        std::sort(all_vehicles.begin(),all_vehicles.end());
        for (auto i = all_vehicles.begin(); i != all_vehicles.end(); i++) {
            Vehicle current_vehicle = *i;
            ParkingLotPrinter::printVehicle(os,
                                            current_vehicle.getVehicleType(),
                                            current_vehicle.getLicensePlate(),
                                            current_vehicle.getEntranceTime());
            ParkingLotPrinter::printParkingSpot(os,
                                                current_vehicle.getParkingSpot());
        }
        return os;
    }

    void ParkingLot::inspectParkingLot(Time inspectionTime) {
        unsigned int num_fined = 0;

        for (int i = 0; i < motorbike_parking.getSize(); i++) {
            Vehicle *current_vehicle = motorbike_parking.getElementbyIndex(i);
            if (current_vehicle == nullptr) {
                continue;
            }
            if ((inspectionTime.operator-(
                    current_vehicle->getEntranceTime())).toHours() > 24 &&
                !current_vehicle->getFined()) {
                num_fined++;
                current_vehicle->updateFined();
            }
        }

        for (int i = 0; i < handicapped_parking.getSize(); i++) {
            Vehicle *current_vehicle = handicapped_parking.getElementbyIndex(i);
            if (current_vehicle == nullptr) {
                continue;
            }
            if ((inspectionTime.operator-(
                    current_vehicle->getEntranceTime())).toHours() > 24 &&
                !current_vehicle->getFined()) {
                num_fined++;
                current_vehicle->updateFined();
            }
        }

        for (int i = 0; i < car_parking.getSize(); i++) {
            Vehicle *current_vehicle = car_parking.getElementbyIndex(i);
            if (current_vehicle == nullptr) {
                continue;
            }
            if ((inspectionTime.operator-(
                    current_vehicle->getEntranceTime())).toHours() > 24 &&
                !current_vehicle->getFined()) {
                num_fined++;
                current_vehicle->updateFined();
            }
        }


        ParkingLotPrinter::printInspectionResult(std::cout, inspectionTime,
                                                 num_fined);
    }

    //entryHelper
    ParkingResult ParkingLot::entryHelper(
            UniqueArray<Vehicle, VehicleCompare> &parking_block,
            const Vehicle& vehicle, const VehicleType parking_block_type) {
        unsigned int index;
        try {
            index = parking_block.insert(vehicle);
        } catch (
                UniqueArray<Vehicle, VehicleCompare>::UniqueArrayIsFullException) {
            if (parking_block_type != HANDICAPPED) {
                ParkingLotPrinter::printVehicle(std::cout,
                                                vehicle.getVehicleType(),
                                                vehicle.getLicensePlate(),
                                                vehicle.getEntranceTime());
                ParkingLotPrinter::printEntryFailureNoSpot(std::cout);
            }
            return NO_EMPTY_SPOT;
        }
        parking_block.getElementbyIndex(index)->updateParkingSpot(
                parking_block_type, index);
        ParkingLotPrinter::printVehicle(std::cout, vehicle.getVehicleType(),
                                        vehicle.getLicensePlate(),
                                        vehicle.getEntranceTime());
        ParkingLotPrinter::printEntrySuccess(std::cout,
                                             parking_block.getElementbyIndex(
                                                     index)->getParkingSpot());
        return SUCCESS;
    }

    //findVehicleInBlock
    const Vehicle *ParkingLot::findVehicleInBlock(
            const UniqueArray<Vehicle, VehicleCompare> &parking_block,
            const LicensePlate &licensePlate) const {
        Time fictive_time(0, 0, 0);
        Vehicle fictive_vehicle(CAR, licensePlate, fictive_time);
        return parking_block[fictive_vehicle];
    }

    //findVehicleInLot
    const Vehicle *
    ParkingLot::findVehicleInLot(const LicensePlate &license_plate) const {
        const Vehicle *found_vehicle = ParkingLot::findVehicleInBlock(
                motorbike_parking, license_plate);
        if (found_vehicle != nullptr) {
            return found_vehicle;
        }
        found_vehicle = ParkingLot::findVehicleInBlock(car_parking,
                                                       license_plate);
        if (found_vehicle != nullptr) {
            return found_vehicle;
        }
        found_vehicle = ParkingLot::findVehicleInBlock(handicapped_parking,
                                                       license_plate);

        return found_vehicle;
    }

    //vehicleTypetoUniqueArray
    UniqueArray<Vehicle, VehicleCompare> *ParkingLot::vehicleTypetoUniqueArray(
            VehicleType vehicle_block_type) {
        if (vehicle_block_type == MOTORBIKE) {
            return &motorbike_parking;
        }
        if (vehicle_block_type == CAR) {
            return &car_parking;
        }
        return &handicapped_parking;
    }

}

