#include "Scene.h"

void Scene::pauseAll()
{
}

void Scene::unpauseAll()
{
}

void Scene::reset()
{
}

void Scene::addEntity(Entity* entity)
{
}


/*
* DETERMINE WHETHER OR NOT TO KEEP THE BOTTOM TWO FUNCTIONS HERE OR PUT IN KINECTSYSTEM
*/

void Scene::updateEntities()
{
	if (body.getNumBodies() > 0)
	{
		for (int i = 0; i < body.getDeviceCount(); i++) {
			new_trackedbody_* tb = &body.trackedbody.at(i);
			generate_joint_angles(&tb->jointAngleMap, *tb);
		}
		std::map<k4abt_joint_id_t, long double> avg_angles = average_all_joint_angles(body.trackedbody);
		std::map < k4abt_joint_id_t, vector<k4abt_joint_id_t>> angleHierarchy;
		angle_hierarchy(&angleHierarchy);
		vec3 master_positions[K4ABT_JOINT_COUNT];
		copy(begin(body.trackedbody.at(0).joint_positions), end(body.trackedbody.at(0).joint_positions), begin(master_positions));

		std::cout << "Length of array = " << (sizeof(master_positions) / sizeof(*master_positions)) << std::endl;

		for (pair< k4abt_joint_id_t, long double> element : avg_angles)
		{
			double theta = 0;

			long double new_angle = avg_angles[element.first];
			long double old_angle = body.trackedbody.at(0).jointAngleMap[element.first];

			if (new_angle > old_angle)
				theta = new_angle - old_angle;
			else
				theta = old_angle - new_angle;

			k4abt_joint_id_t joint_for_angle = element.first;
			k4abt_joint_id_t joint_for_position = element.first;

			vec3 new_master_joint_position = vec3(coord_after_rotation(vec2(master_positions[joint_for_position].x, master_positions[joint_for_position].y), vec2(master_positions[joint_for_angle].x, master_positions[joint_for_angle].y), theta, new_angle), master_positions[joint_for_position].z);

			vec3 change_in_joint_position = master_positions[joint_for_position] - new_master_joint_position;

			for (k4abt_joint_id_t hierarchy_joints : angleHierarchy[element.first])
			{
				master_positions[hierarchy_joints] = master_positions[hierarchy_joints] - change_in_joint_position;
			}

		}
		copy(begin(master_positions), end(master_positions), begin(body.trackedbody.at(0).joint_positions));
	}
	vector<vec3> temp_posb;

	generate_body_vertices(&body.trackedbody.at(0), &temp_posb, app_posb);
}

std::map< k4abt_joint_id_t, long double> Scene::averageJointAngles(vector<Entity> tracked_body)
{
	std::map< k4abt_joint_id_t, long double> angleAverages;
	for (int i = 0; i < tracked_body.size(); i++)
	{
		for (pair<k4abt_joint_id_t, long double> element : tracked_body.at(i).joint_angle_map)
		{

			angleAverages[element.first] += element.second;
			cout << endl << "DEVICE " << i << ": " << "JOINT " << g_jointNames.at(element.first) << "ANGLE " << element.second << endl;
		}
	}
	for (pair< k4abt_joint_id_t, long double> element : angleAverages)
	{
		angleAverages[element.first] = element.second / tracked_body.size();
		cout << endl << "JOINT " << g_jointNames.at(element.first) << "ANGLE AVERAGES " << angleAverages[element.first] << endl;
	}
	return angleAverages;
}
