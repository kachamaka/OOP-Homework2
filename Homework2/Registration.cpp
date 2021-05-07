#include <stdexcept>
#include <cstdlib>

#include "Registration.h"

bool isLetter(char c) {
	//works for upper and lower case
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
		return true;
	}
	return false;
}

bool isNumber(char c) {
	if (c >= '0' && c <= '9') {
		return true;
	}
	return false;
}

void Registration::checkValidRegistrationString(std::string registration) const {
	std::size_t size = registration.size();
	if (size < 7 || size > 8) {
		throw std::invalid_argument("Please enter valid registration...");
	}
	//check last chars
	if (!isLetter(registration[size - 1]) || !isLetter(registration[size - 2])) {
		throw std::invalid_argument("Please enter valid registration...");
	}
	//check numbers
	if (!isNumber(registration[size - 3]) ||
		!isNumber(registration[size - 4]) || 
		!isNumber(registration[size - 5]) || 
		!isNumber(registration[size - 6])) {
		throw std::invalid_argument("Please enter valid registration...");
	}
	//check town chars
	if (!isLetter(registration[size - 7])) {
		throw std::invalid_argument("Please enter valid registration...");
	}
	if (size == 8) {
		if (!isLetter(registration[size - 8])) {
			throw std::invalid_argument("Please enter valid registration...");
		}
	}
	
}

Registration::Registration(std::string reg) {
	checkValidRegistrationString(reg);

	this->registration = reg;
}

const std::string& Registration::getRegistration() const {
	return registration;
}