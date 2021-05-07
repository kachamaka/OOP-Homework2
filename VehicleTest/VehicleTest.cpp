#include <iostream>
#include <stdexcept>

#include "../Homework2/Vehicle.h"
//#include "Vehicle.h"

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

TEST_CASE(testVehicleConstructor) {
	try {
		Vehicle a("TX1111XH", "Test desc");
	}
	catch (const std::exception& e) {
		std::cout << e.what();
	}
}

TEST_CASE(testVehicleGetMethods) {
	try {
		Vehicle a("TX1111XH", "Audi");
		if (a.getRegistration() != "TX1111XH" || a.getDescription() != "Audi") {
			hasError = true;
		}
	}
	catch (const std::exception& e) {
		std::cout << e.what();
	}
}

TEST_CASE(testVehicleAssignGetAndRemoveOwner) {
	try {
		Vehicle a("TX1111XH", "Audi");

		Person p("Ivan", 42);

		a.setOwner(&p);

		if (a.getOwner() != nullptr) {
			if (a.getOwner()->getName() != p.getName() || a.getOwner()->getID() != p.getID()) {
				hasError = true;
			}
		}
		else {
			hasError = true;
		}

		a.removeOwner();

		if (a.getOwner() != nullptr) {
			hasError = true;
		}
	}
	catch (const std::exception& e) {
		std::cout << e.what();
	}
}
