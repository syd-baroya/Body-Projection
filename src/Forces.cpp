#include "Forces.h"


//Function to create vector of particle positions
vector<vec3> createPart()
{
	//figure out exact pos to start particles at
	vector<vec3> particle_pos;
	float x = 0.0;
	while (x <= 1.0) {
		float y = 0.0;
		while (y <= 1.0) {
			//WHAT SHOULD Z BE?
			particle_pos.push_back(vec3(x, y, -1.618940));
			y += 0.1;
		}
		x += 0.2;
	}
	return particle_pos;
}
//Function to create all of the particle groups
//group 0 is in the bottom left corner
vector<vec2> createGroups() {
	vector<vec2> groups;
	//float add = sqrt(numGroup);
	float x = -5.0;
	int nG = 0;
	while (x <= 5.0) {
		float y = -5.0;
		while (y <= 5.0) {
			groups.push_back(vec2(x, y));
			nG += 1;
			y += (2.0 / GRID_HEIGHT);
		}
		x += (2.0 / GRID_WIDTH);
	}
	printf("There are %d groups\n", nG);
	return groups;
}


//Function to tag each particle with a group
vector<vec4> findGroup(vector<vec3> particle_list, vector<vec2> groups, vector<int>& num_of_group) {
	vector<vec4> particle_groups;
	//array that represents the number of particles in each group at any given time
	for (int i = 0; i < groups.size(); i++) {
		num_of_group.push_back(0);
	}
	//loop to go through particle_list and create particle_groups[] and update groups[]
	for (int i = 0; i < particle_list.size(); i++) {
		//printf("THe particle position: %f, %f\n", particle_list[i].x, particle_list[i].y);
		//check which group particles are in
		 //need to figure out how big the screen is
		 //loop through all the groups to check, break if its in one of the groups
		vec2 part_pos = vec2(particle_list[i].x, particle_list[i].y);
		//printf("GRpup size: %d", groups.size());
		for (int g = 0; g < groups.size(); g++) {
			vec2 group_pos = groups[g];
			//if within bounding box, set flag, add to number in that group and break from loop
			if (within_box(part_pos, group_pos, groups.size())) {
				particle_groups.push_back(vec4(particle_list[i], g));
				int new_val = num_of_group[g] + 1;

				num_of_group[g] = new_val;
				//printf("Got Here\n");
				break;
			}
		}
	}
	for (int i = 0; i < groups.size(); i++) {
		if (num_of_group[i] == 1) {
			printf("Inside func, number in group %d is: %d \n", i, num_of_group[i]);
		}
		
	}
	return particle_groups;
}
//Helper Function to determine if particle is inside of the box
bool within_box(vec2 a, vec2 b, float numG) {
	//.5 because right now, each bounding box has a width and height of .5
	float maxDist = 0.5;
	float dist = sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
	//printf("dist: %f and max distance: %f \n", dist, maxDist);
	if (dist <= maxDist) {
		return true;
	}
	return false;
}