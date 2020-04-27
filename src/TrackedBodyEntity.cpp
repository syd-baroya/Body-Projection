#include "TrackedBodyEntity.h"


using namespace std;



void TexturedMeshEntity::draw(Program* prog)
{
	MeshEntity::draw(prog);
	GeometryComponent gc = this->getComponent<GeometryComponent>();
	gc.draw(prog);

}

void TrackedBodyEntity::draw(Program* prog)
{
	TexturedMeshEntity::draw(prog);
	//SceneComponent sc = this->getComponent<SceneComponent>();
	//AnimationComponent ac = this->getComponent<AnimationComponent>();
	//sc.draw(prog);
	//ac.draw(prog);
}


void TrackedBodyEntity::update(double frametime)
{
	std::cout << this->components.size() << std::endl;
	for (Component* comp : this->components) {
		if (comp == NULL)
			break;
		comp->update(frametime);
	}
	generateBodyVertices();
}

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


void TrackedBodyEntity::generateJointAngles(std::map<k4abt_joint_id_t, long double>* jointAngles)
{
	float forecastfact = FORECASTFACT;

	if (jointAngles->size() == 0)
	{
		jointAngles->insert(make_pair(K4ABT_JOINT_SHOULDER_RIGHT, calculateJointAngles(getJoint(forecastfact, K4ABT_JOINT_SPINE_CHEST), getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT), getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_ELBOW_RIGHT, calculateJointAngles(getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT), getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT), getJoint(forecastfact, K4ABT_JOINT_WRIST_RIGHT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_WRIST_RIGHT, calculateJointAngles(getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT), getJoint(forecastfact, K4ABT_JOINT_WRIST_RIGHT), getJoint(forecastfact, K4ABT_JOINT_HAND_RIGHT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_HIP_RIGHT, calculateJointAngles(getJoint(forecastfact, K4ABT_JOINT_PELVIS), getJoint(forecastfact, K4ABT_JOINT_HIP_RIGHT), getJoint(forecastfact, K4ABT_JOINT_KNEE_RIGHT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_KNEE_RIGHT, calculateJointAngles(getJoint(forecastfact, K4ABT_JOINT_HIP_RIGHT), getJoint(forecastfact, K4ABT_JOINT_KNEE_RIGHT), getJoint(forecastfact, K4ABT_JOINT_ANKLE_RIGHT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_ANKLE_RIGHT, calculateJointAngles(getJoint(forecastfact, K4ABT_JOINT_KNEE_RIGHT), getJoint(forecastfact, K4ABT_JOINT_ANKLE_RIGHT), getJoint(forecastfact, K4ABT_JOINT_FOOT_RIGHT))));

		jointAngles->insert(make_pair(K4ABT_JOINT_SHOULDER_LEFT, calculateJointAngles(getJoint(forecastfact, K4ABT_JOINT_SPINE_CHEST), getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT), getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_ELBOW_LEFT, calculateJointAngles(getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT), getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT), getJoint(forecastfact, K4ABT_JOINT_WRIST_LEFT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_WRIST_LEFT, calculateJointAngles(getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT), getJoint(forecastfact, K4ABT_JOINT_WRIST_LEFT), getJoint(forecastfact, K4ABT_JOINT_HAND_LEFT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_HIP_LEFT, calculateJointAngles(getJoint(forecastfact, K4ABT_JOINT_PELVIS), getJoint(forecastfact, K4ABT_JOINT_HIP_LEFT), getJoint(forecastfact, K4ABT_JOINT_KNEE_LEFT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_KNEE_LEFT, calculateJointAngles(getJoint(forecastfact, K4ABT_JOINT_HIP_LEFT), getJoint(forecastfact, K4ABT_JOINT_KNEE_LEFT), getJoint(forecastfact, K4ABT_JOINT_ANKLE_LEFT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_ANKLE_LEFT, calculateJointAngles(getJoint(forecastfact, K4ABT_JOINT_KNEE_LEFT), getJoint(forecastfact, K4ABT_JOINT_ANKLE_LEFT), getJoint(forecastfact, K4ABT_JOINT_FOOT_LEFT))));
	}

	else
	{
		jointAngles->operator[](K4ABT_JOINT_SHOULDER_RIGHT) = calculateJointAngles(getJoint(forecastfact, K4ABT_JOINT_SPINE_CHEST), getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT), getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT));
		jointAngles->operator[](K4ABT_JOINT_ELBOW_RIGHT) = calculateJointAngles(getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT), getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT), getJoint(forecastfact, K4ABT_JOINT_WRIST_RIGHT));
		jointAngles->operator[](K4ABT_JOINT_WRIST_RIGHT) = calculateJointAngles(getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT), getJoint(forecastfact, K4ABT_JOINT_WRIST_RIGHT), getJoint(forecastfact, K4ABT_JOINT_HAND_RIGHT));
		jointAngles->operator[](K4ABT_JOINT_HIP_RIGHT) = calculateJointAngles(getJoint(forecastfact, K4ABT_JOINT_PELVIS), getJoint(forecastfact, K4ABT_JOINT_HIP_RIGHT), getJoint(forecastfact, K4ABT_JOINT_KNEE_RIGHT));
		jointAngles->operator[](K4ABT_JOINT_KNEE_RIGHT) = calculateJointAngles(getJoint(forecastfact, K4ABT_JOINT_HIP_RIGHT), getJoint(forecastfact, K4ABT_JOINT_KNEE_RIGHT), getJoint(forecastfact, K4ABT_JOINT_ANKLE_RIGHT));
		jointAngles->operator[](K4ABT_JOINT_ANKLE_RIGHT) = calculateJointAngles(getJoint(forecastfact, K4ABT_JOINT_KNEE_RIGHT), getJoint(forecastfact, K4ABT_JOINT_ANKLE_RIGHT), getJoint(forecastfact, K4ABT_JOINT_FOOT_RIGHT));

		jointAngles->operator[](K4ABT_JOINT_SHOULDER_LEFT) = calculateJointAngles(getJoint(forecastfact, K4ABT_JOINT_SPINE_CHEST), getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT), getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT));
		jointAngles->operator[](K4ABT_JOINT_ELBOW_LEFT) = calculateJointAngles(getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT), getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT), getJoint(forecastfact, K4ABT_JOINT_WRIST_LEFT));
		jointAngles->operator[](K4ABT_JOINT_WRIST_LEFT) = calculateJointAngles(getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT), getJoint(forecastfact, K4ABT_JOINT_WRIST_LEFT), getJoint(forecastfact, K4ABT_JOINT_HAND_LEFT));
		jointAngles->operator[](K4ABT_JOINT_HIP_LEFT) = calculateJointAngles(getJoint(forecastfact, K4ABT_JOINT_PELVIS), getJoint(forecastfact, K4ABT_JOINT_HIP_LEFT), getJoint(forecastfact, K4ABT_JOINT_KNEE_LEFT));
		jointAngles->operator[](K4ABT_JOINT_KNEE_LEFT) = calculateJointAngles(getJoint(forecastfact, K4ABT_JOINT_HIP_LEFT), getJoint(forecastfact, K4ABT_JOINT_KNEE_LEFT), getJoint(forecastfact, K4ABT_JOINT_ANKLE_LEFT));
		jointAngles->operator[](K4ABT_JOINT_ANKLE_LEFT) = calculateJointAngles(getJoint(forecastfact, K4ABT_JOINT_KNEE_LEFT), getJoint(forecastfact, K4ABT_JOINT_ANKLE_LEFT), getJoint(forecastfact, K4ABT_JOINT_FOOT_LEFT));
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

void TrackedBodyEntity::generateBodyVertices()
{

	std::vector<glm::vec3> vertex_coords = this->getComponent<GeometryComponent>().getMutableVertices();

	float forecastfact = FORECASTFACT;
	float z_base = getJoint(forecastfact, 0).z;

	vec3 posi[K4ABT_JOINT_COUNT];
	for (int ii = 1; ii < K4ABT_JOINT_COUNT; ii++)
		joint_positions[ii].z = z_base;

	//for (int ii = 1; ii < K4ABT_JOINT_COUNT; ii++)
	//{
	//	posi[ii] = getJoint(forecastfact, ii);
	//	posi[ii].z = 0;

	//}
	float throat_width = 0.3 * throat_scale;

	float torso_width_left = torso_width_scale * length(getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT).x - getJoint(forecastfact, K4ABT_JOINT_CLAVICLE_LEFT).x);
	float torso_width_right = torso_width_scale * length(getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT).x - getJoint(forecastfact, K4ABT_JOINT_CLAVICLE_RIGHT).x);
	float torso_above_sholders = torso_height_scale * length(getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT).y - getJoint(forecastfact, K4ABT_JOINT_NECK).y);
	float arm_thickness = torso_above_sholders * arm_thickness_scale;
	float leg_thickness = std::max(torso_width_left, torso_width_right) * leg_thickness_scale;
	float foot_thickness = leg_thickness * foot_thickness_scale;
	float head_thickness = torso_above_sholders * head_thickness_scale;

	//torso
	vec3 utl = normalize(
		normalize((getJoint(forecastfact, K4ABT_JOINT_CLAVICLE_LEFT)) - getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT)) +
		normalize(getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT) - getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT))) *
		torso_above_sholders;
	float s = sign(cross((getJoint(forecastfact, K4ABT_JOINT_CLAVICLE_LEFT)) - getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT),
		getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT) - getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT)).z);
	utl *= -s;
	/*cout << s << endl;
	cout << normalize(posi[JointType_SpineShoulder] - posi[JointType_ShoulderLeft]).x << ", " << normalize(posi[JointType_SpineShoulder] - posi[JointType_ShoulderLeft]).y << "... " <<
		normalize(posi[JointType_ElbowLeft] - posi[JointType_ShoulderLeft]).x << ", " << normalize(posi[JointType_ElbowLeft] - posi[JointType_ShoulderLeft]).y << ", :: " << s << endl;*/
	utl = utl + getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT);

	vec3 utr = normalize(
		normalize((getJoint(forecastfact, K4ABT_JOINT_CLAVICLE_RIGHT)) - getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT)) +
		normalize(getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT) - getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT))) *
		torso_above_sholders;
	utr *= sign(cross((getJoint(forecastfact, K4ABT_JOINT_CLAVICLE_RIGHT)) - getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT),
		getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT) - getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT)).z);
	utr = utr + getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT);
	//utr -= vec3(0.5, 0, 0);
	vec3 mtl = normalize(
		normalize(getJoint(forecastfact, K4ABT_JOINT_HIP_LEFT) - getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT)) +
		normalize(getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT) - getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT))) *
		torso_above_sholders;
	mtl *= sign(cross(getJoint(forecastfact, K4ABT_JOINT_HIP_LEFT) - getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT),
		getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT) - getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT)).z);
	mtl = vec3(mtl.x + getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT).x, mtl.y + (getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT).y) * 0.85, (mtl.z + getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT).z));

	vec3 mtr = normalize(
		normalize(getJoint(forecastfact, K4ABT_JOINT_HIP_RIGHT) - getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT)) +
		normalize(getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT) - getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT))) *
		torso_above_sholders;
	mtr *= -sign(cross(getJoint(forecastfact, K4ABT_JOINT_HIP_RIGHT) - getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT),
		getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT) - getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT)).z);
	mtr = vec3(mtr.x + getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT).x, mtr.y + (getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT).y) * 0.85, (mtr.z + getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT).z));
	//mtr -= vec3(1, 0, 0);
	vec3 mll = getJoint(forecastfact, K4ABT_JOINT_PELVIS) + normalize(getJoint(forecastfact, K4ABT_JOINT_HIP_LEFT) - getJoint(forecastfact, K4ABT_JOINT_PELVIS)) * torso_width_left;
	vec3 mlr = getJoint(forecastfact, K4ABT_JOINT_PELVIS) + normalize(getJoint(forecastfact, K4ABT_JOINT_HIP_RIGHT) - getJoint(forecastfact, K4ABT_JOINT_PELVIS)) * torso_width_right;
	vec3 sll = getJoint(forecastfact, K4ABT_JOINT_NECK) + normalize(utl - getJoint(forecastfact, K4ABT_JOINT_NECK)) * torso_width_left * throat_width;
	vec3 slr = getJoint(forecastfact, K4ABT_JOINT_NECK) + normalize(utr - getJoint(forecastfact, K4ABT_JOINT_NECK)) * torso_width_right * throat_width;
	//vec3 mtc = (mtl + mtr) / (float)2.;
	//construct torso:
	vertex_coords.push_back(mll); //0
	vertex_coords.push_back(mlr); //1
	vertex_coords.push_back(mtl); //2
	vertex_coords.push_back(mtr); //3

	vertex_coords.push_back(getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT)); //4
	/*vertex_coords.push_back(getJoint(forecastfact, K4ABT_JOINT_CLAVICLE_LEFT));
	vertex_coords.push_back(getJoint(forecastfact, K4ABT_JOINT_CLAVICLE_RIGHT));*/
	vertex_coords.push_back(getJoint(forecastfact, K4ABT_JOINT_NECK)); //5
	vertex_coords.push_back(getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT)); //6

	vertex_coords.push_back(utl); //7
	vertex_coords.push_back(getJoint(forecastfact, K4ABT_JOINT_NECK)); //8
	vertex_coords.push_back(utr); //9

	vertex_coords.push_back(sll); //10
	vertex_coords.push_back(slr); //11

	//return;

	//arms:

	vec3 a, b;
	a = normalize(getJoint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT) - getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT));
	b = normalize(getJoint(forecastfact, K4ABT_JOINT_WRIST_LEFT) - getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT));

	vec3 ell = normalize(a + b);
	ell *= arm_thickness;
	ell *= -sign(cross(posi[K4ABT_JOINT_SHOULDER_LEFT] - posi[K4ABT_JOINT_ELBOW_LEFT],
		posi[K4ABT_JOINT_WRIST_LEFT] - posi[K4ABT_JOINT_ELBOW_LEFT]).z);
	vec3 hll = getJoint(forecastfact, K4ABT_JOINT_WRIST_LEFT) + ell;
	vec3 hlr = getJoint(forecastfact, K4ABT_JOINT_WRIST_LEFT) - ell;
	vec3 elr = getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT) - ell;
	ell = getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT) + ell;

	vec3 err = normalize(normalize(getJoint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT) - getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT)) +
		normalize(getJoint(forecastfact, K4ABT_JOINT_WRIST_RIGHT) - getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT))) *
		arm_thickness;

	err *= sign(cross(posi[K4ABT_JOINT_SHOULDER_RIGHT] - posi[K4ABT_JOINT_ELBOW_RIGHT],
		posi[K4ABT_JOINT_WRIST_RIGHT] - posi[K4ABT_JOINT_ELBOW_RIGHT]).z);

	vec3 hrr = getJoint(forecastfact, K4ABT_JOINT_WRIST_RIGHT) + err;
	vec3 hrl = getJoint(forecastfact, K4ABT_JOINT_WRIST_RIGHT) - err;
	vec3 erl = getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT) - err;
	err = getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT) + err;


	//construct arms:
	vertex_coords.push_back(hll); //12
	vertex_coords.push_back(hlr); //13
	vertex_coords.push_back(ell); //14
	vertex_coords.push_back(elr); //15

	vertex_coords.push_back(hrl); //16
	vertex_coords.push_back(hrr); //17
	vertex_coords.push_back(erl); //18
	vertex_coords.push_back(err); //19



	//legs
	vec3 fut = getJoint(forecastfact, K4ABT_JOINT_PELVIS) +
		normalize(getJoint(forecastfact, K4ABT_JOINT_PELVIS) - getJoint(forecastfact, K4ABT_JOINT_SPINE_NAVEL)) * torso_above_sholders;

	vec3 kll = normalize(
		normalize(getJoint(forecastfact, K4ABT_JOINT_HIP_LEFT) - getJoint(forecastfact, K4ABT_JOINT_KNEE_LEFT)) +
		normalize(getJoint(forecastfact, K4ABT_JOINT_ANKLE_LEFT) - getJoint(forecastfact, K4ABT_JOINT_KNEE_LEFT))) *
		leg_thickness;
	kll *= -sign(cross(posi[K4ABT_JOINT_HIP_LEFT] - posi[K4ABT_JOINT_KNEE_LEFT],
		posi[K4ABT_JOINT_ANKLE_LEFT] - posi[K4ABT_JOINT_KNEE_LEFT]).z);
	vec3 klr = getJoint(forecastfact, K4ABT_JOINT_KNEE_LEFT) - kll;
	kll = getJoint(forecastfact, K4ABT_JOINT_KNEE_LEFT) + kll;

	vec3 krr = normalize(
		normalize(getJoint(forecastfact, K4ABT_JOINT_HIP_RIGHT) - getJoint(forecastfact, K4ABT_JOINT_KNEE_RIGHT)) +
		normalize(getJoint(forecastfact, K4ABT_JOINT_ANKLE_RIGHT) - getJoint(forecastfact, K4ABT_JOINT_KNEE_RIGHT))) *
		leg_thickness;
	krr *= -sign(cross(posi[K4ABT_JOINT_HIP_RIGHT] - posi[K4ABT_JOINT_KNEE_RIGHT],
		posi[K4ABT_JOINT_ANKLE_RIGHT] - posi[K4ABT_JOINT_KNEE_RIGHT]).z);
	vec3 krl = getJoint(forecastfact, K4ABT_JOINT_KNEE_RIGHT) - krr;
	krr = getJoint(forecastfact, K4ABT_JOINT_KNEE_RIGHT) + krr;

	vec3 all = normalize(
		normalize(getJoint(forecastfact, K4ABT_JOINT_KNEE_LEFT) - getJoint(forecastfact, K4ABT_JOINT_ANKLE_LEFT)) +
		normalize(getJoint(forecastfact, K4ABT_JOINT_FOOT_LEFT) - getJoint(forecastfact, K4ABT_JOINT_ANKLE_LEFT))) *
		foot_thickness;
	all *= -sign(cross(posi[K4ABT_JOINT_KNEE_LEFT] - posi[K4ABT_JOINT_ANKLE_LEFT],
		posi[K4ABT_JOINT_FOOT_LEFT] - posi[K4ABT_JOINT_ANKLE_LEFT]).z);
	vec3 fll = getJoint(forecastfact, K4ABT_JOINT_FOOT_LEFT) + all;
	vec3 flr = getJoint(forecastfact, K4ABT_JOINT_FOOT_LEFT) - all;
	vec3 alr = getJoint(forecastfact, K4ABT_JOINT_ANKLE_LEFT) - all;
	all = getJoint(forecastfact, K4ABT_JOINT_ANKLE_LEFT) + all;

	vec3 arr = normalize(
		normalize(getJoint(forecastfact, K4ABT_JOINT_KNEE_RIGHT) - getJoint(forecastfact, K4ABT_JOINT_ANKLE_RIGHT)) +
		normalize(getJoint(forecastfact, K4ABT_JOINT_FOOT_RIGHT) - getJoint(forecastfact, K4ABT_JOINT_ANKLE_RIGHT))) *
		foot_thickness;
	arr *= -sign(cross(posi[K4ABT_JOINT_KNEE_RIGHT] - posi[K4ABT_JOINT_ANKLE_RIGHT],
		posi[K4ABT_JOINT_FOOT_RIGHT] - posi[K4ABT_JOINT_ANKLE_RIGHT]).z);
	vec3 frr = getJoint(forecastfact, K4ABT_JOINT_FOOT_RIGHT) + arr;
	vec3 frl = getJoint(forecastfact, K4ABT_JOINT_FOOT_RIGHT) - arr;
	vec3 arl = getJoint(forecastfact, K4ABT_JOINT_ANKLE_RIGHT) - arr;
	arr = getJoint(forecastfact, K4ABT_JOINT_ANKLE_RIGHT) + arr;


	//construct legs
	vertex_coords.push_back(fll); //20
	vertex_coords.push_back(flr); //21
	vertex_coords.push_back(all); //22
	vertex_coords.push_back(alr); //23
	vertex_coords.push_back(kll); //24
	vertex_coords.push_back(klr); //25

	vertex_coords.push_back(fut); //26

	vertex_coords.push_back(frl); //27
	vertex_coords.push_back(frr); //28
	vertex_coords.push_back(arl); //29
	vertex_coords.push_back(arr); //30
	vertex_coords.push_back(krl); //31
	vertex_coords.push_back(krr); //32


	//head/throat:
	vec3 chin = getJoint(forecastfact, K4ABT_JOINT_HEAD) + (getJoint(forecastfact, K4ABT_JOINT_NOSE) - getJoint(forecastfact, K4ABT_JOINT_HEAD)) * chinstart;
	vec3 sidechincenter = getJoint(forecastfact, K4ABT_JOINT_HEAD) + (getJoint(forecastfact, K4ABT_JOINT_NOSE) - getJoint(forecastfact, K4ABT_JOINT_HEAD)) * sidechinstart;
	vec3 tophead = getJoint(forecastfact, K4ABT_JOINT_NOSE) + (getJoint(forecastfact, K4ABT_JOINT_NOSE) - getJoint(forecastfact, K4ABT_JOINT_HEAD)) * chinstart * 5.0f;

	vec3 gl = normalize(
		normalize(getJoint(forecastfact, K4ABT_JOINT_NOSE) - getJoint(forecastfact, K4ABT_JOINT_HEAD)) +
		normalize(getJoint(forecastfact, K4ABT_JOINT_NECK) - getJoint(forecastfact, K4ABT_JOINT_HEAD))) *
		head_thickness;
	gl *= -sign(cross(posi[K4ABT_JOINT_NOSE] - posi[K4ABT_JOINT_HEAD],
		posi[K4ABT_JOINT_NECK] - posi[K4ABT_JOINT_HEAD]).z);

	vec3 hl = tophead + gl;
	vec3 hr = tophead - gl;
	vec3 gr = sidechincenter - gl;
	gl = sidechincenter + gl;
	//contruct head
	vertex_coords.push_back(chin); //33
	vertex_coords.push_back(gl); //34
	vertex_coords.push_back(gr); //35
	vertex_coords.push_back(hl); //36
	vertex_coords.push_back(hr); //37



	//hands


	vec3 tll = normalize(
		normalize(getJoint(forecastfact, K4ABT_JOINT_ELBOW_LEFT) - getJoint(forecastfact, K4ABT_JOINT_WRIST_LEFT)) +
		normalize(getJoint(forecastfact, K4ABT_JOINT_HANDTIP_LEFT) - getJoint(forecastfact, K4ABT_JOINT_WRIST_LEFT))) *
		foot_thickness;
	tll *= sign(cross(posi[K4ABT_JOINT_ELBOW_LEFT] - posi[K4ABT_JOINT_WRIST_LEFT],
		posi[K4ABT_JOINT_ELBOW_LEFT] - posi[K4ABT_JOINT_WRIST_LEFT]).z);



	vec3 tlr = getJoint(forecastfact, K4ABT_JOINT_HANDTIP_LEFT) - tll;
	tll = getJoint(forecastfact, K4ABT_JOINT_HANDTIP_LEFT) + tll;



	vec3 trr = normalize(
		normalize(getJoint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT) - getJoint(forecastfact, K4ABT_JOINT_WRIST_RIGHT)) +
		normalize(getJoint(forecastfact, K4ABT_JOINT_HANDTIP_RIGHT) - getJoint(forecastfact, K4ABT_JOINT_WRIST_RIGHT))) *
		foot_thickness;
	trr *= sign(cross(posi[K4ABT_JOINT_ELBOW_RIGHT] - posi[K4ABT_JOINT_WRIST_RIGHT],
		posi[K4ABT_JOINT_HANDTIP_RIGHT] - posi[K4ABT_JOINT_WRIST_RIGHT]).z);
	vec3 trl = getJoint(forecastfact, K4ABT_JOINT_HANDTIP_RIGHT) - trr;
	trr = getJoint(forecastfact, K4ABT_JOINT_HANDTIP_RIGHT) + trr;


	vertex_coords.push_back(tll); //38
	vertex_coords.push_back(tlr); //39
	vertex_coords.push_back(trl); //40
	vertex_coords.push_back(trr); //41

}

//void SceneComponent::updateEntities()
//{
//	if (body.getNumBodies() > 0)
//	{
//		for (int i = 0; i < body.getDeviceCount(); i++) {
//			new_trackedbody_* tb = &body.trackedbody.at(i);
//			generate_joint_angles(&tb->jointAngleMap, *tb);
//		}
//		std::map<k4abt_joint_id_t, long double> avg_angles = average_all_joint_angles(body.trackedbody);
//		std::map < k4abt_joint_id_t, vector<k4abt_joint_id_t>> angleHierarchy;
//		angle_hierarchy(&angleHierarchy);
//		vec3 master_positions[K4ABT_JOINT_COUNT];
//		copy(begin(body.trackedbody.at(0).joint_positions), end(body.trackedbody.at(0).joint_positions), begin(master_positions));
//
//		std::cout << "Length of array = " << (sizeof(master_positions) / sizeof(*master_positions)) << std::endl;
//
//		for (pair< k4abt_joint_id_t, long double> element : avg_angles)
//		{
//			double theta = 0;
//
//			long double new_angle = avg_angles[element.first];
//			long double old_angle = body.trackedbody.at(0).jointAngleMap[element.first];
//
//			if (new_angle > old_angle)
//				theta = new_angle - old_angle;
//			else
//				theta = old_angle - new_angle;
//
//			k4abt_joint_id_t joint_for_angle = element.first;
//			k4abt_joint_id_t joint_for_position = element.first;
//
//			vec3 new_master_joint_position = vec3(coord_after_rotation(vec2(master_positions[joint_for_position].x, master_positions[joint_for_position].y), vec2(master_positions[joint_for_angle].x, master_positions[joint_for_angle].y), theta, new_angle), master_positions[joint_for_position].z);
//
//			vec3 change_in_joint_position = master_positions[joint_for_position] - new_master_joint_position;
//
//			for (k4abt_joint_id_t hierarchy_joints : angleHierarchy[element.first])
//			{
//				master_positions[hierarchy_joints] = master_positions[hierarchy_joints] - change_in_joint_position;
//			}
//
//		}
//		copy(begin(master_positions), end(master_positions), begin(body.trackedbody.at(0).joint_positions));
//	}
//	vector<vec3> temp_posb;
//
//	generate_body_vertices(&body.trackedbody.at(0), &temp_posb, app_posb);
//}
//
//std::map< k4abt_joint_id_t, long double> SceneComponent::averageJointAngles(vector<Entity> tracked_body)
//{
//	std::map< k4abt_joint_id_t, long double> angleAverages;
//	for (int i = 0; i < tracked_body.size(); i++)
//	{
//		for (pair<k4abt_joint_id_t, long double> element : tracked_body.at(i).joint_angle_map)
//		{
//
//			angleAverages[element.first] += element.second;
//			cout << endl << "DEVICE " << i << ": " << "JOINT " << g_jointNames.at(element.first) << "ANGLE " << element.second << endl;
//		}
//	}
//	for (pair< k4abt_joint_id_t, long double> element : angleAverages)
//	{
//		angleAverages[element.first] = element.second / tracked_body.size();
//		cout << endl << "JOINT " << g_jointNames.at(element.first) << "ANGLE AVERAGES " << angleAverages[element.first] << endl;
//	}
//	return angleAverages;
//}
//
