#include "Entity.h"
#include <iostream>
#include <chrono>
#include <random>
#include <sstream>


uuid_t generate_uuid() {
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

std::string uuid_to_string(uuid_t uuid) {
	std::stringstream ss;
	size_t count16 = sizeof(uuid_t) / 2;
	ss << std::hex << static_cast<uint16_t>(uuid);
	for (size_t i = 1; i < count16; i++) {
		uuid >>= 16;
		ss << "-" << std::hex << static_cast<uint16_t>(uuid);
	}
	return(ss.str());
}