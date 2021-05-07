#include <stdexcept>
#include <cstddef>
#include <cassert>

#include "VehicleList.h"
#include "PersonList.h"

VehicleList::~VehicleList() {
	for (std::size_t i = 0; i < vehicles.size(); i++) {
		delete vehicles[i];
	}
}

void VehicleList::checkRegistrationExists(std::string registration) const {
	for (std::size_t i = 0; i < vehicles.size(); i++) {
		if (vehicles[i]->getRegistration() == registration) {
			throw std::invalid_argument("Registration already exists...");
		}
	}
}

const Vehicle* VehicleList::operator[](std::size_t index) const {
	assert(index >= 0 && index < vehicles.size());

	return vehicles[index];
}

void VehicleList::insert(std::string registration, std::string description) {
	checkRegistrationExists(registration);

	vehicles.push_back(new Vehicle(registration, description));
}

void VehicleList::remove(std::string registration) {
	for (std::size_t i = 0; i < vehicles.size(); i++) {
		if (vehicles[i]->getRegistration() == registration) {
			delete vehicles[i];
			vehicles[i] = vehicles[vehicles.size() - 1];
			vehicles.pop_back();
			break;
		}
	}
}
void VehicleList::remove(Vehicle* vehicle) {
	for (std::size_t i = 0; i < vehicles.size(); i++) {
		if (vehicles[i] == vehicle) {
			delete vehicles[i];
			vehicles[i] = vehicles[vehicles.size() - 1];
			vehicles.pop_back();
			break;
		}
	}
}

Vehicle* VehicleList::getVehicle(std::string registration) const {
	for (std::size_t i = 0; i < vehicles.size(); i++) {
		if (vehicles[i]->getRegistration() == registration) {
			return vehicles[i];
		}
	}
	return nullptr;
}

const std::vector<Vehicle*>& VehicleList::getVehicles() const {
	return vehicles;
}

std::size_t VehicleList::size() const {
	return vehicles.size();
}
