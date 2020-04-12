#include "TrackedBodyEntity.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "HelperFunctions.h"
#include "BodyTrackingHelpers.h"

using namespace std;


glm::vec3 TrackedBodyEntity::getJoint(float forecast_fact, int j)
{
	float cur_time = glfwGetTime();
	float future_time = glfwGetTime() + +0.000000001f;
	static glm::vec3 oldv = glm::vec3(0);
	glm::vec3 sumv = joint_speed[0] + joint_speed[1] + joint_speed[2];
	sumv /= 3.;
	oldv = oldv + (sumv - oldv) * 0.03f;
	glm::vec3 delta_d = oldv * (future_time - cur_time);
	joint_positions[j] = joint_positions[j] + delta_d;
	return joint_positions[j];// -oldv * forecast_fact;// -joint_speed[0] * forecast_fact;
}

void TrackedBodyEntity::reset()
{
	std::cout << "body resetted" << std::endl;
	init_tracked = false;
	time_till_last_tracked = -1;
	for (int i = 0; i < K4ABT_JOINT_COUNT; i++)
	{
		cascade_count[i] = 0;
		joint_tracked[i] = false;
		joint_positions[i] = glm::vec3(0);
		time_till_last_joint_tracked[i] = -1;
		for (int ii = 0; ii < CASCADE; ii++)
			old_joint_positions[i][ii] = glm::vec3(0);
		joint_speed[i] = glm::vec3(0);
	}

}

void TrackedBodyEntity::cascade()
{
	bool stepcascade = false;


	for (int ii = 0; ii < K4ABT_JOINT_COUNT; ii++)
	{
		if (joint_tracked[ii])
		{
			if (cascade_count[ii] == 0)
			{
				joint_speed[ii] = glm::vec3(0);
				old_joint_positions[ii][4] = joint_positions[ii];
			}
			else
			{
				glm::vec3 sum = glm::vec3(0);
				for (int i = CASCADE - 1; i >= CASCADE - cascade_count[ii]; i--)
					sum += old_joint_positions[ii][i];
				sum /= cascade_count[ii];

				old_joint_positions[ii][CASCADE - 1] = joint_positions[ii];
				joint_speed[ii] = sum - joint_positions[ii];
				joint_positions[ii] = sum;

			}
			stepcascade = true;
		}
		else if (cascade_count[ii] > 0 && time_till_last_joint_tracked[ii] <= RECOVER_JOINT_POSITION_TIME)
		{
			glm::vec3 sum = glm::vec3(0);
			for (int i = 0; i < CASCADE - cascade_count[ii]; i++)
				sum += old_joint_positions[ii][i];
			sum /= cascade_count[ii];
			joint_positions[ii] = sum;
			joint_speed[ii] = glm::vec3(0);
		}
		else
		{
			joint_speed[ii] = glm::vec3(0);
			if (time_till_last_joint_tracked[ii] > RECOVER_JOINT_POSITION_TIME)
				cascade_count[ii] = 0;
		}

	}
	if (stepcascade)
	{
		for (int ii = 0; ii < K4ABT_JOINT_COUNT; ii++)
		{
			for (int i = 0; i < CASCADE - 1; i++)
				old_joint_positions[ii][i] = old_joint_positions[ii][i + 1];

			if (cascade_count[ii] < CASCADE)
				cascade_count[ii]++;

		}
	}
}

long double TrackedBodyEntity::calculateJointAngles(vec3 a, vec3 b, vec3 c)
{
	vec3 vec_ab = points_to_vector(a, b);
	vec3 vec_bc = points_to_vector(b, c);

	return calc_angle(vec_ab, vec_bc);

}

std::map< k4abt_joint_id_t, long double> TrackedBodyEntity::averageJointAngles(vector<TrackedBodyEntity> tracked_body)
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

void TrackedBodyEntity::generateJointAngles(std::map<k4abt_joint_id_t, long double>* jointAngles, TrackedBodyEntity tracked_body)
{
	float forecastfact = FORECASTFACT;

	if (jointAngles->size() == 0)
	{
		jointAngles->insert(make_pair(K4ABT_JOINT_SHOULDER_RIGHT, calculateJointAngles(tracked_body.getJoint(forecastfact, K4ABT_JOINT_SPINE_CHEST), tracked_body.getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_ELBOW_RIGHT, calculateJointAngles(tracked_body.getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_WRIST_RIGHT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_WRIST_RIGHT, calculateJointAngles(tracked_body.getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_WRIST_RIGHT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_HAND_RIGHT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_HIP_RIGHT, calculateJointAngles(tracked_body.getJoint(forecastfact, K4ABT_JOINT_PELVIS), tracked_body.getJoint(forecastfact, K4ABT_JOINT_HIP_RIGHT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_KNEE_RIGHT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_KNEE_RIGHT, calculateJointAngles(tracked_body.getJoint(forecastfact, K4ABT_JOINT_HIP_RIGHT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_KNEE_RIGHT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_ANKLE_RIGHT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_ANKLE_RIGHT, calculateJointAngles(tracked_body.getJoint(forecastfact, K4ABT_JOINT_KNEE_RIGHT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_ANKLE_RIGHT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_FOOT_RIGHT))));

		jointAngles->insert(make_pair(K4ABT_JOINT_SHOULDER_LEFT, calculateJointAngles(tracked_body.getJoint(forecastfact, K4ABT_JOINT_SPINE_CHEST), tracked_body.getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_ELBOW_LEFT, calculateJointAngles(tracked_body.getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_WRIST_LEFT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_WRIST_LEFT, calculateJointAngles(tracked_body.getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_WRIST_LEFT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_HAND_LEFT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_HIP_LEFT, calculateJointAngles(tracked_body.getJoint(forecastfact, K4ABT_JOINT_PELVIS), tracked_body.getJoint(forecastfact, K4ABT_JOINT_HIP_LEFT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_KNEE_LEFT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_KNEE_LEFT, calculateJointAngles(tracked_body.getJoint(forecastfact, K4ABT_JOINT_HIP_LEFT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_KNEE_LEFT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_ANKLE_LEFT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_ANKLE_LEFT, calculateJointAngles(tracked_body.getJoint(forecastfact, K4ABT_JOINT_KNEE_LEFT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_ANKLE_LEFT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_FOOT_LEFT))));
	}

	else
	{
		jointAngles->operator[](K4ABT_JOINT_SHOULDER_RIGHT) = calculateJointAngles(tracked_body.getJoint(forecastfact, K4ABT_JOINT_SPINE_CHEST), tracked_body.getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT));
		jointAngles->operator[](K4ABT_JOINT_ELBOW_RIGHT) = calculateJointAngles(tracked_body.getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_WRIST_RIGHT));
		jointAngles->operator[](K4ABT_JOINT_WRIST_RIGHT) = calculateJointAngles(tracked_body.getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_WRIST_RIGHT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_HAND_RIGHT));
		jointAngles->operator[](K4ABT_JOINT_HIP_RIGHT) = calculateJointAngles(tracked_body.getJoint(forecastfact, K4ABT_JOINT_PELVIS), tracked_body.getJoint(forecastfact, K4ABT_JOINT_HIP_RIGHT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_KNEE_RIGHT));
		jointAngles->operator[](K4ABT_JOINT_KNEE_RIGHT) = calculateJointAngles(tracked_body.getJoint(forecastfact, K4ABT_JOINT_HIP_RIGHT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_KNEE_RIGHT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_ANKLE_RIGHT));
		jointAngles->operator[](K4ABT_JOINT_ANKLE_RIGHT) = calculateJointAngles(tracked_body.getJoint(forecastfact, K4ABT_JOINT_KNEE_RIGHT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_ANKLE_RIGHT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_FOOT_RIGHT));

		jointAngles->operator[](K4ABT_JOINT_SHOULDER_LEFT) = calculateJointAngles(tracked_body.getJoint(forecastfact, K4ABT_JOINT_SPINE_CHEST), tracked_body.getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT));
		jointAngles->operator[](K4ABT_JOINT_ELBOW_LEFT) = calculateJointAngles(tracked_body.getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_WRIST_LEFT));
		jointAngles->operator[](K4ABT_JOINT_WRIST_LEFT) = calculateJointAngles(tracked_body.getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_WRIST_LEFT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_HAND_LEFT));
		jointAngles->operator[](K4ABT_JOINT_HIP_LEFT) = calculateJointAngles(tracked_body.getJoint(forecastfact, K4ABT_JOINT_PELVIS), tracked_body.getJoint(forecastfact, K4ABT_JOINT_HIP_LEFT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_KNEE_LEFT));
		jointAngles->operator[](K4ABT_JOINT_KNEE_LEFT) = calculateJointAngles(tracked_body.getJoint(forecastfact, K4ABT_JOINT_HIP_LEFT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_KNEE_LEFT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_ANKLE_LEFT));
		jointAngles->operator[](K4ABT_JOINT_ANKLE_LEFT) = calculateJointAngles(tracked_body.getJoint(forecastfact, K4ABT_JOINT_KNEE_LEFT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_ANKLE_LEFT), tracked_body.getJoint(forecastfact, K4ABT_JOINT_FOOT_LEFT));
	}


}

void TrackedBodyEntity::angleHierarchy(std::map <k4abt_joint_id_t, vector<k4abt_joint_id_t>>* angle_hierarchy)
{
	vector<k4abt_joint_id_t> shoulder_right;
	shoulder_right.push_back(K4ABT_JOINT_SHOULDER_RIGHT);
	shoulder_right.push_back(K4ABT_JOINT_ELBOW_RIGHT);
	shoulder_right.push_back(K4ABT_JOINT_WRIST_RIGHT);
	angle_hierarchy->insert(make_pair(K4ABT_JOINT_SHOULDER_RIGHT, shoulder_right));

	vector<k4abt_joint_id_t> elbow_right;
	elbow_right.push_back(K4ABT_JOINT_ELBOW_RIGHT);
	elbow_right.push_back(K4ABT_JOINT_WRIST_RIGHT);
	angle_hierarchy->insert(make_pair(K4ABT_JOINT_SHOULDER_RIGHT, elbow_right));

	vector<k4abt_joint_id_t> wrist_right;
	wrist_right.push_back(K4ABT_JOINT_WRIST_RIGHT);
	angle_hierarchy->insert(make_pair(K4ABT_JOINT_SHOULDER_RIGHT, wrist_right));

	vector<k4abt_joint_id_t> shoulder_left;
	shoulder_left.push_back(K4ABT_JOINT_SHOULDER_LEFT);
	shoulder_left.push_back(K4ABT_JOINT_ELBOW_LEFT);
	shoulder_left.push_back(K4ABT_JOINT_WRIST_LEFT);
	angle_hierarchy->insert(make_pair(K4ABT_JOINT_SHOULDER_LEFT, shoulder_left));

	vector<k4abt_joint_id_t> elbow_left;
	elbow_left.push_back(K4ABT_JOINT_ELBOW_LEFT);
	elbow_left.push_back(K4ABT_JOINT_WRIST_LEFT);
	angle_hierarchy->insert(make_pair(K4ABT_JOINT_SHOULDER_LEFT, elbow_left));

	vector<k4abt_joint_id_t> wrist_left;
	wrist_left.push_back(K4ABT_JOINT_WRIST_LEFT);
	angle_hierarchy->insert(make_pair(K4ABT_JOINT_SHOULDER_LEFT, wrist_left));

	vector<k4abt_joint_id_t> hip_right;
	hip_right.push_back(K4ABT_JOINT_HIP_RIGHT);
	hip_right.push_back(K4ABT_JOINT_KNEE_RIGHT);
	hip_right.push_back(K4ABT_JOINT_ANKLE_RIGHT);
	angle_hierarchy->insert(make_pair(K4ABT_JOINT_SHOULDER_RIGHT, hip_right));

	vector<k4abt_joint_id_t> knee_right;
	knee_right.push_back(K4ABT_JOINT_KNEE_RIGHT);
	knee_right.push_back(K4ABT_JOINT_ANKLE_RIGHT);
	angle_hierarchy->insert(make_pair(K4ABT_JOINT_SHOULDER_RIGHT, knee_right));

	vector<k4abt_joint_id_t> ankle_right;
	ankle_right.push_back(K4ABT_JOINT_ANKLE_RIGHT);
	angle_hierarchy->insert(make_pair(K4ABT_JOINT_SHOULDER_RIGHT, ankle_right));

	vector<k4abt_joint_id_t> hip_left;
	hip_left.push_back(K4ABT_JOINT_HIP_LEFT);
	hip_left.push_back(K4ABT_JOINT_KNEE_LEFT);
	hip_left.push_back(K4ABT_JOINT_ANKLE_LEFT);
	angle_hierarchy->insert(make_pair(K4ABT_JOINT_SHOULDER_LEFT, hip_left));

	vector<k4abt_joint_id_t> knee_left;
	knee_left.push_back(K4ABT_JOINT_KNEE_LEFT);
	knee_left.push_back(K4ABT_JOINT_ANKLE_LEFT);
	angle_hierarchy->insert(make_pair(K4ABT_JOINT_SHOULDER_LEFT, knee_left));

	vector<k4abt_joint_id_t> ankle_left;
	ankle_left.push_back(K4ABT_JOINT_ANKLE_LEFT);
	angle_hierarchy->insert(make_pair(K4ABT_JOINT_SHOULDER_LEFT, ankle_left));
}

void TrackedBodyEntity::generateBodyVertices(TrackedBodyEntity* tracked_body, vector<vec3>* pos, vector<vec3> app_pos)
{



	float forecastfact = FORECASTFACT;
	float z_base = tracked_body->getJoint(forecastfact, 0).z;

	vec3 posi[K4ABT_JOINT_COUNT];
	for (int ii = 1; ii < K4ABT_JOINT_COUNT; ii++)
		tracked_body->joint_positions[ii].z = z_base;

	for (int ii = 1; ii < K4ABT_JOINT_COUNT; ii++)
	{
		posi[ii] = tracked_body->getJoint(forecastfact, ii);
		posi[ii].z = 0;

	}
	float throat_width = 0.3 * throat_scale;

	float torso_width_left = torso_width_scale * length(tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT).x - tracked_body->getJoint(forecastfact, K4ABT_JOINT_CLAVICLE_LEFT).x);
	float torso_width_right = torso_width_scale * length(tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT).x - tracked_body->getJoint(forecastfact, K4ABT_JOINT_CLAVICLE_RIGHT).x);
	float torso_above_sholders = torso_height_scale * length(tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT).y - tracked_body->getJoint(forecastfact, K4ABT_JOINT_NECK).y);
	float arm_thickness = torso_above_sholders * arm_thickness_scale;
	float leg_thickness = std::max(torso_width_left, torso_width_right) * leg_thickness_scale;
	float foot_thickness = leg_thickness * foot_thickness_scale;
	float head_thickness = torso_above_sholders * head_thickness_scale;

	//torso
	vec3 utl = normalize(
		normalize((tracked_body->getJoint(forecastfact, K4ABT_JOINT_CLAVICLE_LEFT)) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT)) +
		normalize(tracked_body->getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT))) *
		torso_above_sholders;
	float s = sign(cross((tracked_body->getJoint(forecastfact, K4ABT_JOINT_CLAVICLE_LEFT)) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT),
		tracked_body->getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT)).z);
	utl *= -s;
	/*cout << s << endl;
	cout << normalize(posi[JointType_SpineShoulder] - posi[JointType_ShoulderLeft]).x << ", " << normalize(posi[JointType_SpineShoulder] - posi[JointType_ShoulderLeft]).y << "... " <<
		normalize(posi[JointType_ElbowLeft] - posi[JointType_ShoulderLeft]).x << ", " << normalize(posi[JointType_ElbowLeft] - posi[JointType_ShoulderLeft]).y << ", :: " << s << endl;*/
	utl = utl + tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT);

	vec3 utr = normalize(
		normalize((tracked_body->getJoint(forecastfact, K4ABT_JOINT_CLAVICLE_RIGHT)) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT)) +
		normalize(tracked_body->getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT))) *
		torso_above_sholders;
	utr *= sign(cross((tracked_body->getJoint(forecastfact, K4ABT_JOINT_CLAVICLE_RIGHT)) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT),
		tracked_body->getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT)).z);
	utr = utr + tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT);
	//utr -= vec3(0.5, 0, 0);
	vec3 mtl = normalize(
		normalize(tracked_body->getJoint(forecastfact, K4ABT_JOINT_HIP_LEFT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT)) +
		normalize(tracked_body->getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT))) *
		torso_above_sholders;
	mtl *= sign(cross(tracked_body->getJoint(forecastfact, K4ABT_JOINT_HIP_LEFT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT),
		tracked_body->getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT)).z);
	mtl = vec3(mtl.x + tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT).x, mtl.y + (tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT).y) * 0.85, (mtl.z + tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT).z));

	vec3 mtr = normalize(
		normalize(tracked_body->getJoint(forecastfact, K4ABT_JOINT_HIP_RIGHT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT)) +
		normalize(tracked_body->getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT))) *
		torso_above_sholders;
	mtr *= -sign(cross(tracked_body->getJoint(forecastfact, K4ABT_JOINT_HIP_RIGHT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT),
		tracked_body->getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT)).z);
	mtr = vec3(mtr.x + tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT).x, mtr.y + (tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT).y) * 0.85, (mtr.z + tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT).z));
	//mtr -= vec3(1, 0, 0);
	vec3 mll = tracked_body->getJoint(forecastfact, K4ABT_JOINT_PELVIS) + normalize(tracked_body->getJoint(forecastfact, K4ABT_JOINT_HIP_LEFT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_PELVIS)) * torso_width_left;
	vec3 mlr = tracked_body->getJoint(forecastfact, K4ABT_JOINT_PELVIS) + normalize(tracked_body->getJoint(forecastfact, K4ABT_JOINT_HIP_RIGHT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_PELVIS)) * torso_width_right;
	vec3 sll = tracked_body->getJoint(forecastfact, K4ABT_JOINT_NECK) + normalize(utl - tracked_body->getJoint(forecastfact, K4ABT_JOINT_NECK)) * torso_width_left * throat_width;
	vec3 slr = tracked_body->getJoint(forecastfact, K4ABT_JOINT_NECK) + normalize(utr - tracked_body->getJoint(forecastfact, K4ABT_JOINT_NECK)) * torso_width_right * throat_width;
	//vec3 mtc = (mtl + mtr) / (float)2.;
	//construct torso:
	pos->push_back(mll); //0
	pos->push_back(mlr); //1
	pos->push_back(mtl); //2
	pos->push_back(mtr); //3

	pos->push_back(tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT)); //4
	/*pos->push_back(tracked_body->getJoint(forecastfact, K4ABT_JOINT_CLAVICLE_LEFT));
	pos->push_back(tracked_body->getJoint(forecastfact, K4ABT_JOINT_CLAVICLE_RIGHT));*/
	pos->push_back(tracked_body->getJoint(forecastfact, K4ABT_JOINT_NECK)); //5
	pos->push_back(tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT)); //6

	pos->push_back(utl); //7
	pos->push_back(tracked_body->getJoint(forecastfact, K4ABT_JOINT_NECK)); //8
	pos->push_back(utr); //9

	pos->push_back(sll); //10
	pos->push_back(slr); //11

	//return;

	//arms:

	vec3 a, b;
	a = normalize(tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT));
	b = normalize(tracked_body->getJoint(forecastfact, K4ABT_JOINT_WRIST_LEFT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT));

	vec3 ell = normalize(a + b);
	ell *= arm_thickness;
	ell *= -sign(cross(posi[K4ABT_JOINT_SHOULDER_LEFT] - posi[K4ABT_JOINT_ELBOW_LEFT],
		posi[K4ABT_JOINT_WRIST_LEFT] - posi[K4ABT_JOINT_ELBOW_LEFT]).z);
	vec3 hll = tracked_body->getJoint(forecastfact, K4ABT_JOINT_WRIST_LEFT) + ell;
	vec3 hlr = tracked_body->getJoint(forecastfact, K4ABT_JOINT_WRIST_LEFT) - ell;
	vec3 elr = tracked_body->getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT) - ell;
	ell = tracked_body->getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT) + ell;

	vec3 err = normalize(normalize(tracked_body->getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT)) +
		normalize(tracked_body->getJoint(forecastfact, K4ABT_JOINT_WRIST_RIGHT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT))) *
		arm_thickness;

	err *= sign(cross(posi[K4ABT_JOINT_SHOULDER_RIGHT] - posi[K4ABT_JOINT_ELBOW_RIGHT],
		posi[K4ABT_JOINT_WRIST_RIGHT] - posi[K4ABT_JOINT_ELBOW_RIGHT]).z);

	vec3 hrr = tracked_body->getJoint(forecastfact, K4ABT_JOINT_WRIST_RIGHT) + err;
	vec3 hrl = tracked_body->getJoint(forecastfact, K4ABT_JOINT_WRIST_RIGHT) - err;
	vec3 erl = tracked_body->getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT) - err;
	err = tracked_body->getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT) + err;


	//construct arms:
	pos->push_back(hll); //12
	pos->push_back(hlr); //13
	pos->push_back(ell); //14
	pos->push_back(elr); //15

	pos->push_back(hrl); //16
	pos->push_back(hrr); //17
	pos->push_back(erl); //18
	pos->push_back(err); //19



	//legs
	vec3 fut = tracked_body->getJoint(forecastfact, K4ABT_JOINT_PELVIS) +
		normalize(tracked_body->getJoint(forecastfact, K4ABT_JOINT_PELVIS) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_SPINE_NAVEL)) * torso_above_sholders;

	vec3 kll = normalize(
		normalize(tracked_body->getJoint(forecastfact, K4ABT_JOINT_HIP_LEFT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_KNEE_LEFT)) +
		normalize(tracked_body->getJoint(forecastfact, K4ABT_JOINT_ANKLE_LEFT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_KNEE_LEFT))) *
		leg_thickness;
	kll *= -sign(cross(posi[K4ABT_JOINT_HIP_LEFT] - posi[K4ABT_JOINT_KNEE_LEFT],
		posi[K4ABT_JOINT_ANKLE_LEFT] - posi[K4ABT_JOINT_KNEE_LEFT]).z);
	vec3 klr = tracked_body->getJoint(forecastfact, K4ABT_JOINT_KNEE_LEFT) - kll;
	kll = tracked_body->getJoint(forecastfact, K4ABT_JOINT_KNEE_LEFT) + kll;

	vec3 krr = normalize(
		normalize(tracked_body->getJoint(forecastfact, K4ABT_JOINT_HIP_RIGHT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_KNEE_RIGHT)) +
		normalize(tracked_body->getJoint(forecastfact, K4ABT_JOINT_ANKLE_RIGHT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_KNEE_RIGHT))) *
		leg_thickness;
	krr *= -sign(cross(posi[K4ABT_JOINT_HIP_RIGHT] - posi[K4ABT_JOINT_KNEE_RIGHT],
		posi[K4ABT_JOINT_ANKLE_RIGHT] - posi[K4ABT_JOINT_KNEE_RIGHT]).z);
	vec3 krl = tracked_body->getJoint(forecastfact, K4ABT_JOINT_KNEE_RIGHT) - krr;
	krr = tracked_body->getJoint(forecastfact, K4ABT_JOINT_KNEE_RIGHT) + krr;

	vec3 all = normalize(
		normalize(tracked_body->getJoint(forecastfact, K4ABT_JOINT_KNEE_LEFT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_ANKLE_LEFT)) +
		normalize(tracked_body->getJoint(forecastfact, K4ABT_JOINT_FOOT_LEFT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_ANKLE_LEFT))) *
		foot_thickness;
	all *= -sign(cross(posi[K4ABT_JOINT_KNEE_LEFT] - posi[K4ABT_JOINT_ANKLE_LEFT],
		posi[K4ABT_JOINT_FOOT_LEFT] - posi[K4ABT_JOINT_ANKLE_LEFT]).z);
	vec3 fll = tracked_body->getJoint(forecastfact, K4ABT_JOINT_FOOT_LEFT) + all;
	vec3 flr = tracked_body->getJoint(forecastfact, K4ABT_JOINT_FOOT_LEFT) - all;
	vec3 alr = tracked_body->getJoint(forecastfact, K4ABT_JOINT_ANKLE_LEFT) - all;
	all = tracked_body->getJoint(forecastfact, K4ABT_JOINT_ANKLE_LEFT) + all;

	vec3 arr = normalize(
		normalize(tracked_body->getJoint(forecastfact, K4ABT_JOINT_KNEE_RIGHT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_ANKLE_RIGHT)) +
		normalize(tracked_body->getJoint(forecastfact, K4ABT_JOINT_FOOT_RIGHT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_ANKLE_RIGHT))) *
		foot_thickness;
	arr *= -sign(cross(posi[K4ABT_JOINT_KNEE_RIGHT] - posi[K4ABT_JOINT_ANKLE_RIGHT],
		posi[K4ABT_JOINT_FOOT_RIGHT] - posi[K4ABT_JOINT_ANKLE_RIGHT]).z);
	vec3 frr = tracked_body->getJoint(forecastfact, K4ABT_JOINT_FOOT_RIGHT) + arr;
	vec3 frl = tracked_body->getJoint(forecastfact, K4ABT_JOINT_FOOT_RIGHT) - arr;
	vec3 arl = tracked_body->getJoint(forecastfact, K4ABT_JOINT_ANKLE_RIGHT) - arr;
	arr = tracked_body->getJoint(forecastfact, K4ABT_JOINT_ANKLE_RIGHT) + arr;


	//construct legs
	pos->push_back(fll); //20
	pos->push_back(flr); //21
	pos->push_back(all); //22
	pos->push_back(alr); //23
	pos->push_back(kll); //24
	pos->push_back(klr); //25

	pos->push_back(fut); //26

	pos->push_back(frl); //27
	pos->push_back(frr); //28
	pos->push_back(arl); //29
	pos->push_back(arr); //30
	pos->push_back(krl); //31
	pos->push_back(krr); //32


	//head/throat:
	vec3 chin = tracked_body->getJoint(forecastfact, K4ABT_JOINT_HEAD) + (tracked_body->getJoint(forecastfact, K4ABT_JOINT_NOSE) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_HEAD)) * chinstart;
	vec3 sidechincenter = tracked_body->getJoint(forecastfact, K4ABT_JOINT_HEAD) + (tracked_body->getJoint(forecastfact, K4ABT_JOINT_NOSE) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_HEAD)) * sidechinstart;
	vec3 tophead = tracked_body->getJoint(forecastfact, K4ABT_JOINT_NOSE) + (tracked_body->getJoint(forecastfact, K4ABT_JOINT_NOSE) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_HEAD)) * chinstart * 5.0f;

	vec3 gl = normalize(
		normalize(tracked_body->getJoint(forecastfact, K4ABT_JOINT_NOSE) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_HEAD)) +
		normalize(tracked_body->getJoint(forecastfact, K4ABT_JOINT_NECK) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_HEAD))) *
		head_thickness;
	gl *= -sign(cross(posi[K4ABT_JOINT_NOSE] - posi[K4ABT_JOINT_HEAD],
		posi[K4ABT_JOINT_NECK] - posi[K4ABT_JOINT_HEAD]).z);

	vec3 hl = tophead + gl;
	vec3 hr = tophead - gl;
	vec3 gr = sidechincenter - gl;
	gl = sidechincenter + gl;
	//contruct head
	pos->push_back(chin); //33
	pos->push_back(gl); //34
	pos->push_back(gr); //35
	pos->push_back(hl); //36
	pos->push_back(hr); //37



	//hands


	vec3 tll = normalize(
		normalize(tracked_body->getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_WRIST_LEFT)) +
		normalize(tracked_body->getJoint(forecastfact, K4ABT_JOINT_HANDTIP_LEFT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_WRIST_LEFT))) *
		foot_thickness;
	tll *= sign(cross(posi[K4ABT_JOINT_ELBOW_LEFT] - posi[K4ABT_JOINT_WRIST_LEFT],
		posi[K4ABT_JOINT_ELBOW_LEFT] - posi[K4ABT_JOINT_WRIST_LEFT]).z);



	vec3 tlr = tracked_body->getJoint(forecastfact, K4ABT_JOINT_HANDTIP_LEFT) - tll;
	tll = tracked_body->getJoint(forecastfact, K4ABT_JOINT_HANDTIP_LEFT) + tll;

	if ((!tracked_body->joint_tracked[K4ABT_JOINT_ELBOW_LEFT] || !tracked_body->joint_tracked[K4ABT_JOINT_WRIST_LEFT] || !tracked_body->joint_tracked[K4ABT_JOINT_HANDTIP_LEFT]) && !app_pos.empty())
	{
		tll = app_pos[38];
		tlr = app_pos[39];
	}

	vec3 trr = normalize(
		normalize(tracked_body->getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_WRIST_RIGHT)) +
		normalize(tracked_body->getJoint(forecastfact, K4ABT_JOINT_HANDTIP_RIGHT) - tracked_body->getJoint(forecastfact, K4ABT_JOINT_WRIST_RIGHT))) *
		foot_thickness;
	trr *= sign(cross(posi[K4ABT_JOINT_ELBOW_RIGHT] - posi[K4ABT_JOINT_WRIST_RIGHT],
		posi[K4ABT_JOINT_HANDTIP_RIGHT] - posi[K4ABT_JOINT_WRIST_RIGHT]).z);
	vec3 trl = tracked_body->getJoint(forecastfact, K4ABT_JOINT_HANDTIP_RIGHT) - trr;
	trr = tracked_body->getJoint(forecastfact, K4ABT_JOINT_HANDTIP_RIGHT) + trr;

	if ((!tracked_body->joint_tracked[K4ABT_JOINT_ELBOW_RIGHT] || !tracked_body->joint_tracked[K4ABT_JOINT_WRIST_RIGHT] || !tracked_body->joint_tracked[K4ABT_JOINT_HANDTIP_RIGHT]) && !app_pos.empty())
	{
		trl = app_pos[40];
		trr = app_pos[41];
	}

	pos->push_back(tll); //38
	pos->push_back(tlr); //39
	pos->push_back(trl); //40
	pos->push_back(trr); //41

}


