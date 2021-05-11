#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <fstream>

#include "Core.h"
#include "CoreExceptions.h"
#include "PersonList.h"
#include "VehicleList.h"

bool isNumber(const std::string& s) {
	for (std::size_t i = 0; i < s.size(); i++) {
		if (s[i] < '0' || s[i] >'9') {
			return false;
		}
	}
	if (s.size() >= 2) {
		// 042, 01, 05253
		if (s[0] == '0') {
			return false;
		}
	}
	return true;
}

std::string toLower(const std::string& s) {
	std::string str = s;
	for (std::size_t i = 0; i < str.size(); i++) {
		if (str[i] >= 'A' && str[i] <= 'Z') {
			str[i] = str[i] - 'A' + 'a';
		}
	}
	return str;
}

void transformPath(std::string& s) {
	#ifdef _WIN32
	std::string newS;
	for (std::size_t i = 0; i < s.size(); i++) {
		newS += s[i];
		if (s[i] == '\\') {
			newS += "\\";
		}
	}
	s = newS;
	#endif
}
void printCommands() {
	std::cout << "PERSON <name> <id> - adds person to the list\n";
	std::cout << "VEHICLE <registration> <description> - adds vehicle to the list\n";
	std::cout << "ACQUIRE <owner-id> <vehicle-id> - person acquires vehicle\n";
	std::cout << "RELEASE <owner-id> <vehicle-id> - person releases vehicle\n";
	std::cout << "SHOW [PEOPLE|VEHICLES|<id>] - shows information about people|vehicles or specific object\n";
	std::cout << "REMOVE <what> - removes person|vehicle from the list\n";
	std::cout << "SAVE <path> - saves data to a file\n";
	std::cout << "cls - clear console\n";
	std::cout << "exit - exit program\n";
}
void clearConsole() {
#ifdef _WIN32
	std::system("cls");
#else
	std::system("clear");
#endif
}

PersonList& pL = PersonList::Get();
VehicleList& vL = VehicleList::Get();

void Core::validateInput() const {
		//validation in generic case
		if (line.size() == 0) {
			throw invalidCommand;
		}

		bool inQuotes = false;
		for (std::size_t i = 0; i < line.size(); i++) {
			if (line[i] == '\"') {
				inQuotes = !inQuotes;
			}
		}

		if (inQuotes) {
			throw invalidCommand;
		}
	}

void Core::start() {
	while (true) {
		std::getline(std::cin, line);

		std::string command = toLower(line);
		if (command == "exit") {
			return;
		}

		try {
			parseCommand();
		}
		catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}
}

void Core::start(std::string path) {
	transformPath(path);

	try {
		std::ifstream file;
		file.open(path);
		if (file.fail()) {
			throw std::invalid_argument("Error opening file...");
		}

		while (std::getline(file, line)) {
			std::cout << line << " check??" << "\n";
			parseCommand();
		}
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	start();

}

void Core::parseCommand() const {
	validateInput();

	std::vector<std::string> commandArguments;

	std::size_t argStart = 0;
	std::size_t argEnd = 0;

	bool inQuotes = false;
	bool inWord = false;

	for (std::size_t i = 0; i < line.size(); i++) {
		if (!inWord) {
			if (line[i] == '\"') {
				if (!inQuotes) {
					argStart = i + 1;
				}
				inQuotes = !inQuotes;
			}
			if (!inQuotes) {
				if (line[i] == ' ') {
					argStart = i + 1;
					continue;
				}
				else {
					//line[i] is " or letter
					if (line[i] == '\"') {
						//after exiting quotes iteration
						argEnd = i;
						commandArguments.push_back(line.substr(argStart, argEnd - argStart));
						argStart = argEnd + 1;
						continue;
					}
					else {
						argStart = i;
						if (i == line.size() - 1) {
							//catch one letter word case at the end
							argEnd = line.size();
							commandArguments.push_back(line.substr(argStart, argEnd - argStart));
						}
						inWord = true;
					}
				}
			}
		}
		else {
			if (line[i] == ' ') {
				argEnd = i;
				commandArguments.push_back(line.substr(argStart, argEnd - argStart));
				argStart = argEnd + 1;
				inWord = false;
			}
			else if (i == line.size() - 1) {
				argEnd = i;
				commandArguments.push_back(line.substr(argStart, argEnd - argStart + 1));
				inWord = false;
			}
		}
	}

	commandArguments[0] = toLower(commandArguments[0]);

	executeCommand(commandArguments);
}

void Core::executeCommand(const std::vector<std::string>& cmd) const {
	if (cmd.size() == 0) {
		throw invalidCommand;
	}
	if (cmd[0] == "?" || cmd[0] == "help") {
		printCommands();
	}
	else if(cmd[0] == "cls" || cmd[0] == "clear"){
		clearConsole();
	}
	else if (cmd.size() == 2) {
		if (cmd[1] == "") {
			throw invalidCommand;
		}
		// remove/show/save commands
		if (cmd[0] == "remove") {
			if (isNumber(cmd[1])) {
				std::size_t ownerID = std::stoi(cmd[1]);
				removeCommand(ownerID);
			}
			else {
				removeCommand(cmd[1]);
			}
		}
		else if (cmd[0] == "show") {
			if (isNumber(cmd[1])) {
				std::size_t ownerID = std::stoi(cmd[1]);
				showCommand(ownerID);
			}
			else {
				showCommand(cmd[1]);
			}
		}
		else if (cmd[0] == "save") {
			saveCommand(cmd[1]);
		}
		else {
			throw unknownCommand;
		}
	}
	else if (cmd.size() == 3) {
		if (cmd[1] == "" || cmd[2] == "") {
			throw invalidCommand;
		}

		if (cmd[0] == "vehicle") {
			vehicleCommand(cmd[1], cmd[2]);
		}
		else if (cmd[0] == "person") {
			if (!isNumber(cmd[2])) {
				throw invalidCommand;
			}
			std::size_t ownerID = std::stoi(cmd[2]);
			personCommand(cmd[1], ownerID);
		}
		else if (cmd[0] == "acquire") {
			if (!isNumber(cmd[1])) {
				throw invalidCommand;
			}
			std::size_t ownerID = std::stoi(cmd[1]);
			acquireCommand(ownerID, cmd[2]);
		}
		else if (cmd[0] == "release") {
			if (!isNumber(cmd[1])) {
				throw invalidCommand;
			}
			std::size_t ownerID = std::stoi(cmd[1]);
			releaseCommand(ownerID, cmd[2]);
		}
		else {
			std::cout << "WHAT???????????11\n";
			throw unknownCommand;
		}
	}
	else {
		throw unknownCommand;
	}

}

void Core::vehicleCommand(std::string reg, std::string desc) {
	VehicleList::Get().insert(reg, desc);
	std::cout << "Vehicle successfully added! \n";
}
void Core::personCommand(std::string name, std::size_t id) {
	pL.insert(name, id);
	std::cout << "Person successfully added! \n";
}
void Core::acquireCommand(std::size_t ownerID, std::string vehicleID) {
	Person* person = pL.getPerson(ownerID);
	if (person == nullptr) {
		throw personDoesNotExist;
	}
	else {
		Vehicle* vehicle = vL.getVehicle(vehicleID);
		if (vehicle == nullptr) {
			throw vehicleDoesNotExist;
		}
		else {
			if (vehicle->getOwner() == nullptr) {
				person->acquireVehicle(vehicle);
				vehicle->setOwner(person);
				std::cout << "Person successfully acquired a vehicle! \n";
			}
			else {
				throw std::invalid_argument("Vehicle already has an owner...");
			}
		}
	}
}
void Core::releaseCommand(std::size_t ownerID, std::string vehicleID) {
	Person* person = pL.getPerson(ownerID);
	if (person == nullptr) {
		throw personDoesNotExist;
	} 
	else {
		Vehicle* vehicle = vL.getVehicle(vehicleID);
		if (vehicle == nullptr) {
			throw vehicleDoesNotExist;
		}
		else {
			if (vehicle->getOwner() != person) {
				throw std::invalid_argument("Person doesn't own vehicle...");
			}
			else {
				person->releaseVehicle(vehicle);
				vehicle->removeOwner();
				std::cout << "Person successfully released a vehicle! \n";
			}
		}
	}
}
void Core::removeCommand(std::string vehicleID) {
	Vehicle* vehicle = vL.getVehicle(vehicleID);
	if (vehicle == nullptr) {
		throw vehicleDoesNotExist;
	}
	else {
		if (vehicle->getOwner() == nullptr) {
			vL.remove(vehicle);
			std::cout << "Vehicle successfully removed! \n";
		}
		else {
			std::cout << "Vehicle has an owner... Confirm? [Y/n] ";
			std::string confirm;
			std::getline(std::cin, confirm);
			confirm = toLower(confirm);
			if (confirm == "y" || confirm == "yes") {
				Person* owner = pL.getPerson(vehicle->getOwner()->getID());
				owner->releaseVehicle(vehicle);
				vL.remove(vehicle);
				std::cout << "Vehicle successfully removed! \n";
			}
			else {
				std::cout << "Aborting... \n";
			}
		}
	}
}
void Core::removeCommand(std::size_t ownerID) {
	Person* person = pL.getPerson(ownerID);
	if (person == nullptr) {
		throw personDoesNotExist;
	}
	else {
		if (person->getVehicles().size() == 0) {
			pL.remove(person);
			std::cout << "Person successfully removed! \n";
		}
		else {
			std::cout << "Person owns vehicle(s)... Confirm? [Y/n] ";
			std::string confirm;
			std::getline(std::cin, confirm);
			confirm = toLower(confirm);
			if (confirm == "y" || confirm == "yes") {
				for (std::size_t i = 0; i < person->getVehicles().size(); i++) {
					person->getVehicles()[i]->removeOwner();
				}
				pL.remove(person);
				std::cout << "Person successfully removed! \n";
			}
			else {
				std::cout << "Aborting... \n";
			}
		}
	}
}
void Core::showCommand(std::string vehicleID) {
	std::string cmd = toLower(vehicleID);
	if (cmd == "people") {
		if (pL.size() == 0) {
			std::cout << "There are currently no people...\n";
		}
		else {
			for (std::size_t i = 0; i < pL.size(); i++) {
				std::cout << "-------------------------\n";
				std::cout << "ID: " << pL[i]->getID() << "\n";
				std::cout << "Name: " << pL[i]->getName() << "\n";
				std::cout << "-------------------------\n";
			}
		}
	}else if (cmd == "vehicles") {
		if (vL.size() == 0) {
			std::cout << "There are currently no vehicles...\n";
		}
		else {
			for (std::size_t i = 0; i < vL.size(); i++) {
				std::cout << "-------------------------\n";
				std::cout << "Registration: " << vL[i]->getRegistration() << "\n";
				std::cout << "Description: " << vL[i]->getDescription() << "\n";
				std::cout << "-------------------------\n";
			}
		}
	}
	else {
		Vehicle* vehicle = vL.getVehicle(vehicleID);
		if (vehicle == nullptr) {
			throw vehicleDoesNotExist;
		}
		else {
			std::cout << "-------------------------\n";
			std::cout << "Registration: " << vehicleID << "\n";
			std::cout << "Description: " << vehicle->getDescription() << "\n";
			if (vehicle->getOwner() == nullptr) {
				std::cout << "Owner: none" << "\n";
			}
			else {
				std::cout << "Owner: " << vehicle->getOwner()->getName()<< "\n";
				std::cout << "Owner ID: " << vehicle->getOwner()->getID() << "\n";
			}
			std::cout << "-------------------------\n";
		}
	}
}
void Core::showCommand(std::size_t ownerID) {
	Person* person = pL.getPerson(ownerID);
	if (person == nullptr) {
		throw personDoesNotExist;
	}
	else {
		std::cout << "-------------------------\n";
		std::cout << "ID: " << person->getID() << "\n";
		std::cout << "Name: " << person->getName() << "\n";
		if (person->getVehicles().size() == 0) {
			std::cout << "Vehicles: none\n";
		}
		else {
			std::cout << "Vehicles: \n";
			for (std::size_t i = 0; i < person->getVehicles().size(); i++) {
				std::cout << "-----------\n";
				std::cout << "Registration: " << person->getVehicles()[i]->getRegistration() << "\n";
				std::cout << "Description: " << person->getVehicles()[i]->getDescription() << "\n";
				std::cout << "-----------\n";
			}
		}
		std::cout << "-------------------------\n";
	}

}
void Core::saveCommand(std::string path) {
	std::vector<std::string> commands;

	//save people
	for (std::size_t i = 0; i < pL.size(); i++) {
		Person* p = pL.getPersons()[i];
		std::string name;
		if (p->getName().find(' ') == std::string::npos) {
			name = p->getName();
		}
		else {
			name = "\"" + p->getName() + "\"";
		}
		std::string cmd = "PERSON " + name + " " + std::to_string(p->getID());
		commands.push_back(cmd);
	}

	//save vehicles
	for (std::size_t j = 0; j < vL.size(); j++) {
		Vehicle* v = vL.getVehicles()[j];
		std::string desc;
		if (v->getDescription().find(' ') == std::string::npos) {
			desc = v->getDescription();
		}
		else {
			desc = "\"" + v->getDescription() + "\"";
		}
		std::string cmd = "VEHICLE " + v->getRegistration() + " " + desc;
		commands.push_back(cmd);
	}

	//save relations
	for (std::size_t k = 0; k < vL.size(); k++) {
		Vehicle* v = vL.getVehicles()[k];
		if (v->getOwner() != nullptr) {
			std::string cmd = "ACQUIRE " + std::to_string(v->getOwner()->getID()) + " " + v->getRegistration();
			commands.push_back(cmd);
		}
	}

	transformPath(path);

	std::ifstream checkFile(path);
	if (checkFile.is_open()) {
		//file exists
		std::ofstream file;
		file.open(path);

		if (file.fail()) {
			throw std::invalid_argument("Error opening file...");
		}

		std::cout << "File already exists... Overwrite? [Y/n] ";
		std::string confirm;
		std::getline(std::cin, confirm);
		confirm = toLower(confirm);
		if (confirm == "y" || confirm == "yes") {
			for (std::size_t i = 0; i < commands.size(); i++) {
				file << commands[i] + "\n";
			}
			std::cout << "Data saved successfully! \n";
		}
		else {
			std::cout << "Aborting... \n";
			return;
		}
	}
	else {
		//create new file
		std::ofstream file;
		file.open(path);

		for (std::size_t i = 0; i < commands.size(); i++) {
			file << commands[i] + "\n";
		}
		std::cout << "Data saved successfully! \n";
	}

}
