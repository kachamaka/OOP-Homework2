#pragma once

#include <cstddef>
#include "Person.h"

class PersonList{
	std::vector<Person*> persons;

	PersonList() {};
	~PersonList();

	void checkPersonExists(unsigned int id) const;
public:
	static PersonList& Get() {
		static PersonList pL_instance;
		return pL_instance;
	}

	PersonList(const PersonList&) = delete;
	PersonList& operator=(const PersonList&) = delete;

	const Person* operator[](std::size_t index) const;

	void insert(std::string name, unsigned int id);

	void remove(unsigned int id);
	void remove(Person* person);

	Person* getPerson(unsigned int id) const;

	const std::vector<Person*>& getPersons() const;

	std::size_t size() const;
};