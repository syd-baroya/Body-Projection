#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include <string>


typedef uintmax_t uuid_t;

uuid_t generate_uuid();

std::string uuid_to_string(uuid_t uuid);

class Entity {
public:
	Entity() : uuid(generate_uuid()) {}
	Entity(const std::string& name) : uuid(generate_uuid()), _name(name) {}

protected:
	std::string _name;

private:
	uuid_t uuid;
};

#endif