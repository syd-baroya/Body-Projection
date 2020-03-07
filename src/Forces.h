#pragma once
#include <iostream>
#include <stdio.h>
#include <glad/glad.h>
#include <vector>
#include <fstream>
#include "GLSL.h"
#include <glm/glm.hpp>

#define GRID_HEIGHT 20.0
#define GRID_WIDTH 20.0


using namespace std;
using namespace glm;


vector<vec3> createPart();
vector<vec2> createGroups();
vector<vec4> findGroup(vector<vec3> particle_list, vector<vec2> groups, vector<int>& num_of_group);
bool within_box(vec2 a, vec2 b, float numG);

	