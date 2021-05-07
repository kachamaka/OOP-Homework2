#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <fstream>

#include "Core.h"
#include "PersonList.h"
#include "VehicleList.h"

bool isNumber(const std::string& s) {
	for (std::size_t i = 0; i < s.size(); i++) {
		if (s[i] < '0' || s[i] >'9') {
			return false;
		}
	}
	return true;
}
void toLower(std::string& s) {
	for (std::size_t i = 0; i < s.size(); i++) {
		if (s[i] >= 'A' && s[i] <= 'Z') {
			s[i] -= 'A';
			s[i] += 'a';
		}
	}
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

bool Core::checkValidInput() const {
	if (line.size() == 0) {
		return false;
	}
	if (line[0] == ' ' || line[line.size() - 1] == ' ') {
		return false;
	}
	short quotes = 0;
	for (std::size_t i = 0; i < line.size(); i++) {
		if (line[i] == '"') {
			quotes++;
		}
		if (quotes % 2 == 0) {
			if (line[i] == ' ') {
				if (line[i + 1] == ' ') {
					return false;
				}
			}
		}
	}
	if (quotes % 2 != 0) {
		std::cout << quotes <<  " - ERROR PLS!\n";
		return false;
	}

	return true;

}

void Core::start() {
	while (true) {
		std::getline(std::cin, line);

		std::string command = line;
		toLower(command);
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
			parseCommand();
		}
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	start();

}

void Core::parseCommand() const {
	if (checkValidInput() == false) {
		throw std::invalid_argument("Please enter a valid command. Type \"?\" for more info");
	}

	std::vector<std::string> commandArguments;

	std::size_t firstSpaceIndex = line.find(' ');
	if (firstSpaceIndex == std::string::npos) {
		std::string cmd = line;
		toLower(cmd);
		commandArguments.push_back(cmd);
	}
	else {
		std::string cmd = line.substr(0, firstSpaceIndex);
		toLower(cmd);
		commandArguments.push_back(cmd);

		std::size_t startOfSecondArgument = 0;

		//firstspaceindex + 1 is valid because checkValidInput()
		if (line[firstSpaceIndex + 1] == '"') {
			//first arg starts and ends with quote ["]
			std::size_t endOfFirstArgument = line.find('"', firstSpaceIndex + 2);
			if (line[endOfFirstArgument + 1] != ' ') {
				throw std::invalid_argument("Please enter a valid command. Type \"?\" for more info");
			}

			//index of beginning of a word
			std::size_t startOfFirstArgument = firstSpaceIndex + 2;
			commandArguments.push_back(line.substr(startOfFirstArgument, endOfFirstArgument - startOfFirstArgument));

			//index after space after first arg
			startOfSecondArgument = endOfFirstArgument + 2;
		}
		else {
			//first arg has no quotes ["]
			std::size_t secondSpaceIndex = line.find(' ', firstSpaceIndex + 1);
			if (secondSpaceIndex == std::string::npos) {
				commandArguments.push_back(line.substr(firstSpaceIndex + 1));
				startOfSecondArgument = std::string::npos;
			}
			else {
				commandArguments.push_back(line.substr(firstSpaceIndex + 1, secondSpaceIndex - firstSpaceIndex - 1));
				startOfSecondArgument = secondSpaceIndex + 1;
			}
		}

		if (startOfSecondArgument != std::string::npos) {
			if (line[startOfSecondArgument] == '"') {
				//second arg has quotes ["]
				std::size_t endOfSecondArgument = line.find('"', startOfSecondArgument + 1);
				//no need for check because checkValidInput()

				commandArguments.push_back(line.substr(startOfSecondArgument + 1, endOfSecondArgument - startOfSecondArgument - 1));
			}
			else {
				//second arg has no quotes ["]
				//get the rest of the word
				commandArguments.push_back(line.substr(startOfSecondArgument));
			}
		}
	}

	executeCommand(commandArguments);
}

void Core::executeCommand(const std::vector<std::string>& cmd) const {
	if (cmd.size() == 0) {
		throw std::invalid_argument("Please enter a valid command. Type \"?\" for more info");
	}
	if (cmd[0] == "?" || cmd[0] == "help") {
		printCommands();
	}
	else if(cmd[0] == "cls" || cmd[0] == "clear"){
		clearConsole();
	}
	else if (cmd.size() == 2) {
		if (cmd[1] == "") {
			throw std::invalid_argument("Please enter a valid command. Type \"?\" for more info");
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
			throw std::invalid_argument("Unknown command. Type \"?\" for more info");
		}
	}
	else if (cmd.size() == 3) {
		if (cmd[1] == "" || cmd[2] == "") {
			throw std::invalid_argument("Please enter a valid command. Type \"?\" for more info");
		}

		if (cmd[0] == "vehicle") {
			vehicleCommand(cmd[1], cmd[2]);
		}
		else if (cmd[0] == "person") {
			if (!isNumber(cmd[2])) {
				throw std::invalid_argument("Please enter a valid command. Type \"?\" for more info");
			}
			std::size_t ownerID = std::stoi(cmd[2]);
			personCommand(cmd[1], ownerID);
		}
		else if (cmd[0] == "acquire") {
			if (!isNumber(cmd[1])) {
				throw std::invalid_argument("Please enter a valid command. Type \"?\" for more info");
			}
			std::size_t ownerID = std::stoi(cmd[1]);
			acquireCommand(ownerID, cmd[2]);
		}
		else if (cmd[0] == "release") {
			if (!isNumber(cmd[1])) {
				throw std::invalid_argument("Please enter a valid command. Type \"?\" for more info");
			}
			std::size_t ownerID = std::stoi(cmd[1]);
			releaseCommand(ownerID, cmd[2]);
		}
		else {
			throw std::invalid_argument("Unknown command. Type \"?\" for more info");
		}
	}
	else {
		throw std::invalid_argument("Unknown command. Type \"?\" for more info");
	}

}

void Core::vehicleCommand(std::string reg, std::string desc) const {
	VehicleList::Get().insert(reg, desc);
}
void Core::personCommand(std::string name, std::size_t id) const {
	pL.insert(name, id);
}
void Core::acquireCommand(std::size_t ownerID, std::string vehicleID) const {
	Person* person = pL.getPerson(ownerID);
	if (person == nullptr) {
		throw std::invalid_argument("Such person doesn't exist...");
	}
	else {
		Vehicle* vehicle = vL.getVehicle(vehicleID);
		if (vehicle == nullptr) {
			throw std::invalid_argument("Such vehicle doesn't exist...");
		}
		else {
			if (vehicle->getOwner() == nullptr) {
				person->acquireVehicle(vehicle);
				vehicle->setOwner(person);
			}
			else {
				throw std::invalid_argument("Vehicle already has an owner...");
			}
		}
	}
}
void Core::releaseCommand(std::size_t ownerID, std::string vehicleID) const {
	Person* person = pL.getPerson(ownerID);
	if (person == nullptr) {
		throw std::invalid_argument("Such person doesn't exist...");
	} 
	else {
		Vehicle* vehicle = vL.getVehicle(vehicleID);
		if (vehicle == nullptr) {
			throw std::invalid_argument("Such vehicle doesn't exist...");
		}
		else {
			if (vehicle->getOwner() != person) {
				throw std::invalid_argument("Person doesn't own vehicle...");
			}
			else {
				person->releaseVehicle(vehicle);
				vehicle->removeOwner();
			}
		}
	}
}
void Core::removeCommand(std::string vehicleID) const {
	Vehicle* vehicle = vL.getVehicle(vehicleID);
	if (vehicle == nullptr) {
		throw std::invalid_argument("Such vehicle doesn't exist...");
	}
	else {
		if (vehicle->getOwner() == nullptr) {
			vL.remove(vehicle);
		}
		else {
			std::cout << "Vehicle has an owner... Confirm? [Y/n] ";
			std::string confirm;
			std::getline(std::cin, confirm);
			toLower(confirm);
			if (confirm == "y" || confirm == "yes") {
				Person* owner = pL.getPerson(vehicle->getOwner()->getID());
				owner->releaseVehicle(vehicle);
				vL.remove(vehicle);
			}
			else {
				std::cout << "Aborting... \n";
			}
		}
	}
}
void Core::removeCommand(std::size_t ownerID) const {
	Person* person = pL.getPerson(ownerID);
	if (person == nullptr) {
		throw std::invalid_argument("Such person doesn't exist...");
	}
	else {
		if (person->getVehicles().size() == 0) {
			pL.remove(person);
		}
		else {
			std::cout << "Person owns vehicle(s)... Confirm? [Y/n] ";
			std::string confirm;
			std::getline(std::cin, confirm);
			toLower(confirm);
			if (confirm == "y" || confirm == "yes") {
				for (std::size_t i = 0; i < person->getVehicles().size(); i++) {
					person->getVehicles()[i]->removeOwner();
				}
				pL.remove(person);
			}
			else {
				std::cout << "Aborting... \n";
			}
		}
	}
}
void Core::showCommand(std::string vehicleID) const {
	std::string cmd = vehicleID;
	toLower(cmd);
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
			throw std::invalid_argument("Such vehicle doesn't exist...");
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
void Core::showCommand(std::size_t ownerID) const {
	Person* person = pL.getPerson(ownerID);
	if (person == nullptr) {
		throw std::invalid_argument("Such person doesn't exist...");
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
void Core::saveCommand(std::string path) const {
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
		toLower(confirm);
		if (confirm == "y" || confirm == "yes") {
			for (std::size_t i = 0; i < commands.size(); i++) {
				file << commands[i] + "\n";
			}
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
	}

}

