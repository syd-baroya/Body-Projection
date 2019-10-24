#include "body.h"



//HRESULT body_::InitializeDefaultSensor()
//	{
//	HRESULT hr;
//
//	hr = GetDefaultKinectSensor(&KinectSensor);
//	if (FAILED(hr))
//		{
//		return hr;
//		}
//
//	if (KinectSensor)
//		{
//		// Initialize the Kinect and get coordinate mapper and the body reader
//		IBodyFrameSource* pBodyFrameSource = NULL;
//
//		hr = KinectSensor->Open();
//
//		if (SUCCEEDED(hr))
//			{
//			hr = KinectSensor->get_CoordinateMapper(&CoordinateMapper);
//			}
//
//		if (SUCCEEDED(hr))
//			{
//			hr = KinectSensor->get_BodyFrameSource(&pBodyFrameSource);
//			}
//
//		if (SUCCEEDED(hr))
//			{
//			hr = pBodyFrameSource->OpenReader(&m_pBodyFrameReader);
//			}
//
//		SafeRelease(pBodyFrameSource);
//		}
//
//	if (!KinectSensor || FAILED(hr))
//		{
//		//SetStatusMessage(L"No ready Kinect found!", 10000, true);
//		return E_FAIL;
//		}
//
//	return hr;
//	}
//*******************************************************************************************************************************
//int body_::Update(float frametime)
//	{
//	int trackedbodies = 0;
//	if (!m_pBodyFrameReader)
//		{
//		return 0;
//		}
//	IBodyFrame* pBodyFrame = NULL;
//
//	HRESULT hr = m_pBodyFrameReader->AcquireLatestFrame(&pBodyFrame);
//
//	if (SUCCEEDED(hr))
//		{
//		//cout << "tracked" << endl;
//		INT64 nTime = 0;
//
//		hr = pBodyFrame->get_RelativeTime(&nTime);
//
//	//	IBody* ppBodies[BODY_COUNT] = { 0 };
//		bodypack_ bodypack;
//
//		if (SUCCEEDED(hr))
//			{
//			hr = pBodyFrame->GetAndRefreshBodyData(_countof(bodypack.ppBodies), bodypack.ppBodies);
//			}
//	
//		for (int i = 0; i < BODY_COUNT; ++i)
//			{
//			IBody* pBody = bodypack.ppBodies[i];
//			if (pBody)
//				{
//				hr = pBody->get_IsTracked(&bodypack.bTracked[i]);
//				if(SUCCEEDED(hr) && bodypack.bTracked[i] == true)
//					trackedbodies++;
//				}
//			}	
//
//			if (SUCCEEDED(hr) && trackedbodies > 0)
//			{
//				ProcessBody(frametime, nTime, BODY_COUNT, &bodypack);
//			}
//			else
//				trackedbody.time_till_last_tracked += frametime;
//		
//		bodypack.saverelease();
//		}
//	/*else
//		cout << "NOT tracked" << endl;*/
//
//	SafeRelease(pBodyFrame);
//
//	//handle timeouts:
//
//	if (trackedbody.time_till_last_tracked > BODY_LOST_TIME)
//		trackedbody.reset();
//
//	return trackedbodies;
//	}
//
////************************************************************************************************************************************************************
//bool body_::ProcessBody(float frametime,INT64 nTime, int nBodyCount, bodypack_* bodypack)
//	{
//	HRESULT hr;
//	if (!CoordinateMapper) return false;
//
//	//first find the closes body:
//	IBody* p_closest_Body_to_track = NULL;
//	float closest = -1;
//	for (int i = 0; i < nBodyCount; ++i)
//		{
//		if (bodypack->ppBodies[i] && bodypack->bTracked[i])
//			{
//			IBody* pBody = bodypack->ppBodies[i];
//
//			Joint joints[JointType_Count];
//
//			HandState leftHandState = HandState_Unknown;
//			HandState rightHandState = HandState_Unknown;
//
//			pBody->get_HandLeftState(&leftHandState);
//			pBody->get_HandRightState(&rightHandState);
//
//			hr = pBody->GetJoints(_countof(joints), joints);
//			if (SUCCEEDED(hr))
//				{
//				if (closest < 0 || closest>joints[0].Position.Z)
//					{
//					closest = joints[0].Position.Z;
//					p_closest_Body_to_track = pBody;
//					}
//				}
//			}
//		}
//	if (!p_closest_Body_to_track)
//		{
//		trackedbody.time_till_last_tracked += frametime;
//		return false;
//		}
//
//	Joint joints[JointType_Count];
//	hr = p_closest_Body_to_track->GetJoints(_countof(joints), joints);
//	if (SUCCEEDED(hr))
//		{		
//		trackedbody.init_tracked = true;
//		trackedbody.time_till_last_tracked = 0;
//		for (int j = 0; j < _countof(joints); ++j)
//			{
//			if (joints[j].TrackingState == TrackingState_NotTracked)
//				{
//				trackedbody.jointTracked[j] = false;
//				trackedbody.time_till_last_joint_tracked[j] += frametime;
//				cout << "lost track on joint: " << j << endl;
//				}
//			else
//				{
//
//				trackedbody.jointTracked[j] = true;
//				trackedbody.time_till_last_joint_tracked[j] = 0;
//				trackedbody.joint_positions[j].x = joints[j].Position.X;
//				trackedbody.joint_positions[j].y = joints[j].Position.Y;
//				trackedbody.joint_positions[j].z = -joints[j].Position.Z;
//				}
//			}
//		trackedbody.cascade();
//		}
//	else
//		trackedbody.time_till_last_tracked += frametime;
	//return true;
	//}
