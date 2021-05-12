#pragma once

#include <string>
#include "Registration.h"
#include "Person.h"

class Vehicle {
	Registration registration;
	std::string description;
	Person* owner = nullptr;

public:
	Vehicle(std::string reg, std::string desc);

	void setOwner(Person* p);
	void removeOwner();

	const std::string& getRegistration() const;
	const std::string& getDescription() const;
	Person* getOwner() const;
};