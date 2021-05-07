#pragma once
#include <string>

class Registration {
	std::string registration;

	void checkValidRegistrationString(std::string reg) const;
public:
	Registration(std::string reg);

	const std::string& getRegistration() const;
};