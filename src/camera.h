#pragma once
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
using namespace std;

class camera
{
public:
	glm::vec3 pos, rot;
	int w, a, s, d, q, e;
	float cfov;
	camera()
	{
		cfov = 3.1415926 / 4.;
		w = a = s = d = q = e = 0;
		rot = glm::vec3(0, 0, 0);
		pos = glm::vec3(0, 0, -5);
		ifstream f("campos.dat");
		if (f.is_open() == false)return;
		f >> pos.x;
		f >> pos.y;
		f >> pos.z;
		f >> rot.x;
		f >> rot.y;
		f >> rot.z;
		f >> cfov;
		f.close();
	}
	void holdcamera();
	glm::mat4 process(double ftime);
};