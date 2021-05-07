#include <iostream>
#include <stdexcept>

#include "../Homework2/PersonList.h"
//#include "PersonList.h"

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

PersonList& pL = PersonList::Get();

void checkPersonListInsert(std::string name, unsigned int id) {
	pL.insert(name, id);
}
TEST_CASE(testPersonListInsert) {
	checkPersonListInsert("Ivan Petrov1", 1);
	checkPersonListInsert("Ivan Petrov2", 2);
	checkPersonListInsert("Ivan Petrov3", 3);
	checkPersonListInsert("Ivan Petrov4", 4);
	checkPersonListInsert("Ivan Petrov5", 5);
	checkPersonListInsert("Ivan Petrov6", 6);
	try {
		checkPersonListInsert("Ivan Petrov6", 6);
	}
	catch (const std::invalid_argument& e) {
		std::cout << e.what();
	}
}

TEST_CASE(testPersonListGetPerson) {
	if (pL.getPerson(4)->getName() != "Ivan Petrov4") {
		hasError = true;
	}
}

TEST_CASE(testPersonListRemoveByID) {
	pL.remove(3);
	if (pL.getPerson(3) != nullptr) {
		hasError = true;
	}
}

TEST_CASE(testPersonListRemoveByPointer) {
	Person* p = pL.getPerson(5);
	pL.remove(p);
	if (pL.getPerson(5) != nullptr) {
		hasError = true;
	}
}

TEST_CASE(testPersonListGetPersons) {
	if (pL.getPersons()[0]->getID() != 1) {
		hasError = true;
	}
}

TEST_CASE(testPersonListAtOperator) {
	if (pL[0]->getID() != 1) {
		hasError = true;
	}
}