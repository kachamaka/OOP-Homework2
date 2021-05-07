#pragma once

#include <string>
#include <vector>

//declaring class Vehicle to avoid recursive definition by including

class Vehicle;

class Person {
	unsigned int ID;
	std::string name;
	std::vector<Vehicle*> vehicles;

public:
	Person(std::string name, unsigned int id);

	void acquireVehicle(Vehicle* vehicle);
	void releaseVehicle(Vehicle* vehicle);

	const std::string& getName() const;
	const unsigned int& getID() const;
	const std::vector<Vehicle*>& getVehicles() const;
};