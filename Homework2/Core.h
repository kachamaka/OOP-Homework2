#pragma once

#include <string>
#include <vector>

class Core {
	std::string line;

	bool checkValidInput() const;

	void parseCommand() const;
	void executeCommand(const std::vector<std::string>& cmd) const;

	void vehicleCommand(std::string reg, std::string desc) const;
	void personCommand(std::string name, std::size_t id) const;
	void acquireCommand(std::size_t ownerID, std::string vehicleID) const;
	void releaseCommand(std::size_t ownerID, std::string vehicleID) const;
	void removeCommand(std::string vehicleID) const;
	void removeCommand(std::size_t ownerID) const;
	void showCommand(std::string vehicleID) const;
	void showCommand(std::size_t ownerID) const;
	void saveCommand(std::string path) const;
public:
	void start();
	void start(std::string path);
};