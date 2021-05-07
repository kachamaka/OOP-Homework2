#pragma once

#include <vector>
#include <cstddef>
#include "Vehicle.h"

class VehicleList {
	std::vector<Vehicle*> vehicles;

	VehicleList() {};
	~VehicleList();

	void checkRegistrationExists(std::string registration) const;
public:
	static VehicleList& Get() {
		static VehicleList vL_instance;
		return vL_instance;
	}

	VehicleList(const VehicleList&) = delete;
	VehicleList& operator=(const VehicleList&) = delete;

	const Vehicle* operator[](std::size_t index) const;

	void insert(std::string registration, std::string description);

	void remove(std::string registration);
	void remove(Vehicle* vehicle);

	std::size_t size() const;

	Vehicle* getVehicle(std::string registration) const;

	const std::vector<Vehicle*>& getVehicles() const;
};