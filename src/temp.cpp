
bool body_::ProcessBody(float frametime,INT64 nTime, int nBodyCount, bodypack_* bodypack)
	{
	HRESULT hr;
	if (!CoordinateMapper) return false;

	//first find the closes body:
	IBody* p_closest_Body_to_track = NULL;
	float closest = -1;
	for (int i = 0; i < nBodyCount; ++i)
		{
		if (bodypack->ppBodies[i] && bodypack->bTracked[i])
			{
			IBody* pBody = bodypack->ppBodies[i];

			Joint joints[JointType_Count];

			HandState leftHandState = HandState_Unknown;
			HandState rightHandState = HandState_Unknown;

			pBody->get_HandLeftState(&leftHandState);
			pBody->get_HandRightState(&rightHandState);

			hr = pBody->GetJoints(_countof(joints), joints);
			if (SUCCEEDED(hr))
				{
				if (closest < 0 || closest>joints[0].Position.Z)
					{
					closest = joints[0].Position.Z;
					p_closest_Body_to_track = pBody;
					}
				}
			}
		}
	if (!p_closest_Body_to_track)
		{
		trackedbody.time_till_last_tracked += frametime;
		return false;
		}

	Joint joints[JointType_Count];
	hr = p_closest_Body_to_track->GetJoints(_countof(joints), joints);
	if (SUCCEEDED(hr))
		{		
		trackedbody.init_tracked = true;
		trackedbody.time_till_last_tracked = 0;
		for (int j = 0; j < _countof(joints); ++j)
			{
			if (joints[j].TrackingState == TrackingState_NotTracked)
				{
				trackedbody.jointTracked[j] = false;
				trackedbody.time_till_last_joint_tracked[j] += frametime;
				cout << "lost track on joint: " << j << endl;
				}
			else
				{

				trackedbody.jointTracked[j] = true;
				trackedbody.time_till_last_joint_tracked[j] = 0;
				trackedbody.joint_positions[j].x = joints[j].Position.X;
				trackedbody.joint_positions[j].y = joints[j].Position.Y;
				trackedbody.joint_positions[j].z = -joints[j].Position.Z;
				}
			}
		trackedbody.cascade();
		}
	else
		trackedbody.time_till_last_tracked += frametime;
	return true;
	}
