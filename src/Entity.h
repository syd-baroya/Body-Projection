#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include <glm/glm.hpp>
#include "Component.h"
#include <iostream>
#include <chrono>
#include <random>
#include <sstream>
#include <array>

typedef uintmax_t uuid_t;

inline uuid_t generate_uuid() {

    const auto now_tp = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());

    unsigned long ns_base = static_cast<unsigned long>(now_tp.count());

    std::random_device rd;

    std::mt19937 generator(rd());

    std::uniform_int_distribution<int> distribution(0, (1 << 16) - 1);

    uuid_t result_uuid = 0;

    size_t count16 = sizeof(uuid_t) / 2;

    for (size_t i = 1; i < count16; i++) {
        result_uuid <<= 16;
        result_uuid |= static_cast<uint16_t>(distribution(generator));
    }
    return(result_uuid);
}



inline std::string uuid_to_string(uuid_t uuid) {

    std::stringstream ss;

    size_t count16 = sizeof(uuid_t) / 2;

    ss << std::hex << static_cast<uint16_t>(uuid);

    for (size_t i = 1; i < count16; i++) {
        uuid >>= 16;
        ss << "-" << std::hex << static_cast<uint16_t>(uuid);
    }
    return(ss.str());
}

using ComponentID = std::size_t;

inline ComponentID getComponentTypeID()
{
    static ComponentID lastID = 0;
    return lastID++;
}



template <typename T> inline ComponentID getComponentTypeID() noexcept
{
    static ComponentID typeID = getComponentTypeID();
    return typeID;
}



constexpr std::size_t maxComponents = 32;

class Entity {
public:
	Entity() : uuid(generate_uuid()) {}
	Entity(const std::string& name) : uuid(generate_uuid()), _name(name) {}

	std::string getName() { return(_name); }
	uuid_t getUUID() { return(uuid); }

	virtual void update() {}

    virtual void draw(Program* prog) {}

    bool isActive() { return active; }

    void setActive(bool a) { active = a; }



    template <typename T, typename... TArgs>

    T* addComponent(TArgs&&... mArgs)
    {
        T* c(new T(std::forward<TArgs>(mArgs)...));
        components[getComponentTypeID<T>()] = c;
        return c;
    }

    template<typename T> T& getComponent() const
    {
        auto ptr(components[getComponentTypeID<T>()]);
        return *static_cast<T*>(ptr);
    }

    virtual ~Entity() {}

protected:
    std::string _name;

private:
    uuid_t uuid;
    bool active = true;
    std::array<Component*, maxComponents> components;
};

#endif