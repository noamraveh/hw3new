//
// Created by Noam Raveh on 12/01/2020.
//

#ifndef HW3_VEHICLE_H
#define HW3_VEHICLE_H

#include "ParkingLotTypes.h"
#include "Time.h"
#include "ParkingSpot.h"

using namespace ParkingLotUtils;
namespace MtmParkingLot {

    static const int MOTORBIKE_PRICE_FIRST = 10 ;
    static const int MOTORBIKE_PRICE_FOLLOWING = 5 ;
    static const int CAR_PRICE_FIRST = 20 ;
    static const int CAR_PRICE_FOLLOWING = 10 ;
    static const int HANDICAPPED_PRICE = 15 ;
    static const int FINE = 250;
    static const int MAX_HOURS_TO_PAY = 6;

    class Vehicle {
        VehicleType vehicle_type;
        ParkingSpot parking_spot;
        Time entrance_time;
        LicensePlate license_plate;
        bool fined;

    public:
        /**
            * @brief Construct a new Vehicle object. Add entrance t

            * @param license_plate The license plate of the vehicle (represented by LicensePlate enum)
            * @param vehicle_type The type of the vehicle (represented by VehicleType enum)
            * @param entrance_time the time of entrance (represented by Time class element)
            */
        Vehicle(VehicleType vehicle_type, LicensePlate license_plate,
                Time entrance_time);

        ~Vehicle()=default;

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
         * @brief Get the fine status of  this vehicle
         *
         * @return fined true/false (bool)
         */
        bool getFined() const;

        /**
         * @brief Update parking spot of existing vehicle
         *
         * @param new_parking_block updated block (represented by VehicleType enum)
         * @param new_parking_spot updated spot (int)
         */
        void updateParkingSpot(VehicleType new_parking_block,
                               unsigned int new_parking_spot);

        /**
         * @brief Calculating the price for parking for all vehicles
         *
         * @param VehicleType type of vehicle (VehicleType enum)
         * @param entry_time time of entrance (Time class element)
         * @param fined true/false whether vehicle is fined or not (bool)
         *
         * @return the price needed to pay
         */
        static int calculatingPrice(VehicleType vehicleType, Time entry_time,
                                    Time exit_time, bool fined);


        /**
        * @brief Calculating the price for given vehicleType
        *
        * @param hours total hours parked
        * @param first_hour_price,next_hours_price - prices for the specific vehicle
        * @param fined true/false whether vehicle is fined or not (bool)
        * @return the price needed to pay for the given type
        */
        static int price_per_vehicle_type(int hours, int first_hour_price, int next_hours_price,
                                          bool fined);

        /**
         * update the find status from false to true of given vehicle
         *
         */

        void updateFined();

        /**
        * @brief Compares given ParkingSpot objects
        *
        * @param vehicle1
        * @param vehicle2
        * @return true If vehicle1 < vehicle2
        * @return false otherwise
        */
        friend bool operator<(const Vehicle &vehicle1, const Vehicle &vehicle2);

    };

    class VehicleCompare {
    public:
        VehicleCompare() = default;

        bool
        operator()(const Vehicle &vehicle1, const Vehicle &vehicle2) const {
            return vehicle1.getLicensePlate() == vehicle2.getLicensePlate();
        };
    };
}

#endif //HW3_VEHICLE_H
