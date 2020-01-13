//
// Created by Noam Raveh on 12/01/2020.
//

#ifndef HW3_VEHICLE_H
#define HW3_VEHICLE_H

#include "ParkingLotTypes.h"
#include "Time.h"
#include "ParkingSpot.h"

using namespace ParkingLotUtils;

class Vehicle {
    VehicleType vehicle_type;
    ParkingSpot parking_spot;
    Time entrance_time;
    LicensePlate license_plate;

public:
    /**
        * @brief Construct a new Vehicle object. Add entrance t
        *
        * @param license_plate The license plate of the vehicle (represented by LicensePlate enum)
        * @param vehicle_type The type of the vehicle (represented by VehicleType enum)
        */
        Vehicle(VehicleType vehicle_type, LicensePlate license_plate, Time entrance_time);

        /**
         * @brief Get the vehicle type of this vehicle
         *
         * @return vehicle_type of the vehicle (represented by enum vehicleType)
         */
        VehicleType getVehicleType() const;
        /**
         * @brief Get the parking spot of this ParkingSpot
         *
         * @return parking_spot The Parking spot (represented by ParkingSpot class)
         */
        ParkingSpot getParkingSpot() const;

        /**
         * @brief Get the entrance time of this vehicle
         *
         * @return entrance_time of the vehicle (represented by Time class)
         */
        Time getEntranceTime() const;

        /**
         * @brief Get the license plate of this vehicle
         *
         * @return license_plate of the vehicle (represented by typedef LicenseString)
         */
        LicensePlate getLicensePlate() const;

        /**
         * @brief Update parking spot of existing vehicle
         *
         * @return
         */
         void UpdateParkingSpot(VehicleType new_parking_block,unsigned int new_parking_spot);


};


class VehicleCompare{
public:
    VehicleCompare() = default;
    bool operator()(const Vehicle& vehicle1, const Vehicle& vehicle2) const{
        return vehicle1.getLicensePlate() == vehicle2.getLicensePlate();
    };
};

Vehicle::Vehicle(VehicleType vehicle_type, LicensePlate license_plate,
                 Time entrance_time):
                 vehicle_type(vehicle_type),
                 license_plate(license_plate),
                 entrance_time(entrance_time) {
}
VehicleType Vehicle::getVehicleType() const {
    return vehicle_type;
}

Time Vehicle::getEntranceTime() const {
    return entrance_time;
}
LicensePlate Vehicle::getLicensePlate() const {
    return license_plate;
}
ParkingSpot Vehicle::getParkingSpot() const {
    return parking_spot;
}

void Vehicle::UpdateParkingSpot( VehicleType new_parking_block,
                                unsigned int new_parking_spot) {
    ParkingSpot temp(new_parking_block,new_parking_spot);
    parking_spot = temp;
}



#endif //HW3_VEHICLE_H
