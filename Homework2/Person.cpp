#include "Person.h"

Person::Person(std::string name, unsigned int id): name(name), ID(id) { }

void Person::acquireVehicle(Vehicle* vehicle) {
	vehicles.push_back(vehicle);
}

void Person::releaseVehicle(Vehicle* vehicle) {
	for (std::size_t i = 0; i < vehicles.size(); i++) {
		if (vehicles[i] == vehicle) {
			vehicles[i] = vehicles[vehicles.size() - 1];
			vehicles.pop_back();
			break;
		}
	}
}

const std::string& Person::getName() const {
	return name;
}
const unsigned int& Person::getID() const {
	return ID;
}
const std::vector<Vehicle*>& Person::getVehicles() const {
	return vehicles;
}
