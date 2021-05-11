#pragma once

#define invalidCommand \
std::invalid_argument("Please enter a valid command. Type \"?\" for more info")

#define unknownCommand \
std::invalid_argument("Unknown command. Type \"?\" for more info")

#define personDoesNotExist \
std::invalid_argument("Such person doesn't exist...")

#define vehicleDoesNotExist \
std::invalid_argument("Such vehicle doesn't exist...")
