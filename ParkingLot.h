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

        explicit ParkingLot(unsigned int parkingBlockSizes[]);

        ~ParkingLot() = default;

        ParkingResult
        enterParking(VehicleType vehicleType, LicensePlate licensePlate,
                     Time entranceTime);

        ParkingResult exitParking(const LicensePlate& licensePlate, Time exitTime);

        ParkingResult getParkingSpot(const LicensePlate& licensePlate,
                                     ParkingSpot &parkingSpot) const;

        void inspectParkingLot(Time inspectionTime);

        friend ostream &operator<<(ostream &os, const ParkingLot &parkingLot);

        // search for specific vehicle by license plate in entire parking lot
        const Vehicle *
        findVehicleInLot(const LicensePlate &license_plate) const;

        // search for vehicle by license plate in given parking block
        const Vehicle *findVehicleInBlock(
                const UniqueArray<Vehicle, VehicleCompare> &parking_block,
                const LicensePlate &licensePlate) const;

        // return unique array pointer by a given vehicleType (block type)
        UniqueArray<Vehicle, VehicleCompare> *
        vehicleTypetoUniqueArray(VehicleType vehicle_block_type);

        // insert new vehicle into given parking block
        ParkingResult entryHelper(
                UniqueArray<Vehicle, VehicleCompare> &parking_block,
                const Vehicle& vehicle, const VehicleType parking_block_type);

    };

}


#endif //MTMPARKINGLOT_PARKINGLOT_H
