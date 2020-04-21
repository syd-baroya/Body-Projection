#pragma once
#ifndef TIME_H
#define TIME_H

#include <GLFW\glfw3.h>


class Time {
public:
	Time(){}
	double getElapsedTime()
	{
		static double lasttime = glfwGetTime();
		double actualtime = glfwGetTime();
		double difference = actualtime - lasttime;
		lasttime = actualtime;
		return difference;
	}
};

#endif
