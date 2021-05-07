#include <iostream>
#include <stdexcept>

#include "../Homework2/Person.h"
#include "../Homework2/Vehicle.h"
#include "../Homework2/VehicleList.h"
//#include "Person.h"
//#include "Vehicle.h"
//#include "VehicleList.h"

using TestFunction = void(*)();

bool hasError = false;

bool runTest(TestFunction pfn, const char* name) {
	std::cout << "Running test \"" << name << "\"... ";

	hasError = false;
	pfn();

	std::cout << (hasError ? " ERROR\n" : " OK\n");

	return hasError;
}

int main()
{

}

#define TEST_CASE(name) \
void name(); \
bool testvar_##name = runTest(name, #name); \
void name()

//--------------------------------------------------
//test functions

TEST_CASE(testPersonGetNameAndID) {
	Person p("Name", 42);
	if (p.getID() != 42 || p.getName() != "Name") {
		hasError = true;
	}
}

TEST_CASE(testPersonAcquireVehicle) {
	try {
		Person p("Peter", 42);
		p.acquireVehicle(new Vehicle("TX1111XT", "Audi A3"));

		if (p.getVehicles().size() != 1) {
			hasError = true;
		}

		if (p.getVehicles()[0]->getRegistration() != "TX1111XT" ||
			p.getVehicles()[0]->getDescription() != "Audi A3") {
			hasError = true;
		}

	}
	catch (const std::exception& e) {
		std::cout << e.what();
	}
}

TEST_CASE(testPersonReleaseVehicle) {
	try {
		Person p("Peter", 42);

		Vehicle* v = new Vehicle("TX1111XT", "Audi A3");
		p.acquireVehicle(v);
		p.acquireVehicle(new Vehicle("TX2222XT", "Audi A4"));
		p.acquireVehicle(new Vehicle("TX4444XT", "Audi A6"));

		if (p.getVehicles().size() != 3) {
			hasError = true;
		}

		p.releaseVehicle(v);

		if (p.getVehicles()[0]->getRegistration() != "TX4444XT" || p.getVehicles().size() != 2) {
			hasError = true;
		}

	}
	catch (const std::exception& e) {
		std::cout << e.what();
	}
}