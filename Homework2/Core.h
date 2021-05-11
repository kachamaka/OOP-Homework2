#pragma once

#include <string>
#include <vector>

class Core {
	std::string line;

	void validateInput() const;

	void parseCommand() const;
	void executeCommand(const std::vector<std::string>& cmd) const;

	static void vehicleCommand(std::string reg, std::string desc);
	static void personCommand(std::string name, std::size_t id);
	static void acquireCommand(std::size_t ownerID, std::string vehicleID);
	static void releaseCommand(std::size_t ownerID, std::string vehicleID);
	static void removeCommand(std::string vehicleID);
	static void removeCommand(std::size_t ownerID);
	static void showCommand(std::string vehicleID);
	static void showCommand(std::size_t ownerID);
	static void saveCommand(std::string path);
public:
	void start();
	void start(std::string path);
};