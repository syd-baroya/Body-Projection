#pragma once

#include <iostream>
#include <stdio.h>
#include <glad/glad.h>
#include <strsafe.h>
//#include <Kinect.h>
#include <k4a/k4a.h>
// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;
using namespace glm;

#define RECOVER_JOINT_POSITION_TIME 1.0
#define BODY_LOST_TIME 0.1
#define FORECASTFACT 1.7 //0.1
#define CASCADE 5

template<class Interface>
inline void SafeRelease(Interface*& pInterfaceToRelease)
	{
	if (pInterfaceToRelease != NULL)
		{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
		}
	}
//-------------------------------------
class bodypack_
	{
	public:
		/*IBody* ppBodies[BODY_COUNT];
		BOOLEAN bTracked[BODY_COUNT];
		bodypack_()
			{
			for (int ii = 0; ii < BODY_COUNT; ii++)
				{
				bTracked[ii] = false;
				ppBodies[ii] = NULL;
				}
			}
		void saverelease()
			{
			for (int i = 0; i < _countof(ppBodies); ++i)
				{
				SafeRelease(ppBodies[i]);
				}
			}*/

	};

class trackedbody_
	{
	//private:
	//	int cascade_count[JointType_Count];
	//	vec3 old_joint_positions[JointType_Count][CASCADE];
	//	
	//	
	//public:
	//	vec3 joint_speed[JointType_Count];
	//	vec3 get_joint(float forecast_fact, int j)
	//		{
	//		static vec3 oldv = vec3(0);
	//		vec3 sumv = joint_speed[0] + joint_speed[1] + joint_speed[20];
	//		sumv /= 3.;
	//		oldv = oldv + (sumv - oldv)*0.03f;
	//		//oldv = vec3(0);
	//		return joint_positions[j] - oldv * forecast_fact;// -joint_speed[0] * forecast_fact;
	//	
	//		}
	//	bool init_tracked;
	//	float time_till_last_tracked;
	//	BOOLEAN jointTracked[JointType_Count];
	//	vec3 joint_positions[JointType_Count];
	//	float time_till_last_joint_tracked[JointType_Count];
	//	trackedbody_() { reset(); }
	//	void OnLostTrack()
	//	{}
	//	void reset()
	//		{			
	//		cout << "body resetted" << endl;
	//		init_tracked = false;
	//		time_till_last_tracked = -1;
	//		for (int i = 0; i < JointType_Count; ++i)
	//			{
	//			cascade_count[i] = 0;
	//			jointTracked[i] = false;
	//			joint_positions[i] = vec3(0);
	//			time_till_last_joint_tracked[i] = -1;
	//			for (int ii = 0; ii < CASCADE; ii++)
	//				old_joint_positions[i][ii] = vec3(0);
	//			joint_speed[i] = vec3(0);
	//			}
	//		

	//	
	//		}
	//

	//	void cascade()
	//		{
	//		bool stepcascade = false;
	//		

	//		for (int ii = 0; ii < JointType_Count; ii++)
	//			if (jointTracked[ii])
	//			{
	//				if (cascade_count[ii] == 0)
	//					{
	//					joint_speed[ii] = vec3(0);
	//					old_joint_positions[ii][4] = joint_positions[ii];
	//					}
	//				else
	//					{
	//					vec3 sum = vec3(0);
	//					for (int i = CASCADE -1; i >= CASCADE - cascade_count[ii]; i--)
	//						sum += old_joint_positions[ii][i];
	//					sum /= cascade_count[ii];
	//					
	//					old_joint_positions[ii][CASCADE - 1] = joint_positions[ii];
	//					joint_speed[ii] = sum - joint_positions[ii];
	//					joint_positions[ii] = sum;
	//					
	//					}
	//				stepcascade = true;
	//			}
	//			else if (cascade_count[ii] >0 && time_till_last_joint_tracked[ii] <= RECOVER_JOINT_POSITION_TIME)
	//			{
	//				vec3 sum = vec3(0);
	//				for (int i = 0; i < CASCADE - cascade_count[ii]; i++)
	//					sum += old_joint_positions[ii][i];
	//				sum /= cascade_count[ii];
	//				joint_positions[ii] = sum;
	//				joint_speed[ii] = vec3(0);
	//			}
	//			else
	//			{
	//				joint_speed[ii] = vec3(0);
	//				if (time_till_last_joint_tracked[ii] > RECOVER_JOINT_POSITION_TIME)
	//					cascade_count[ii] = 0;
	//			}


	//		if(stepcascade)
	//		 
	//			for (int ii = 0; ii < JointType_Count; ii++)
	//			{
	//				for (int i = 0; i < CASCADE - 1; i++)
	//					old_joint_positions[ii][i] = old_joint_positions[ii][i + 1];
	//				
	//			if (cascade_count[ii] < CASCADE)
	//				cascade_count[ii]++;

	//			}
	//		
	//		}

	};
//-------------------------------------
class body_
	{
	//private:
	//	bool ProcessBody(float frametime, INT64 nTime, int nBodyCount, bodypack_ * bodypack);
	//	IKinectSensor* KinectSensor;
	//	ICoordinateMapper* CoordinateMapper;
	//	HWND hwnd;
	//	// Body reader
	//	IBodyFrameReader* m_pBodyFrameReader;
	//	
	//public:
	//	trackedbody_ trackedbody;
	//	//******************************
	//	int Update(float frametime);
	//	HRESULT InitializeDefaultSensor();
	//	body_()
	//		{
	//	

	//		KinectSensor = NULL;
	//		CoordinateMapper = NULL;
	//		m_pBodyFrameReader = NULL;

	//		}
	};
