/*
CPE/CSC 471 Lab base code Wood/Dunn/Eckhardt
*/

#include "application.h"


shared_ptr<Shape> shape;

vec3 modelpos=vec3(0.0500000007,-2.49999881,0), modelscale = vec3(7.60000801,7.60000801,2.0);
float camfov = 3.1415926 / 4.;

float throat_scale = 1.0f;

float torso_width_scale = 1.2;
float torso_height_scale = 0.5;
float arm_thickness_scale = 1.2;
float leg_thickness_scale = 0.4;
float foot_thickness_scale = 0.5;
float head_thickness_scale = 2.0;

float chinstart = 0.5;
float sidechinstart = 1.35;

vec3 points_to_vector(vec3 v1, vec3 v2) {
	return vec3(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
}

vec2 coord_after_rotation(vec2 old_position, vec2 rotating_point, float rotation_angle, float new_angle) {
	float x_prime = 0;
	float y_prime = 0;
	if (new_angle > 90 && new_angle <= 180)
	{
		//rotation_angle = rotation_angle - 90;
		x_prime = -1 * ((old_position.x - rotating_point.x) * cos(rotation_angle) - (old_position.y - rotating_point.y) * sin(rotation_angle)) + rotating_point.x;
		y_prime = (old_position.x - rotating_point.x) * sin(rotation_angle) + (old_position.y - rotating_point.y) * cos(rotation_angle) + rotating_point.y;
	}
	else if (new_angle > 180 && new_angle <= 270)
	{
		//rotation_angle = rotation_angle - 180;
		x_prime = -1 * ((old_position.x - rotating_point.x) * cos(rotation_angle) - (old_position.y - rotating_point.y) * sin(rotation_angle)) + rotating_point.x;
		y_prime = -1 * ((old_position.x - rotating_point.x) * sin(rotation_angle) + (old_position.y - rotating_point.y) * cos(rotation_angle)) + rotating_point.y;
	}
	else if (new_angle > 270 && new_angle <= 360)
	{
		//rotation_angle = rotation_angle - 270;
		x_prime = (old_position.x - rotating_point.x) * cos(rotation_angle) - (old_position.y - rotating_point.y) * sin(rotation_angle) + rotating_point.x;
		y_prime = -1 * ((old_position.x - rotating_point.x) * sin(rotation_angle) + (old_position.y - rotating_point.y) * cos(rotation_angle)) + rotating_point.y;
	}
	else
	{
		x_prime = (old_position.x - rotating_point.x) * cos(rotation_angle) - (old_position.y - rotating_point.y) * sin(rotation_angle) + rotating_point.x;
		y_prime = (old_position.x - rotating_point.x) * sin(rotation_angle) + (old_position.y - rotating_point.y) * cos(rotation_angle) + rotating_point.y;
	}
	return vec2(x_prime, y_prime);
}

long double calc_angle(vec3 v1, vec3 v2)
	{
	v1 = normalize(v1);
	v2 = normalize(v2);
	long double numerator = dot(v1,v2);
	long double denominator = (double)length(v1) * length(v2);
	
	long double f = acos(numerator/denominator);
	//convert radians to degrees

	return f*180.0/PI;
	}


float frand()
	{
	return (float)rand() / 32768.0;
	}
double get_last_elapsed_time()
{
	static double lasttime = glfwGetTime();
	double actualtime =glfwGetTime();
	double difference = actualtime- lasttime;
	lasttime = actualtime;
	return difference;
}



float sign(float f)
{
	if (f < 0)return -1;
	else return 1;
}


long double calculate_joint_angles(vec3 a, vec3 b, vec3 c)
{
	vec3 vec_ab = points_to_vector(a, b);
	vec3 vec_bc = points_to_vector(b, c);

	return calc_angle(vec_ab, vec_bc);

}

std::map< k4abt_joint_id_t, long double> average_all_joint_angles(vector<new_trackedbody_> trackedbody)
{
	std::map< k4abt_joint_id_t, long double> angleAverages;
	for (int i = 0; i < trackedbody.size(); i++) 
	{
		for (pair<k4abt_joint_id_t, long double> element : trackedbody.at(i).jointAngleMap)
		{

			angleAverages[element.first] += element.second;
			cout << endl<< "DEVICE " << i << ": " << "JOINT "<< g_jointNames.at(element.first) << "ANGLE " << element.second<<endl;
		}
	}
	for (pair< k4abt_joint_id_t, long double> element : angleAverages)
	{
		angleAverages[element.first] = element.second / trackedbody.size();
		cout << endl<< "JOINT " << g_jointNames.at(element.first) << "ANGLE AVERAGES " << angleAverages[element.first] <<endl;
	}
	return angleAverages;
}

void generate_joint_angles(std::map<k4abt_joint_id_t, long double> *jointAngles, new_trackedbody_ trackedbody)
{
	float forecastfact = FORECASTFACT;

	if (jointAngles->size() == 0)
	{
		jointAngles->insert(make_pair(K4ABT_JOINT_SHOULDER_RIGHT, calculate_joint_angles(trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_SPINE_CHEST), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_ELBOW_RIGHT, calculate_joint_angles(trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_WRIST_RIGHT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_WRIST_RIGHT, calculate_joint_angles(trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_WRIST_RIGHT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_HAND_RIGHT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_HIP_RIGHT, calculate_joint_angles(trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_PELVIS), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_HIP_RIGHT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_KNEE_RIGHT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_KNEE_RIGHT, calculate_joint_angles(trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_HIP_RIGHT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_KNEE_RIGHT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_ANKLE_RIGHT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_ANKLE_RIGHT, calculate_joint_angles(trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_KNEE_RIGHT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_ANKLE_RIGHT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_FOOT_RIGHT))));

		jointAngles->insert(make_pair(K4ABT_JOINT_SHOULDER_LEFT, calculate_joint_angles(trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_SPINE_CHEST), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_LEFT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_ELBOW_LEFT, calculate_joint_angles(trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_LEFT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_WRIST_LEFT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_WRIST_LEFT, calculate_joint_angles(trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_LEFT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_WRIST_LEFT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_HAND_LEFT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_HIP_LEFT, calculate_joint_angles(trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_PELVIS), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_HIP_LEFT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_KNEE_LEFT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_KNEE_LEFT, calculate_joint_angles(trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_HIP_LEFT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_KNEE_LEFT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_ANKLE_LEFT))));
		jointAngles->insert(make_pair(K4ABT_JOINT_ANKLE_LEFT, calculate_joint_angles(trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_KNEE_LEFT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_ANKLE_LEFT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_FOOT_LEFT))));
	}

	else
	{
		jointAngles->operator[](K4ABT_JOINT_SHOULDER_RIGHT) = calculate_joint_angles(trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_SPINE_CHEST), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT));
		jointAngles->operator[](K4ABT_JOINT_ELBOW_RIGHT) = calculate_joint_angles(trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_WRIST_RIGHT));
		jointAngles->operator[](K4ABT_JOINT_WRIST_RIGHT) = calculate_joint_angles(trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_WRIST_RIGHT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_HAND_RIGHT));
		jointAngles->operator[](K4ABT_JOINT_HIP_RIGHT) = calculate_joint_angles(trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_PELVIS), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_HIP_RIGHT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_KNEE_RIGHT));
		jointAngles->operator[](K4ABT_JOINT_KNEE_RIGHT) = calculate_joint_angles(trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_HIP_RIGHT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_KNEE_RIGHT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_ANKLE_RIGHT));
		jointAngles->operator[](K4ABT_JOINT_ANKLE_RIGHT) = calculate_joint_angles(trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_KNEE_RIGHT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_ANKLE_RIGHT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_FOOT_RIGHT));

		jointAngles->operator[](K4ABT_JOINT_SHOULDER_LEFT) = calculate_joint_angles(trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_SPINE_CHEST), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_LEFT));
		jointAngles->operator[](K4ABT_JOINT_ELBOW_LEFT) = calculate_joint_angles(trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_LEFT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_WRIST_LEFT));
		jointAngles->operator[](K4ABT_JOINT_WRIST_LEFT) = calculate_joint_angles(trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_LEFT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_WRIST_LEFT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_HAND_LEFT));
		jointAngles->operator[](K4ABT_JOINT_HIP_LEFT) = calculate_joint_angles(trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_PELVIS), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_HIP_LEFT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_KNEE_LEFT));
		jointAngles->operator[](K4ABT_JOINT_KNEE_LEFT) = calculate_joint_angles(trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_HIP_LEFT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_KNEE_LEFT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_ANKLE_LEFT));
		jointAngles->operator[](K4ABT_JOINT_ANKLE_LEFT) = calculate_joint_angles(trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_KNEE_LEFT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_ANKLE_LEFT), trackedbody.new_get_joint(forecastfact, K4ABT_JOINT_FOOT_LEFT));
	}


}

void angle_hierarchy(std::map <k4abt_joint_id_t, vector<k4abt_joint_id_t>> *angleHierarchy) 
{
	vector<k4abt_joint_id_t> shoulder_right;
	shoulder_right.push_back(K4ABT_JOINT_SHOULDER_RIGHT);
	shoulder_right.push_back(K4ABT_JOINT_ELBOW_RIGHT);
	shoulder_right.push_back(K4ABT_JOINT_WRIST_RIGHT);
	angleHierarchy->insert(make_pair(K4ABT_JOINT_SHOULDER_RIGHT, shoulder_right));

	vector<k4abt_joint_id_t> elbow_right;
	elbow_right.push_back(K4ABT_JOINT_ELBOW_RIGHT);
	elbow_right.push_back(K4ABT_JOINT_WRIST_RIGHT);
	angleHierarchy->insert(make_pair(K4ABT_JOINT_SHOULDER_RIGHT, elbow_right));

	vector<k4abt_joint_id_t> wrist_right;
	wrist_right.push_back(K4ABT_JOINT_WRIST_RIGHT);
	angleHierarchy->insert(make_pair(K4ABT_JOINT_SHOULDER_RIGHT, wrist_right));

	vector<k4abt_joint_id_t> shoulder_left;
	shoulder_left.push_back(K4ABT_JOINT_SHOULDER_LEFT);
	shoulder_left.push_back(K4ABT_JOINT_ELBOW_LEFT);
	shoulder_left.push_back(K4ABT_JOINT_WRIST_LEFT);
	angleHierarchy->insert(make_pair(K4ABT_JOINT_SHOULDER_LEFT, shoulder_left));

	vector<k4abt_joint_id_t> elbow_left;
	elbow_left.push_back(K4ABT_JOINT_ELBOW_LEFT);
	elbow_left.push_back(K4ABT_JOINT_WRIST_LEFT);
	angleHierarchy->insert(make_pair(K4ABT_JOINT_SHOULDER_LEFT, elbow_left));

	vector<k4abt_joint_id_t> wrist_left;
	wrist_left.push_back(K4ABT_JOINT_WRIST_LEFT);
	angleHierarchy->insert(make_pair(K4ABT_JOINT_SHOULDER_LEFT, wrist_left));

	vector<k4abt_joint_id_t> hip_right;
	hip_right.push_back(K4ABT_JOINT_HIP_RIGHT);
	hip_right.push_back(K4ABT_JOINT_KNEE_RIGHT);
	hip_right.push_back(K4ABT_JOINT_ANKLE_RIGHT);
	angleHierarchy->insert(make_pair(K4ABT_JOINT_SHOULDER_RIGHT, hip_right));

	vector<k4abt_joint_id_t> knee_right;
	knee_right.push_back(K4ABT_JOINT_KNEE_RIGHT);
	knee_right.push_back(K4ABT_JOINT_ANKLE_RIGHT);
	angleHierarchy->insert(make_pair(K4ABT_JOINT_SHOULDER_RIGHT, knee_right));

	vector<k4abt_joint_id_t> ankle_right;
	ankle_right.push_back(K4ABT_JOINT_ANKLE_RIGHT);
	angleHierarchy->insert(make_pair(K4ABT_JOINT_SHOULDER_RIGHT, ankle_right));

	vector<k4abt_joint_id_t> hip_left;
	hip_left.push_back(K4ABT_JOINT_HIP_LEFT);
	hip_left.push_back(K4ABT_JOINT_KNEE_LEFT);
	hip_left.push_back(K4ABT_JOINT_ANKLE_LEFT);
	angleHierarchy->insert(make_pair(K4ABT_JOINT_SHOULDER_LEFT, hip_left));

	vector<k4abt_joint_id_t> knee_left;
	knee_left.push_back(K4ABT_JOINT_KNEE_LEFT);
	knee_left.push_back(K4ABT_JOINT_ANKLE_LEFT);
	angleHierarchy->insert(make_pair(K4ABT_JOINT_SHOULDER_LEFT, knee_left));

	vector<k4abt_joint_id_t> ankle_left;
	ankle_left.push_back(K4ABT_JOINT_ANKLE_LEFT);
	angleHierarchy->insert(make_pair(K4ABT_JOINT_SHOULDER_LEFT, ankle_left));
}

void generate_body_vertices(new_trackedbody_ *trackedbody, vector<vec3>* pos, vector<vec3> app_pos)
{



	float forecastfact = FORECASTFACT;
	float z_base = trackedbody->new_get_joint(forecastfact, 0).z;

	vec3 posi[K4ABT_JOINT_COUNT];
	for (int ii = 1; ii < K4ABT_JOINT_COUNT; ii++)
		trackedbody->joint_positions[ii].z = z_base;

	for (int ii = 1; ii < K4ABT_JOINT_COUNT; ii++)
	{
		posi[ii] = trackedbody->new_get_joint(forecastfact, ii);
		posi[ii].z = 0;

	}
	float throat_width = 0.3 * throat_scale;

	float torso_width_left = torso_width_scale * length(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT).x - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_CLAVICLE_LEFT).x);
	float torso_width_right = torso_width_scale * length(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT).x - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_CLAVICLE_RIGHT).x);
	float torso_above_sholders = torso_height_scale * length(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT).y - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_NECK).y);
	float arm_thickness = torso_above_sholders * arm_thickness_scale;
	float leg_thickness = std::max(torso_width_left, torso_width_right) * leg_thickness_scale;
	float foot_thickness = leg_thickness * foot_thickness_scale;
	float head_thickness = torso_above_sholders * head_thickness_scale;

	//torso
	vec3 utl = normalize(
		normalize((trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_CLAVICLE_LEFT)) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT)) +
		normalize(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_LEFT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT))) *
		torso_above_sholders;
	float s = sign(cross((trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_CLAVICLE_LEFT)) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT),
		trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_LEFT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT)).z);
	utl *= -s;
	/*cout << s << endl;
	cout << normalize(posi[JointType_SpineShoulder] - posi[JointType_ShoulderLeft]).x << ", " << normalize(posi[JointType_SpineShoulder] - posi[JointType_ShoulderLeft]).y << "... " <<
		normalize(posi[JointType_ElbowLeft] - posi[JointType_ShoulderLeft]).x << ", " << normalize(posi[JointType_ElbowLeft] - posi[JointType_ShoulderLeft]).y << ", :: " << s << endl;*/
	utl = utl + trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT);

	vec3 utr = normalize(
		normalize((trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_CLAVICLE_RIGHT)) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT)) +
		normalize(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT))) *
		torso_above_sholders;
	utr *= sign(cross((trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_CLAVICLE_RIGHT)) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT),
		trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT)).z);
	utr = utr + trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT);
	//utr -= vec3(0.5, 0, 0);
	vec3 mtl = normalize(
		normalize(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_HIP_LEFT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT)) +
		normalize(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_LEFT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT))) *
		torso_above_sholders;
	mtl *= sign(cross(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_HIP_LEFT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT),
		trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_LEFT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT)).z);
	mtl = vec3(mtl.x + trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT).x, mtl.y + (trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT).y) * 0.85, (mtl.z + trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT).z));

	vec3 mtr = normalize(
		normalize(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_HIP_RIGHT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT)) +
		normalize(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT))) *
		torso_above_sholders;
	mtr *= -sign(cross(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_HIP_RIGHT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT),
		trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT)).z);
	mtr = vec3(mtr.x + trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT).x, mtr.y + (trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT).y) * 0.85, (mtr.z + trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT).z));
	//mtr -= vec3(1, 0, 0);
	vec3 mll = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_PELVIS) + normalize(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_HIP_LEFT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_PELVIS)) * torso_width_left;
	vec3 mlr = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_PELVIS) + normalize(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_HIP_RIGHT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_PELVIS)) * torso_width_right;
	vec3 sll = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_NECK) + normalize(utl - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_NECK)) * torso_width_left * throat_width;
	vec3 slr = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_NECK) + normalize(utr - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_NECK)) * torso_width_right * throat_width;
	//vec3 mtc = (mtl + mtr) / (float)2.;
	//construct torso:
	pos->push_back(mll); //0
	pos->push_back(mlr); //1
	pos->push_back(mtl); //2
	pos->push_back(mtr); //3

	pos->push_back(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT)); //4
	/*pos->push_back(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_CLAVICLE_LEFT));
	pos->push_back(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_CLAVICLE_RIGHT));*/
	pos->push_back(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_NECK)); //5
	pos->push_back(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT)); //6

	pos->push_back(utl); //7
	pos->push_back(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_NECK)); //8
	pos->push_back(utr); //9

	pos->push_back(sll); //10
	pos->push_back(slr); //11

	//return;

	//arms:

	vec3 a, b;
	a = normalize(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_LEFT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_LEFT));
	b = normalize(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_WRIST_LEFT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_LEFT));

	vec3 ell = normalize(a + b);
	ell *= arm_thickness;
	ell *= -sign(cross(posi[K4ABT_JOINT_SHOULDER_LEFT] - posi[K4ABT_JOINT_ELBOW_LEFT],
		posi[K4ABT_JOINT_WRIST_LEFT] - posi[K4ABT_JOINT_ELBOW_LEFT]).z);
	vec3 hll = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_WRIST_LEFT) + ell;
	vec3 hlr = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_WRIST_LEFT) - ell;
	vec3 elr = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_LEFT) - ell;
	ell = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_LEFT) + ell;

	vec3 err = normalize(normalize(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SHOULDER_RIGHT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT)) +
		normalize(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_WRIST_RIGHT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT))) *
		arm_thickness;

	err *= sign(cross(posi[K4ABT_JOINT_SHOULDER_RIGHT] - posi[K4ABT_JOINT_ELBOW_RIGHT],
		posi[K4ABT_JOINT_WRIST_RIGHT] - posi[K4ABT_JOINT_ELBOW_RIGHT]).z);

	vec3 hrr = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_WRIST_RIGHT) + err;
	vec3 hrl = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_WRIST_RIGHT) - err;
	vec3 erl = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT) - err;
	err = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT) + err;


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
	vec3 fut = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_PELVIS) +
		normalize(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_PELVIS) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_SPINE_NAVEL)) * torso_above_sholders;

	vec3 kll = normalize(
		normalize(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_HIP_LEFT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_KNEE_LEFT)) +
		normalize(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_ANKLE_LEFT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_KNEE_LEFT))) *
		leg_thickness;
	kll *= -sign(cross(posi[K4ABT_JOINT_HIP_LEFT] - posi[K4ABT_JOINT_KNEE_LEFT],
		posi[K4ABT_JOINT_ANKLE_LEFT] - posi[K4ABT_JOINT_KNEE_LEFT]).z);
	vec3 klr = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_KNEE_LEFT) - kll;
	kll = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_KNEE_LEFT) + kll;

	vec3 krr = normalize(
		normalize(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_HIP_RIGHT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_KNEE_RIGHT)) +
		normalize(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_ANKLE_RIGHT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_KNEE_RIGHT))) *
		leg_thickness;
	krr *= -sign(cross(posi[K4ABT_JOINT_HIP_RIGHT] - posi[K4ABT_JOINT_KNEE_RIGHT],
		posi[K4ABT_JOINT_ANKLE_RIGHT] - posi[K4ABT_JOINT_KNEE_RIGHT]).z);
	vec3 krl = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_KNEE_RIGHT) - krr;
	krr = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_KNEE_RIGHT) + krr;

	vec3 all = normalize(
		normalize(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_KNEE_LEFT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_ANKLE_LEFT)) +
		normalize(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_FOOT_LEFT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_ANKLE_LEFT))) *
		foot_thickness;
	all *= -sign(cross(posi[K4ABT_JOINT_KNEE_LEFT] - posi[K4ABT_JOINT_ANKLE_LEFT],
		posi[K4ABT_JOINT_FOOT_LEFT] - posi[K4ABT_JOINT_ANKLE_LEFT]).z);
	vec3 fll = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_FOOT_LEFT) + all;
	vec3 flr = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_FOOT_LEFT) - all;
	vec3 alr = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_ANKLE_LEFT) - all;
	all = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_ANKLE_LEFT) + all;

	vec3 arr = normalize(
		normalize(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_KNEE_RIGHT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_ANKLE_RIGHT)) +
		normalize(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_FOOT_RIGHT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_ANKLE_RIGHT))) *
		foot_thickness;
	arr *= -sign(cross(posi[K4ABT_JOINT_KNEE_RIGHT] - posi[K4ABT_JOINT_ANKLE_RIGHT],
		posi[K4ABT_JOINT_FOOT_RIGHT] - posi[K4ABT_JOINT_ANKLE_RIGHT]).z);
	vec3 frr = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_FOOT_RIGHT) + arr;
	vec3 frl = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_FOOT_RIGHT) - arr;
	vec3 arl = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_ANKLE_RIGHT) - arr;
	arr = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_ANKLE_RIGHT) + arr;


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
	vec3 chin = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_HEAD) + (trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_NOSE) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_HEAD)) * chinstart;
	vec3 sidechincenter = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_HEAD) + (trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_NOSE) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_HEAD)) * sidechinstart;
	vec3 tophead = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_NOSE) + (trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_NOSE) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_HEAD)) * chinstart*5.0f;

	vec3 gl = normalize(
		normalize(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_NOSE) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_HEAD)) +
		normalize(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_NECK) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_HEAD))) *
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


	//vec3 tll = normalize(
	//	normalize(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_LEFT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_WRIST_LEFT)) +
	//	normalize(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_HANDTIP_LEFT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_WRIST_LEFT))) *
	//	foot_thickness;
	//tll *= sign(cross(posi[K4ABT_JOINT_ELBOW_LEFT] - posi[K4ABT_JOINT_WRIST_LEFT],
	//	posi[K4ABT_JOINT_ELBOW_LEFT] - posi[K4ABT_JOINT_WRIST_LEFT]).z);



	//vec3 tlr = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_HANDTIP_LEFT) - tll;
	//tll = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_HANDTIP_LEFT) + tll;

	//if ((!trackedbody->jointTracked[K4ABT_JOINT_ELBOW_LEFT] || !trackedbody->jointTracked[K4ABT_JOINT_WRIST_LEFT] || !trackedbody->jointTracked[K4ABT_JOINT_HANDTIP_LEFT]) && !app_pos.empty()) 
	//{
	//	tll = app_pos[38];
	//	tlr = app_pos[39];
	//}

	//vec3 trr = normalize(
	//	normalize(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_ELBOW_RIGHT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_WRIST_RIGHT)) +
	//	normalize(trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_HANDTIP_RIGHT) - trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_WRIST_RIGHT))) *
	//	foot_thickness;
	//trr *= sign(cross(posi[K4ABT_JOINT_ELBOW_RIGHT] - posi[K4ABT_JOINT_WRIST_RIGHT],
	//	posi[K4ABT_JOINT_HANDTIP_RIGHT] - posi[K4ABT_JOINT_WRIST_RIGHT]).z);
	//vec3 trl = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_HANDTIP_RIGHT) - trr;
	//trr = trackedbody->new_get_joint(forecastfact, K4ABT_JOINT_HANDTIP_RIGHT) + trr;

	//if ((!trackedbody->jointTracked[K4ABT_JOINT_ELBOW_RIGHT] || !trackedbody->jointTracked[K4ABT_JOINT_WRIST_RIGHT] || !trackedbody->jointTracked[K4ABT_JOINT_HANDTIP_RIGHT]) && !app_pos.empty())
	//{
	//	trl = app_pos[40];
	//	trr = app_pos[41];
	//}

	//pos->push_back(tll); //38
	//pos->push_back(tlr); //39
	//pos->push_back(trl); //40
	//pos->push_back(trr); //41

}


//******************************************************************************************
enum progmode_ {MODE_STDBY,MODE_GO,MODE_UNKNOWN};
int main(int argc, char **argv)
{
	progmode_ progmode = MODE_GO;
	std::string resourceDir = "../resources"; // Where the resources are loaded from

	ifstream testfile;
	string path = resourceDir + "/frag.glsl";
	testfile.open(path);
	if (testfile.is_open() == false)
		{
		resourceDir = "../../resources";
		path = resourceDir + "/frag.glsl";
		testfile.open(path);
		if (testfile.is_open() == false)
			{
			cout << "ERROR, resource folder cannot be found or there is no frag.glsl" << endl;
			return 0;
			}
		}

	if (argc >= 2)
	{
		resourceDir = argv[1];
	}
	srand(time(0));
	Application *application = new Application();
#ifdef NOKINECT
	application->get_record(&application->body, &application->body.trackedbody);
#endif // NOKINECT


	/* your main will always include a similar set up to establish your window
		and GL context, etc. */
	WindowManager * windowManager = new WindowManager();
	windowManager->init(1280,800);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	/* This is the code that will likely change program to program as you
		may need to initialize or set up different data and state */
	// Initialize scene.
	application->init(resourceDir);
	application->initGeom();
	application->init_atomic();

	// Loop until the user closes the window.
	mat4 V, P;
	int width, height;
	glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
	P = glm::perspective(camfov, (float)((float)width / (float)height), 0.1f, 1000.0f); //so much type casting... GLM metods are quite funny ones

	double totaltime = 0;
	double cooldowntime = 0;
	double time_since_last_body_tracked = 0;
	double countfps = 0;
	int frame = 0;
	windowManager->SetFullScreen(fullscreen);

	//app nicht 2 mal oder oefter ausfuehren
	const char szUniqueNamedMutex[] = "eyeofmetamorphosis";
	//HANDLE hHandle = CreateMutex(NULL, TRUE, szUniqueNamedMutex);
	//if (ERROR_ALREADY_EXISTS == GetLastError())
	//	{
	//	cout << "program runs" << endl;
	//	return(1); // Exit program
	//	}

	while(! glfwWindowShouldClose(windowManager->getHandle()))
		{
		windowManager->SetFullScreen(fullscreen);
		double frametime = get_last_elapsed_time();
		
		countfps += frametime;
		frame++;
		if (frame >= 100)
		{
//#ifndef RELEASEVERSION
//			cout << 1./(countfps / 100.) << endl;
//#endif
			frame = 0;
			countfps = 0;
		}
		
		
		if (application->runmode == RUN_NOFIRE)
			totaltime = 0;
		else
			totaltime += frametime;


		float cooldowntime = COOLDOWNTIME;
		float firestarttime = FIRESTARTTIME;
		if (application->runmode == RUN_DEBUGFIRE)
			{
			firestarttime = FIRESTARTTIMEDEBUG;
			cooldowntime = COOLDOWNTIMEDEBUG;
			}
		
		if (totaltime > firestarttime + cooldowntime)
			{
			application->roll_dice();
			totaltime = 0;
			}
		else if (totaltime > firestarttime)
			{
			application->phaseprogresstotaltime += frametime;		
			}


		V = mycam.process(frametime);
		// Render scene.
		static int bodytracked = 0;
		static bool hz30 = true;
		if(hz30) 
			bodytracked = application->Update_Kinect(frametime);
		hz30 = !hz30;
		if (bodytracked > 0)
			{
			//cout << application->body.trackedbody.joint_positions[K4ABT_JOINT_SPINE_CHEST].x << endl;
			application->render_body_to_FBO(frametime, P, V);
			application->render_render_fire_to_screen_FBO(frametime, P, V);
			application->compute();
			time_since_last_body_tracked = 0;
			//if (firstTime == true) 
			//	{
			//		application->record();
			//		firstTime = false;
			//	}
			}
		else
			time_since_last_body_tracked += frametime;
		bool black = false;
		if (time_since_last_body_tracked > 1.0)
			{
			application->roll_dice();
			totaltime = 0;
			black = true;
			}
		application->render_to_screen(black);
		application->get_SSBO_back();
		application->read_atomic();
		//system("pause");
		// Swap front and back buffers.
		glfwSwapBuffers(windowManager->getHandle());
		// Poll for and process events.
		glfwPollEvents();
		}

	// Quit program.
	windowManager->shutdown();
#ifndef NOKINECT
	application->Close_Kinect();
#endif

	return 0;
}
