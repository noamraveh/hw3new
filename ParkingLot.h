#ifndef MTMPARKINGLOT_PARKINGLOT_H
#define MTMPARKINGLOT_PARKINGLOT_H

#include "ParkingLotTypes.h"
#include "Time.h"
#include "ParkingSpot.h"
#include "Vehicle.h"
#include "UniqueArray.h"
#include "ParkingLotPrinter.h"

namespace MtmParkingLot {

    using namespace ParkingLotUtils;
    using std::ostream;

    class ParkingLot {
        UniqueArray<Vehicle, VehicleCompare> motorbike_parking;
        UniqueArray<Vehicle, VehicleCompare> handicapped_parking;
        UniqueArray<Vehicle, VehicleCompare> car_parking;
    public:

        ParkingLot(unsigned int parkingBlockSizes[]);

        ~ParkingLot();

        ParkingResult
        enterParking(VehicleType vehicleType, LicensePlate licensePlate,
                     Time entranceTime);

        ParkingResult exitParking(LicensePlate licensePlate, Time exitTime);

        ParkingResult getParkingSpot(LicensePlate licensePlate,
                                     ParkingSpot &parkingSpot) const;

        void inspectParkingLot(Time inspectionTime);

        friend ostream &operator<<(ostream &os, const ParkingLot &parkingLot);

        const Vehicle *
        findVehicleInLot(const LicensePlate &license_plate) const;

        const Vehicle *findVehicleInBlock(
                const UniqueArray<Vehicle, VehicleCompare> &parking_block,
                const LicensePlate &licensePlate) const;

        UniqueArray<Vehicle, VehicleCompare> *
        vehicleTypetoUniqueArray(VehicleType vehicle_block_type);

        bool compareSpot(Vehicle *vehicle1, Vehicle *vehicle2);

        ParkingResult entryHelper(
                UniqueArray<Vehicle, VehicleCompare> &parking_block,
                Vehicle vehicle, const VehicleType parking_block_type);

    };

}


#endif //MTMPARKINGLOT_PARKINGLOT_H
