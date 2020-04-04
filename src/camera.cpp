#include "camera.h"

glm::mat4 camera::process(double ftime)
{
	float fwdspeed = 0;
	float udspeed = 0;
	if (w == 1)
	{
		fwdspeed = 1 * ftime;
	}
	else if (s == 1)
	{
		fwdspeed = -1 * ftime;
	}
	if (q == 1)
	{
		udspeed = 1 * ftime;
	}
	else if (e == 1)
	{
		udspeed = -1 * ftime;
	}
	float yangle = 0;
	if (a == 1)
		yangle = -1 * ftime;
	else if (d == 1)
		yangle = 1 * ftime;
	rot.y += yangle;
	glm::mat4 R = glm::rotate(glm::mat4(1), rot.y, glm::vec3(0, 1, 0));
	glm::vec4 dir = glm::vec4(0, udspeed, fwdspeed, 1);
	dir = dir * R;
	pos += glm::vec3(dir.x, dir.y, dir.z);
	glm::mat4 T = glm::translate(glm::mat4(1), pos);
	return R * T;
}
void camera::holdcamera()
{
	ofstream f("campos.dat");
	if (f.is_open() == false) return;
	f << pos.x << endl;
	f << pos.y << endl;
	f << pos.z << endl;
	f << rot.x << endl;
	f << rot.y << endl;
	f << rot.z << endl;
	f << cfov << endl;
	f.close();
}