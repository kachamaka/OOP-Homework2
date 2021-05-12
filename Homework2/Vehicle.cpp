#include "Vehicle.h"

Vehicle::Vehicle(std::string reg, std::string desc): registration(reg), description(desc) { }

void Vehicle::setOwner(Person* p) {
	owner = p;
}

void Vehicle::removeOwner() {
	owner = nullptr;
}

const std::string& Vehicle::getRegistration() const {
	return this->registration.getRegistration();
}

const std::string& Vehicle::getDescription() const {
	return this->description;
}

Person* Vehicle::getOwner() const {
	return owner;
}