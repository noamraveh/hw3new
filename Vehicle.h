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
    bool fined;

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
         * @brief Get the license plate of this vehicle
         *
         * @return license_plate of the vehicle (represented by typedef LicenseString)
         */
        bool getFined() const;

        /**
         * @brief Update parking spot of existing vehicle
         *
         * @return
         */
         void updateParkingSpot(VehicleType new_parking_block,unsigned int new_parking_spot);

        /**
         * @brief Calculating the price for parking
         *
         * @return the price needed to pay
         */
        static int calculatingPrice(VehicleType vehicleType, Time entry_time, Time exit_time, const bool fined) ;


         /**
         * @brief Calculating the price for given vehicleType
         *
         * @return the price needed to pay
         */
         static int calc(int hours, int first_hour_price, int next_hours_price, const bool fined);

         void updateFined();

        /**
        * @brief Compares given ParkingSpot objects
        *
        * @param vehicle1
        * @param vehicle2
        * @return true If vehicle1 < vehicle2
        * @return false otherwise
        */
        friend bool operator< (const Vehicle& vehicle1, const Vehicle& vehicle2);

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
                 entrance_time(entrance_time),
                 fined(false){
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
bool Vehicle::getFined() const {
    return fined;
}

void Vehicle::updateParkingSpot( VehicleType new_parking_block,
                                unsigned int new_parking_spot) {
    ParkingSpot temp(new_parking_block,new_parking_spot);
    parking_spot = temp;
}
int Vehicle::calc(int hours,int first_hour_price, int next_hours_price,const bool fined)  {
    if (hours>6){
        return first_hour_price+5*next_hours_price+250*fined;
    }
    return (first_hour_price +(hours-1)*next_hours_price);
}
int Vehicle::calculatingPrice(VehicleType vehicleType, Time entry_time,
                              Time exit_time, const bool fined)  {
    int price = 0;
    Time time_diff = exit_time.operator-(entry_time);
    int total_hours = time_diff.toHours();
    switch (vehicleType){
        case MOTORBIKE:{
            price = calc(total_hours,10,5,fined);
            break;
        }
        case CAR:{
            price = calc(total_hours,20,10,fined);
            break;
        }
        case HANDICAPPED:{
            price = 15 + 250*fined;
            break;
        }
        default:{
            price = 0;
        }
    }
    return price;
}

void Vehicle::updateFined() {
    fined = true;
}

bool operator<(const Vehicle &vehicle1, const Vehicle &vehicle2) {
    return vehicle1.getParkingSpot() < vehicle2.getParkingSpot();
}


#endif //HW3_VEHICLE_H
