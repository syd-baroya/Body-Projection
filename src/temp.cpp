//
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
//	return true;
//	}


//void generate_body_vertices(body_ *body, vector<vec3> *pos)
//{
//
////	cout << body->trackedbody.joint_speed[JointType_WristLeft].y << endl;
//
//	float forecastfact = FORECASTFACT;
//	float z_base = body->trackedbody.get_joint(forecastfact,0).z;
//
//	vec3 posi[JointType_Count];
//	for (int ii = 1; ii < JointType_Count; ii++)
//		body->trackedbody.joint_positions[ii].z = z_base;
//
//	
//	vec3 center = body->trackedbody.get_joint(forecastfact,0);
//	//cout << center.x << endl;
//	center.z = 0;
//	/*for (int ii = 0; ii < JointType_Count; ii++)
//		body->trackedbody.get_joint(forecastfact,ii] -= center;
//*/
//	/*body->trackedbody.get_joint(forecastfact,JointType_ElbowLeft].y += (body->trackedbody.get_joint(forecastfact,JointType_ShoulderLeft].y - body->trackedbody.get_joint(forecastfact,JointType_ElbowLeft].y)*2.0f;
//	body->trackedbody.get_joint(forecastfact,JointType_ElbowRight].y +=( body->trackedbody.get_joint(forecastfact,JointType_ShoulderRight].y - body->trackedbody.get_joint(forecastfact,JointType_ElbowRight].y) * 2.0f;
//
//	body->trackedbody.get_joint(forecastfact,JointType_WristLeft].y += (body->trackedbody.get_joint(forecastfact,JointType_ShoulderLeft].y - body->trackedbody.get_joint(forecastfact,JointType_WristLeft].y) * 2.0f;
//	body->trackedbody.get_joint(forecastfact,JointType_WristRight].y += (body->trackedbody.get_joint(forecastfact,JointType_ShoulderRight].y - body->trackedbody.get_joint(forecastfact,JointType_WristRight].y) * 2.0f;
//
//	body->trackedbody.get_joint(forecastfact,JointType_HandTipLeft].y += (body->trackedbody.get_joint(forecastfact,JointType_ShoulderLeft].y - body->trackedbody.get_joint(forecastfact,JointType_HandTipLeft].y) * 2.0f;
//	body->trackedbody.get_joint(forecastfact,JointType_HandTipRight].y += (body->trackedbody.get_joint(forecastfact,JointType_ShoulderRight].y - body->trackedbody.get_joint(forecastfact,JointType_HandTipRight].y) * 2.0f;*/
//
//	
//	for (int ii = 1; ii < JointType_Count; ii++)
//	{	
//		posi[ii] = body->trackedbody.get_joint(forecastfact,ii);
//		posi[ii].z = 0;
//
//	}
//	float throat_width = 0.3;
//	
//	float torso_width_left = length(body->trackedbody.get_joint(forecastfact,JointType_ShoulderLeft).x - body->trackedbody.get_joint(forecastfact,JointType_SpineShoulder).x);
//	float torso_width_right = length(body->trackedbody.get_joint(forecastfact,JointType_ShoulderRight).x - body->trackedbody.get_joint(forecastfact,JointType_SpineShoulder).x);
//	float torso_above_sholders = length(body->trackedbody.get_joint(forecastfact,JointType_SpineShoulder).y - body->trackedbody.get_joint(forecastfact,JointType_Neck).y);
//	float arm_thickness = torso_above_sholders * 0.9;
//	float leg_thickness = max(torso_width_left, torso_width_right) * 0.7;
//	float foot_thickness = leg_thickness * 0.5;
//	float head_thickness = torso_above_sholders * 1.5;
//
//	/*arm_thickness = torso_above_sholders * 0.6;
//	leg_thickness = max(torso_width_left, torso_width_right) * 0.7;
//	foot_thickness = leg_thickness * 0.8;
//	head_thickness = torso_above_sholders * 1.0;*/
//
//	float chinstart = 0.5;
//	float sidechinstart = 0.75;
//	//torso
//	vec3 utl = normalize(
//		normalize(body->trackedbody.get_joint(forecastfact,JointType_SpineShoulder)- body->trackedbody.get_joint(forecastfact,JointType_ShoulderLeft)) +
//		normalize(body->trackedbody.get_joint(forecastfact,JointType_ElbowLeft) - body->trackedbody.get_joint(forecastfact,JointType_ShoulderLeft))) *
//		torso_above_sholders;
//	float s= sign(cross(posi[JointType_SpineShoulder] - posi[JointType_ShoulderLeft],
//				  posi[JointType_ElbowLeft] - posi[JointType_ShoulderLeft]).z);
//	utl *= s;
//	/*cout << s << endl;
//	cout << normalize(posi[JointType_SpineShoulder] - posi[JointType_ShoulderLeft]).x << ", " << normalize(posi[JointType_SpineShoulder] - posi[JointType_ShoulderLeft]).y << "... " <<
//		normalize(posi[JointType_ElbowLeft] - posi[JointType_ShoulderLeft]).x << ", " << normalize(posi[JointType_ElbowLeft] - posi[JointType_ShoulderLeft]).y << ", :: " << s << endl;*/
//	utl = utl + body->trackedbody.get_joint(forecastfact,JointType_ShoulderLeft);
//
//	vec3 utr = normalize(
//		normalize(body->trackedbody.get_joint(forecastfact,JointType_SpineShoulder) - body->trackedbody.get_joint(forecastfact,JointType_ShoulderRight)) +
//		normalize(body->trackedbody.get_joint(forecastfact,JointType_ElbowRight) - body->trackedbody.get_joint(forecastfact,JointType_ShoulderRight))) *
//		torso_above_sholders;
//	utr *= -sign(cross(body->trackedbody.get_joint(forecastfact,JointType_SpineShoulder) - body->trackedbody.get_joint(forecastfact,JointType_ShoulderRight),
//		body->trackedbody.get_joint(forecastfact,JointType_ElbowRight) - body->trackedbody.get_joint(forecastfact,JointType_ShoulderRight)).z);
//	utr = utr + body->trackedbody.get_joint(forecastfact,JointType_ShoulderRight);
//
//	vec3 mtl = normalize(
//		normalize(body->trackedbody.get_joint(forecastfact,JointType_HipLeft) - body->trackedbody.get_joint(forecastfact,JointType_ShoulderLeft)) +
//		normalize(body->trackedbody.get_joint(forecastfact,JointType_ElbowLeft) - body->trackedbody.get_joint(forecastfact,JointType_ShoulderLeft))) *
//		torso_above_sholders;
//	mtl *= -sign(cross(body->trackedbody.get_joint(forecastfact,JointType_HipLeft) - body->trackedbody.get_joint(forecastfact,JointType_ShoulderLeft),
//		body->trackedbody.get_joint(forecastfact,JointType_ElbowLeft) - body->trackedbody.get_joint(forecastfact,JointType_ShoulderLeft)).z);
//	mtl = mtl + body->trackedbody.get_joint(forecastfact,JointType_ShoulderLeft);
//
//	vec3 mtr = normalize(
//		normalize(body->trackedbody.get_joint(forecastfact,JointType_HipRight) - body->trackedbody.get_joint(forecastfact,JointType_ShoulderRight)) +
//		normalize(body->trackedbody.get_joint(forecastfact,JointType_ElbowRight) - body->trackedbody.get_joint(forecastfact,JointType_ShoulderRight))) *
//		torso_above_sholders;
//	mtr *= sign(cross(body->trackedbody.get_joint(forecastfact,JointType_HipRight) - body->trackedbody.get_joint(forecastfact,JointType_ShoulderRight),
//		body->trackedbody.get_joint(forecastfact,JointType_ElbowRight) - body->trackedbody.get_joint(forecastfact,JointType_ShoulderRight)).z);
//	mtr = mtr + body->trackedbody.get_joint(forecastfact,JointType_ShoulderRight);
//
//	vec3 mll = body->trackedbody.get_joint(forecastfact,JointType_SpineBase) + normalize(body->trackedbody.get_joint(forecastfact,JointType_HipLeft) - body->trackedbody.get_joint(forecastfact,JointType_SpineBase)) * torso_width_left;
//	vec3 mlr = body->trackedbody.get_joint(forecastfact,JointType_SpineBase) + normalize(body->trackedbody.get_joint(forecastfact,JointType_HipRight) - body->trackedbody.get_joint(forecastfact,JointType_SpineBase)) * torso_width_left;
//
//	vec3 sll = body->trackedbody.get_joint(forecastfact,JointType_Neck) + normalize(utl - body->trackedbody.get_joint(forecastfact,JointType_Neck)) * torso_width_left * throat_width;
//	vec3 slr = body->trackedbody.get_joint(forecastfact,JointType_Neck) + normalize(utr - body->trackedbody.get_joint(forecastfact,JointType_Neck)) * torso_width_right * throat_width;
//	//vec3 mtc = (mtl + mtr) / (float)2.;
//	//construct torso:
//	pos->push_back(mll);
//	pos->push_back(mlr);
//	pos->push_back(mtl);
//	pos->push_back(mtr);
//	pos->push_back(body->trackedbody.get_joint(forecastfact,JointType_ShoulderLeft));
//	pos->push_back(body->trackedbody.get_joint(forecastfact,JointType_SpineShoulder));
//	pos->push_back(body->trackedbody.get_joint(forecastfact,JointType_ShoulderRight));
//	pos->push_back(utl);
//	pos->push_back(body->trackedbody.get_joint(forecastfact,JointType_Neck));
//	pos->push_back(utr);
//	pos->push_back(sll);
//	pos->push_back(slr);
//
//	
//	//arms:
//
//	vec3 a, b;
//	a = normalize(body->trackedbody.get_joint(forecastfact,JointType_ShoulderLeft) - body->trackedbody.get_joint(forecastfact,JointType_ElbowLeft));
//	b = normalize(body->trackedbody.get_joint(forecastfact,JointType_WristLeft) - body->trackedbody.get_joint(forecastfact,JointType_ElbowLeft));
//
//	vec3 ell = normalize(a + b);
//	ell *= arm_thickness;
//	ell *= sign(cross(posi[JointType_ShoulderLeft] - posi[JointType_ElbowLeft],
//		posi[JointType_WristLeft] - posi[JointType_ElbowLeft]).z);
//	vec3 hll = body->trackedbody.get_joint(forecastfact,JointType_WristLeft) + ell;
//	vec3 hlr = body->trackedbody.get_joint(forecastfact,JointType_WristLeft)- ell;
//	vec3 elr = body->trackedbody.get_joint(forecastfact,JointType_ElbowLeft) - ell;
//	ell = body->trackedbody.get_joint(forecastfact,JointType_ElbowLeft) + ell;
//	
//	vec3 err = normalize(normalize(body->trackedbody.get_joint(forecastfact,JointType_ShoulderRight) - body->trackedbody.get_joint(forecastfact,JointType_ElbowRight)) +
//		normalize(body->trackedbody.get_joint(forecastfact,JointType_WristRight) - body->trackedbody.get_joint(forecastfact,JointType_ElbowRight))) *
//		arm_thickness;
//
//	err *= -sign(cross(posi[JointType_ShoulderRight] - posi[JointType_ElbowRight],
//		posi[JointType_WristRight] - posi[JointType_ElbowRight]).z);
//
//	vec3 hrr = body->trackedbody.get_joint(forecastfact,JointType_WristRight) + err;
//	vec3 hrl = body->trackedbody.get_joint(forecastfact,JointType_WristRight) - err;
//	vec3 erl = body->trackedbody.get_joint(forecastfact,JointType_ElbowRight) - err;
//	err = body->trackedbody.get_joint(forecastfact,JointType_ElbowRight) + err;
//
//	
//	//construct arms:
//	pos->push_back(hll);
//	pos->push_back(hlr);
//	pos->push_back(ell);
//	pos->push_back(elr);
//
//	pos->push_back(hrl);
//	pos->push_back(hrr);
//	pos->push_back(erl);
//	pos->push_back(err);
//
//	
//
//	//legs
//	vec3 fut = body->trackedbody.get_joint(forecastfact,JointType_SpineBase) +
//		normalize(body->trackedbody.get_joint(forecastfact,JointType_SpineBase) - body->trackedbody.get_joint(forecastfact,JointType_SpineMid)) *	torso_above_sholders;
//
//	vec3 kll = normalize(
//		normalize(body->trackedbody.get_joint(forecastfact,JointType_HipLeft) - body->trackedbody.get_joint(forecastfact,JointType_KneeLeft)) +
//		normalize(body->trackedbody.get_joint(forecastfact,JointType_AnkleLeft) - body->trackedbody.get_joint(forecastfact,JointType_KneeLeft))) *
//		leg_thickness;
//	kll *= sign(cross(posi[JointType_HipLeft] - posi[JointType_KneeLeft],
//		posi[JointType_AnkleLeft] - posi[JointType_KneeLeft]).z);
//	vec3 klr = body->trackedbody.get_joint(forecastfact,JointType_KneeLeft) - kll;
//	kll = body->trackedbody.get_joint(forecastfact,JointType_KneeLeft) + kll;
//
//	vec3 krr = normalize(
//		normalize(body->trackedbody.get_joint(forecastfact,JointType_HipRight) - body->trackedbody.get_joint(forecastfact,JointType_KneeRight)) +
//		normalize(body->trackedbody.get_joint(forecastfact,JointType_AnkleRight) - body->trackedbody.get_joint(forecastfact,JointType_KneeRight))) *
//		leg_thickness;
//	krr *= sign(cross(posi[JointType_HipRight] - posi[JointType_KneeRight],
//		posi[JointType_AnkleRight] - posi[JointType_KneeRight]).z);
//	vec3 krl = body->trackedbody.get_joint(forecastfact,JointType_KneeRight) - krr;
//	krr = body->trackedbody.get_joint(forecastfact,JointType_KneeRight) + krr;
//
//	vec3 all = normalize(
//		normalize(body->trackedbody.get_joint(forecastfact,JointType_KneeLeft) - body->trackedbody.get_joint(forecastfact,JointType_AnkleLeft)) +
//		normalize(body->trackedbody.get_joint(forecastfact,JointType_FootLeft) - body->trackedbody.get_joint(forecastfact,JointType_AnkleLeft))) *
//		foot_thickness;
//	all *= sign(cross(posi[JointType_KneeLeft] - posi[JointType_AnkleLeft],
//		posi[JointType_FootLeft] - posi[JointType_AnkleLeft]).z);
//	vec3 fll = body->trackedbody.get_joint(forecastfact,JointType_FootLeft) + all;
//	vec3 flr = body->trackedbody.get_joint(forecastfact,JointType_FootLeft) - all;
//	vec3 alr = body->trackedbody.get_joint(forecastfact,JointType_AnkleLeft) - all;
//	all = body->trackedbody.get_joint(forecastfact,JointType_AnkleLeft) + all;
//
//	vec3 arr = normalize(
//		normalize(body->trackedbody.get_joint(forecastfact,JointType_KneeRight) - body->trackedbody.get_joint(forecastfact,JointType_AnkleRight)) +
//		normalize(body->trackedbody.get_joint(forecastfact,JointType_FootRight) - body->trackedbody.get_joint(forecastfact,JointType_AnkleRight))) *
//		foot_thickness;
//	arr *= sign(cross(posi[JointType_KneeRight] - posi[JointType_AnkleRight],
//		posi[JointType_FootRight] - posi[JointType_AnkleRight]).z);
//	vec3 frr = body->trackedbody.get_joint(forecastfact,JointType_FootRight) + arr;
//	vec3 frl = body->trackedbody.get_joint(forecastfact,JointType_FootRight) - arr;
//	vec3 arl = body->trackedbody.get_joint(forecastfact,JointType_AnkleRight) - arr;
//	arr = body->trackedbody.get_joint(forecastfact,JointType_AnkleRight) + arr;
//
//	
//	//construct legs
//	pos->push_back(fll);
//	pos->push_back(flr);
//	pos->push_back(all);
//	pos->push_back(alr);
//	pos->push_back(kll);
//	pos->push_back(klr);
//
//	pos->push_back(fut);
//
//	pos->push_back(frl);
//	pos->push_back(frr);
//	pos->push_back(arl);
//	pos->push_back(arr);
//	pos->push_back(krl);
//	pos->push_back(krr);
//
//	
//	//head/throat:
//	vec3 chin = body->trackedbody.get_joint(forecastfact,JointType_Neck)+ (body->trackedbody.get_joint(forecastfact,JointType_Head) - body->trackedbody.get_joint(forecastfact,JointType_Neck)) * chinstart;
//	vec3 sidechincenter = body->trackedbody.get_joint(forecastfact,JointType_Neck) + (body->trackedbody.get_joint(forecastfact,JointType_Head) - body->trackedbody.get_joint(forecastfact,JointType_Neck)) * sidechinstart;
//	vec3 tophead = body->trackedbody.get_joint(forecastfact,JointType_Head) + (body->trackedbody.get_joint(forecastfact,JointType_Head) - body->trackedbody.get_joint(forecastfact,JointType_Neck)) * chinstart;
//
//	vec3 gl = normalize(
//		normalize(body->trackedbody.get_joint(forecastfact,JointType_Head) - body->trackedbody.get_joint(forecastfact,JointType_Neck)) +
//		normalize(body->trackedbody.get_joint(forecastfact,JointType_SpineShoulder) - body->trackedbody.get_joint(forecastfact,JointType_Neck))) *
//		head_thickness;
//	gl *= sign(cross(posi[JointType_Head] - posi[JointType_Neck],
//		posi[JointType_SpineShoulder] - posi[JointType_Neck]).z);
//
//	vec3 hl = tophead + gl;
//	vec3 hr = tophead - gl;
//	vec3 gr = sidechincenter - gl;
//	gl = sidechincenter + gl;
//	//contruct head
//	pos->push_back(chin);
//	pos->push_back(gl);
//	pos->push_back(gr);
//	pos->push_back(hl);
//	pos->push_back(hr);
//
//	
//
//	//hands
//
//
//	vec3 tll = normalize(
//		normalize(body->trackedbody.get_joint(forecastfact,JointType_ElbowLeft) - body->trackedbody.get_joint(forecastfact,JointType_WristLeft)) +
//		normalize(body->trackedbody.get_joint(forecastfact,JointType_HandTipLeft) - body->trackedbody.get_joint(forecastfact,JointType_WristLeft))) *
//		foot_thickness;
//	tll *= sign(cross(posi[JointType_ElbowLeft] - posi[JointType_WristLeft],
//				posi[JointType_ElbowLeft] - posi[JointType_WristLeft]).z);
//
//
//
//	vec3 tlr = body->trackedbody.get_joint(forecastfact,JointType_HandTipLeft) - tll;
//	tll = body->trackedbody.get_joint(forecastfact,JointType_HandTipLeft) + tll;
//
//	vec3 trr = normalize(
//		normalize(body->trackedbody.get_joint(forecastfact,JointType_ElbowRight)- body->trackedbody.get_joint(forecastfact,JointType_WristRight)) +
//		normalize(body->trackedbody.get_joint(forecastfact,JointType_HandTipRight) - body->trackedbody.get_joint(forecastfact,JointType_WristRight))) *
//		foot_thickness;
//	trr *= -sign(cross(posi[JointType_ElbowRight] - posi[JointType_WristRight],
//				posi[JointType_HandTipRight] - posi[JointType_WristRight]).z);
//	vec3 trl = body->trackedbody.get_joint(forecastfact,JointType_HandTipRight) - trr;
//	trr = body->trackedbody.get_joint(forecastfact,JointType_HandTipRight) + trr;
//	pos->push_back(tll);
//	pos->push_back(tlr);
//	pos->push_back(trl);
//	pos->push_back(trr);
//

//indices.push_back(0);	indices.push_back(2);	indices.push_back(3);
//indices.push_back(0);	indices.push_back(3);	indices.push_back(1);

//indices.push_back(2);	indices.push_back(4);	indices.push_back(5);
//indices.push_back(2);	indices.push_back(5);	indices.push_back(3);
//indices.push_back(5);	indices.push_back(6);	indices.push_back(3);
//indices.push_back(4);	indices.push_back(7);	indices.push_back(10);
//indices.push_back(4);	indices.push_back(10);	indices.push_back(5);
//indices.push_back(10);	indices.push_back(8);	indices.push_back(5);
//indices.push_back(8);	indices.push_back(11);	indices.push_back(5);
//indices.push_back(5);	indices.push_back(11);	indices.push_back(6);
//indices.push_back(6);	indices.push_back(11);	indices.push_back(9);
