#include <iostream>
#include <stdexcept>

#include "../Homework2/VehicleList.h"
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

VehicleList& vL = VehicleList::Get();


void checkVehicleListInsert(std::string reg, std::string desc) {
	vL.insert(reg, desc);
}
TEST_CASE(testVehicleListInsert) {
	checkVehicleListInsert("TX1111XH", "Audi A3");
	checkVehicleListInsert("TX2222XH", "Audi A4");
	checkVehicleListInsert("TX3333XH", "Audi A6");
	checkVehicleListInsert("TX4444XH", "Audi A8");
	try {
		checkVehicleListInsert("TX3333XH", "Audi A6");
	}
	catch (const std::invalid_argument& e) {
		std::cout << e.what();
	}
}

TEST_CASE(testVehicleListGetVehicle) {
	if (vL.getVehicle("TX3333XH") == nullptr) {
		hasError = true;
	}
}

TEST_CASE(testVehicleListRemoveByRegistration) {
	vL.remove("TX1111XH");
	if (vL.getVehicle("TX1111XH") != nullptr) {
		hasError = true;
	}
}

TEST_CASE(testVehicleListRemoveByPointer) {
	vL.remove("TX4444XH");
	if (vL.getVehicle("TX4444XH") != nullptr) {
		hasError = true;
	}
}

TEST_CASE(testVehicleListGetVehicles) {
	if (vL.getVehicles()[0]->getRegistration() != "TX3333XH") {
		hasError = true;
	}
}

TEST_CASE(testVehicleListAtOperator) {
	if (vL[0]->getRegistration() != "TX3333XH") {
		hasError = true;
	}
}