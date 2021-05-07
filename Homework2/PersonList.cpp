#include <stdexcept>
#include <cstdlib>
#include <cassert>

#include "PersonList.h"


PersonList::~PersonList() {
	for (std::size_t i = 0; i < persons.size(); i++) {
		delete persons[i];
	}
}

void PersonList::checkPersonExists(unsigned int id) const {
	for (std::size_t i = 0; i < persons.size(); i++) {
		if (persons[i]->getID() == id) {
			throw std::invalid_argument("Person already exists...");
		}
	}
}

const Person* PersonList::operator[](std::size_t index) const {
	assert(index >= 0 && index < persons.size());

	return persons[index];
}

void PersonList::insert(std::string name, unsigned int id) {
	checkPersonExists(id);

	persons.push_back(new Person(name, id));
}

void PersonList::remove(unsigned int id) {
	for (std::size_t i = 0; i < persons.size(); i++) {
		if (persons[i]->getID() == id) {
			delete persons[i];
			persons[i] = persons[persons.size() - 1];
			persons.pop_back();
			break;
		}
	}
}
void PersonList::remove(Person* person) {
	for (std::size_t i = 0; i < persons.size(); i++) {
		if (persons[i] == person) {
			delete persons[i];
			persons[i] = persons[persons.size() - 1];
			persons.pop_back();
			break;
		}
	}
}

Person* PersonList::getPerson(unsigned int id) const {
	for (std::size_t i = 0; i < persons.size(); i++) {
		if (persons[i]->getID() == id) {
			return persons[i];
		}
	}
	return nullptr;
}

const std::vector<Person*>& PersonList::getPersons() const {
	return persons;
}

std::size_t PersonList::size() const {
	return persons.size();
}