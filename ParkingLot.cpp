#include "ParkingLot.h"


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
        if (findVehicleInLot(vehicle.getLicensePlate()) != NULL) {
            ParkingLotPrinter::printEntryFailureAlreadyParked(std::cout,
                                                              vehicle.getParkingSpot());
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
    ParkingResult ParkingLot::exitParking(LicensePlate licensePlate,
                                          Time exitTime) {
        Vehicle *wanted_vehicle = findVehicleInLot(licensePlate);
        if (wanted_vehicle == NULL) {
            ParkingLotPrinter::printExitFailure(std::cout, licensePlate);
            return VEHICLE_NOT_FOUND;
        }
        ParkingLotPrinter::printVehicle(std::cout,
                                        wanted_vehicle->getVehicleType(),
                                        wanted_vehicle->getLicensePlate(),
                                        wanted_vehicle->getEntranceTime());
        UniqueArray<Vehicle, VehicleCompare> wanted_vehicle_block = vehicleTypetoUniqueArray(
                wanted_vehicle->getParkingSpot().getParkingBlock());
        int price = Vehicle::calculatingPrice(wanted_vehicle->getVehicleType(),
                                              wanted_vehicle->getEntranceTime(),
                                              exitTime);
        wanted_vehicle_block.remove(*wanted_vehicle);
        ParkingLotPrinter::printExitSuccess(std::cout,
                                            wanted_vehicle->getParkingSpot(),
                                            exitTime, price);
        return SUCCESS;
    }

    //get parking spot
    ParkingResult ParkingLot::getParkingSpot(LicensePlate licensePlate,
                                             ParkingSpot &parkingSpot) const {
        Vehicle *found_vehicle = ParkingLot::findVehicleInLot(licensePlate);
        if (found_vehicle == NULL) {
            return VEHICLE_NOT_FOUND;
        }
        parkingSpot = found_vehicle->getParkingSpot();
        return SUCCESS;

    }

    //print lot
    ostream &ParkingLot::operator<<(ostream &os,
                                    const MtmParkingLot::ParkingLot &parkingLot) {
        ParkingLotPrinter::printParkingLotTitle(os)
        std::vector<Vehicle* > all_vehicles;

        Vehicle **all_motorbikes = motorbike_parking.getData();
        for (int i = 0; i < motorbike_parking.getSize(); i++) {
            all_vehicles.push_back(all_motorbikes[i]);
        }

        Vehicle **all_handicapped = handicapped_parking.getData();
        for (int i = 0; i < handicapped_parking.getSize(); i++) {
            all_vehicles.push_back(all_handicapped[i]);
        }
        Vehicle **all_cars = car_parking.getData();
        for (int i = 0; i < car_parking.getSize(); i++) {
            all_vehicles.push_back(all_cars[i]);
        }
        typedef std::vector<Vehicle *>::iterator Iterator;

        for (Iterator = all_vehicles.begin();
             Iterator != all_vehicles.end(), Iterator++) {
            Vehicle current_vehicle = *Iterator;
            ParkingLotPrinter::printVehicle(os,
                                            current_vehicle.getVehicleType(),
                                            current_vehicle.getLicensePlate(),
                                            current_vehicle.getEntranceTime());
            ParkingLotPrinter::printParkingSpot(
                    current_vehicle.getParkingSpot());
        }
    }

    void ParkingLot::inspectParkingLot(Time inspectionTime) {
        unsigned int num_fined = 0;
        Vehicle **all_motorbikes = motorbike_parking.getData();
        for (int i = 0; i < motorbike_parking.getSize(); i++) {
            if (((inspectionTime.operator-(all_motorbikes[i]->getEntranceTime())).toHours()>24) && all_motorbikes[i].getFined() == false){
                all_motorbikes[i]->updateFined();
                num_fined++;
            }
        }
        Vehicle **all_handicapped = handicapped_parking.getData();
        for (int i = 0; i < handicapped_parking.getSize(); i++) {
            if (((inspectionTime.operator-(all_handicapped[i]->getEntranceTime())).toHours()>24) && all_handicapped[i].getFined() == false){
                all_handicapped[i]->updateFined();
                num_fined++;
            }
        }
        Vehicle **all_cars = car_parking.getData();
        for (int i = 0; i < car_parking.getSize(); i++) {
            if (((inspectionTime.operator-(all_cars[i]->getEntranceTime())).toHours()>24) && all_cars[i].getFined() == false){
                all_cars[i]->updateFined();
                num_fined++;
            }
        }
        ParkingLotPrinter::printInspectionResult(std::cout,inspectionTime,num_fined);
    }

    //inspectorPerBlock
    void ParkingLot::inspectorPerBlock(UniqueArray<Vehicle,VehicleCompare> parking_block, Time inspectionTime, unsigned int& num_fined){

    }


    //entryHelper
    ParkingResult ParkingLot::entryHelper(
            UniqueArray<Vehicle, std::equal_to> parking_block,
            Vehicle vehicle, const VehicleType parking_block_type) {

        try {
            int index = parking_block.insert(vehicle);
        } catch (UniqueArray::UniqueArrayIsFullException) {
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
    Vehicle *ParkingLot::findVehicleInBlock(
            const UniqueArray<Vehicle, VehicleCompare> parking_block,
            const LicensePlate licensePlate) {
        Time fictive_time(0, 0, 0);
        Vehicle fictive_vehicle(CAR, licensePlate, fictive_time);
        return parking_block[fictive_vehicle];
    }

    //findVehicleInLot
    Vehicle *
    ParkingLot::findVehicleInLot(const LicensePlate license_plate) const {
        Vehicle *found_vehicle = ParkingLot::findVehicleInBlock(
                motorbike_parking, licensePlate);
        if (found_vehicle != NULL) {
            return found_vehicle;
        }
        found_vehicle = ParkingLot::findVehicleInBlock(car_parking,
                                                       licensePlate);
        if (found_vehicle != NULL) {
            return found_vehicle;
        }
        found_vehicle = ParkingLot::findVehicleInBlock(handicapped_parking,
                                                       licensePlate);
        return found_vehicle;
    }

    //vehicleTypetoUniqueArray
    UniqueArray<Vehicle, VehicleCompare> ParkingLot::vehicleTypetoUniqueArray(
            VehicleType vehicle_block_type) const {
        if (vehicle_block_type == MOTORBIKE) {
            return motorbike_parking;
        }
        if (vehicle_block_type == CAR) {
            return car_parking;
        }
        return handicapped_parking;
    }

}

