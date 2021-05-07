#include <iostream>
#include <stdexcept>

#include "../Homework2/Registration.h"
//#include "Registration.h"

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

void createRegistration(std::string str) {
	try {
		Registration reg(str);
	}
	catch (const std::invalid_argument& e) {
		std::cout << e.what();
	}
}
TEST_CASE(testRegistrationWithValidString) {
	std::string str = "TX2786XH";
	createRegistration(str);
}
TEST_CASE(testRegistrationWithValidStringAndMixedLetters) {
	std::string str = "Tx2786Xh";
	createRegistration(str);
}
TEST_CASE(testRegistrationWithValidStringAndLowerCaseLetters) {
	std::string str = "tx2786xh";
	createRegistration(str);
}
TEST_CASE(testRegistrationWithNumbersOnly) {
	std::string str = "111145";
	createRegistration(str);
}
TEST_CASE(testRegistrationWithLettersOnly) {
	std::string str = "ABCDEFG";
	createRegistration(str);
}
TEST_CASE(testRegistrationWithInvalidString) {
	std::string str = "ABC34DEF";
	createRegistration(str);
}
TEST_CASE(testRegistrationWithValidStringAndSpaces) {
	std::string str = "TX1234XH    ";
	createRegistration(str);
}
TEST_CASE(testRegistrationWithStringWithSpacesOnly) {
	std::string str = "       ";
	createRegistration(str);
}

void testGetReg(std::string str) {
	try {
		Registration reg(str);
		if (reg.getRegistration() != str) {
			hasError = true;
		}
	}
	catch (const std::invalid_argument& e) {
		std::cout << e.what();
	}
}
TEST_CASE(testGetRegistrationWithValidString1) {
	std::string str = "TX2786XH";
	createRegistration(str);
}
TEST_CASE(testGetRegistrationWithValidString2) {
	std::string str = "C1234BA";
	createRegistration(str);
}
TEST_CASE(testGetRegistrationWithValidString3) {
	std::string str = "A9999AA";
	createRegistration(str);
}
TEST_CASE(testGetRegistrationWithCyrilicSymbols) {
	std::string str = "ру1234ум";
	createRegistration(str);
}
