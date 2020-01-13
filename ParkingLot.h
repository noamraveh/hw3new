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
        //change to compare
        UniqueArray<Vehicle,std::equal_to> motorbike_parking;
        UniqueArray<Vehicle,std::equal_to> handicapped_parking;
        UniqueArray<Vehicle,std::equal_to> car_parking;

    public:

        ParkingLot(unsigned int parkingBlockSizes[]);
        ~ParkingLot();
        ParkingResult enterParking(VehicleType vehicleType, LicensePlate licensePlate, Time entranceTime);
        ParkingResult exitParking(LicensePlate licensePlate, Time exitTime);
        ParkingResult getParkingSpot(LicensePlate licensePlate, ParkingSpot& parkingSpot) const;
        void inspectParkingLot(Time inspectionTime);
        friend ostream& operator<<(ostream& os, const ParkingLot& parkingLot);
        Vehicle* findVehicleInLot (const LicensePlate license_plate) const;
        Vehicle* findVehicleInBlock (const UniqueArray<Vehicle,VehicleCompare> parking_block,const LicensePlate licensePlate);
        UniqueArray<Vehicle,VehicleCompare> vehicleTypetoUniqueArray (VehicleType vehicle_block_type)const;



            ParkingResult entryHelper (
                UniqueArray<Vehicle, std::equal_to> parking_block,
                Vehicle vehicle,
                const VehicleType parking_block_type);
    };
}

#endif //MTMPARKINGLOT_PARKINGLOT_H
