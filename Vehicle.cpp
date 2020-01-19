#include "Vehicle.h"

    namespace MtmParkingLot {
        Vehicle::Vehicle(VehicleType vehicle_type, LicensePlate license_plate,
                         Time entrance_time) :
                vehicle_type(vehicle_type),
                entrance_time(entrance_time),
                license_plate(license_plate),
                fined(false) {
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

        void Vehicle::updateParkingSpot(VehicleType new_parking_block,
                                        unsigned int new_parking_spot) {
            ParkingSpot temp(new_parking_block, new_parking_spot);
            parking_spot = temp;
        }

        int Vehicle::price_per_vehicle_type(int hours, int first_hour_price, int next_hours_price,
                                            const bool fined) {
            if (hours > MAX_HOURS_TO_PAY) {
                return first_hour_price + (MAX_HOURS_TO_PAY - 1) * next_hours_price + FINE * fined;
            }
            if (hours == 0) {
                return 0;
            }
            return (first_hour_price + (hours - 1) * next_hours_price);
        }

        int Vehicle::calculatingPrice(VehicleType vehicleType, Time entry_time,
                                      Time exit_time, const bool fined) {
            int price = 0;
            Time time_diff = exit_time.operator-(entry_time);
            int total_hours = time_diff.toHours();
            switch (vehicleType) {
                case MOTORBIKE: {
                    price = price_per_vehicle_type(total_hours,
                                                   MOTORBIKE_PRICE_FIRST,
                                                   MOTORBIKE_PRICE_FOLLOWING,
                                                   fined);
                    break;
                }
                case CAR: {
                    price = price_per_vehicle_type(total_hours, CAR_PRICE_FIRST,
                                                   CAR_PRICE_FOLLOWING, fined);
                    break;
                }
                case HANDICAPPED: {
                    if (total_hours == 0) {
                        return 0;
                    }
                    price = HANDICAPPED_PRICE + FINE * fined;
                    break;
                }
                default: {
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
    }