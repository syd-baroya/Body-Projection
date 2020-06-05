#include "SceneComponent.h"



void SceneComponent::init()
{
	if (add_tones)
	{
		red_tone = normalize(vec3(frand(), frand(), frand()));
		green_tone = normalize(vec3(frand(), frand(), frand()));
		blue_tone = normalize(vec3(frand(), frand(), frand()));

		color_scaling = normalize(vec3(frand(), frand(), frand()));
		red_tone *= color_scaling.x;
		green_tone *= color_scaling.y;
		blue_tone *= color_scaling.z;
	}
}

void SceneComponent::update(double frametime)
{
	if (this->effect_time >= 16.0f)
		this->effect_time = 0.0f;
	this->effect_time += frametime;

}


void SceneComponent::draw(Program* prog)
{
	glUniform1f(prog->getUniform("totaltime"), this->effect_time);
	if (has_offset)
	{
		
		tileprogress = (this->effect_time / 16.0f)* (offset.x*offset.y);
		int tx = (int)tileprogress % offset.x;
		int ty = (int)tileprogress / offset.y;
		vec4 texoff = vec4(offset.x, offset.y, tx, ty);
		glUniform4fv(prog->getUniform("texoffset"), 1, &texoff.x);

	}
	if (add_tones)
	{
		glUniform1f(prog->getUniform("texblend"), 0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glUniform3fv(prog->getUniform("redmul"), 1, &red_tone.x);
		glUniform3fv(prog->getUniform("greenmul"), 1, &green_tone.x);
		glUniform3fv(prog->getUniform("bluemul"), 1, &blue_tone.x);
	}
	this->activateTexture(prog);
}

void SceneComponent::activateTexture(Program* prog)
{
	//scene_texture->uploadToGPU(prog->getPID(), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, scene_texture->getTextureID());
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void SceneComponent::setOffset(int length_of_x_anims, int length_of_y_anims)
{
	offset.x = length_of_x_anims;
	offset.y = length_of_y_anims;
	has_offset = true;
}

void SceneComponent::pauseAll()
{
	if (!is_paused)
		is_paused = true;
}

void SceneComponent::unpauseAll()
{
	if (is_paused)
		is_paused = false;
}

void SceneComponent::reset()
{
}



void FurScene::init()
{
	SceneComponent::init();
	tex1 = -1;
	tex2 = -1;
	actual_redtone = this->red_tone;
	actual_greentone = this->green_tone;
	actual_bluetone = this->blue_tone;

}

void FurScene::update(double frametime)
{
	
	static vec3 next_redtone = vec3(1, 0, 0);
	static vec3 next_greentone = vec3(0, 1, 0);
	static vec3 next_bluetone = vec3(0, 0, 1);
	
	if (this->effect_time < 0.001)
	{
		tex1 = rand() % FURMAXTEX;
		tex2 = rand() % FURMAXTEX;
		next_redtone = normalize(vec3(frand(), frand(), frand()));
		next_greentone = normalize(vec3(frand(), frand(), frand()));
		next_bluetone = normalize(vec3(frand(), frand(), frand()));
	}
	else if (this->effect_time > FURCHANGETIME)
	{
		this->effect_time = 0.001;
		tex1 = tex2;
		tex2 = rand() % FURMAXTEX;
		this->red_tone = next_redtone;
		this->green_tone = next_greentone;
		this->blue_tone = next_bluetone;
		next_redtone = normalize(vec3(frand(), frand(), frand()));
		next_greentone = normalize(vec3(frand(), frand(), frand()));
		next_bluetone = normalize(vec3(frand(), frand(), frand()));
	}
	else if (this->effect_time > FURBLENDTIME)
	{
		blend = this->effect_time - FURBLENDTIME;
		blend /= FURCHANGETIME - FURBLENDTIME;
		if (blend > 1.0)	blend = 1.0;
		
		actual_redtone = mix(this->red_tone, next_redtone, blend);
		actual_greentone = mix(this->green_tone, next_greentone, blend);
		actual_bluetone = mix(this->blue_tone, next_bluetone, blend);
	}
	SceneComponent::update(frametime);


}

void FurScene::draw(Program* prog)
{
	if (this->effect_time > FURBLENDTIME)
		glUniform1f(prog->getUniform("texblend"), blend);
	
	glUniform3fv(prog->getUniform("redmul"), 1, &actual_redtone.x);
	glUniform3fv(prog->getUniform("greenmul"), 1, &actual_greentone.x);
	glUniform3fv(prog->getUniform("bluemul"), 1, &actual_bluetone.x);
	/*	glUniform3fv(progbody->getUniform("redmul"), 1, &redtone.x);
		glUniform3fv(progbody->getUniform("bluemul"), 1, &greentone.x);
		glUniform3fv(progbody->getUniform("greenmul"), 1, &bluetone.x);*/
	glActiveTexture(GL_TEXTURE0);		glBindTexture(GL_TEXTURE_2D ,this->scene_texture[tex1].getTextureID());
	glActiveTexture(GL_TEXTURE1);		glBindTexture(GL_TEXTURE_2D, this->scene_texture[tex2].getTextureID());
	
}


//void SkeletonHeartScene::draw(Program* prog)
//{
//	TrackedBodyEntity* tracked_body_entity = dynamic_cast<TrackedBodyEntity*>(entity);
//
//	float tileprogress = this->effect_time * 16.;
//	int tx = (int)tileprogress % 4;
//	int ty = (int)tileprogress / 4;
//	vec4 texoff = vec4(4, 4, tx, ty);
//	vec3 a = tracked_body_entity->getJoint(FORECASTFACT, K4ABT_JOINT_SHOULDER_LEFT);
//	vec3 b = tracked_body_entity->getJoint(FORECASTFACT, K4ABT_JOINT_SPINE_CHEST);
//	vec3 pos;
//	pos.x = a.x * 0.2 + b.x * 0.8;
//	pos.y = a.y * 0.7 + b.y * 0.3;
//	pos.z = a.z;
//	mat4 MrectHeart = translate(mat4(1), pos) * rotate(mat4(1), 3.14159265f, vec3(0, 1, 0)) * scale(mat4(1), vec3(0.4, 0.4, 0.4));
//	render_rect(P, V, heart_texture, MrectHeart, texoff);
//
//
//
//	mat4 MrectHead = translate(mat4(1), tracked_body_entity->getJoint(FORECASTFACT, K4ABT_JOINT_NOSE) * 0.6f + tracked_body_entity->getJoint(FORECASTFACT, K4ABT_JOINT_HEAD) * 0.6f) * scale(mat4(1), vec3(0.61, 0.61, 0.61));
//	texoff = vec4(1, 1, 0, 0);
//	render_rect(P, V, scene_texture, MrectHead, texoff);
//	this->red_tone = vec3(1, 0, 0);
//	this->green_tone = vec3(0, 1, 0);
//	this->blue_tone = vec3(0, 0, 1);
//
//	SceneComponent::draw(prog);
//}
//
//void ButterflyScene::init()
//{
//	SceneComponent::init();
//
//	for (int ii = 0; ii < BUTTERFLYCOUNT; ii++)
//	{
//		butterfly[ii].rotz = frand() * 3.1415926 * 2.0;
//		butterfly[ii].scale = frand() * 0.05;
//		butterfly[ii].red = vec3(frand(), frand(), frand());
//		butterfly[ii].green = vec3(frand(), frand(), frand());
//		butterfly[ii].blue = vec3(frand(), frand(), frand());
//		butterfly[ii].startanim = 1 + frand() * 4;
//	}
//
//	//torso
//	int c = 13;
//	butterfly[c].iA = 20;		butterfly[c].iB = 12;		butterfly[c].scale += 0.13;		butterfly[c].rationAB = 0.6; c++;
//	butterfly[c].iA = 1;		butterfly[c].iB = 8;		butterfly[c].scale += 0.07;		butterfly[c].rationAB = 0.3; c++;
//	butterfly[c].iA = 1;		butterfly[c].iB = 8;		butterfly[c].scale += 0.05;		butterfly[c].rationAB = 0.9; c++;
//	butterfly[c].iA = 20;		butterfly[c].iB = 8;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.2; c++;
//	butterfly[c].iA = 4;		butterfly[c].iB = 1;		butterfly[c].scale += 0.06;		butterfly[c].rationAB = 0.7; c++;
//	butterfly[c].iA = 4;		butterfly[c].iB = 20;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.5; c++;
//	//head:
//	butterfly[c].iA = 3;		butterfly[c].iB = 2;		butterfly[c].scale += 0.06;		butterfly[c].rationAB = 0.4; c++;
//	//hip
//	butterfly[c].iA = 16;		butterfly[c].iB = 16;		butterfly[c].scale += 0.04;		butterfly[c].rationAB = 0.5; c++;
//	//rightleg
//	butterfly[c].iA = 16;		butterfly[c].iB = 17;		butterfly[c].scale += 0.07;		butterfly[c].rationAB = 0.5; c++;
//	butterfly[c].iA = 17;		butterfly[c].iB = 18;		butterfly[c].scale += 0.04;		butterfly[c].rationAB = 0.2; c++;
//	butterfly[c].iA = 17;		butterfly[c].iB = 18;		butterfly[c].scale += 0.05;		butterfly[c].rationAB = 0.5; c++;
//	butterfly[c].iA = 17;		butterfly[c].iB = 18;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.9; c++;
//	butterfly[c].iA = 18;		butterfly[c].iB = 19;		butterfly[c].scale += 0.04;		butterfly[c].rationAB = 0.5; c++;
//
//	//leftleg
//	butterfly[c].iA = 12;		butterfly[c].iB = 13;		butterfly[c].scale += 0.06;		butterfly[c].rationAB = 0.6; c++;
//	butterfly[c].iA = 12;		butterfly[c].iB = 13;		butterfly[c].scale += 0.05;		butterfly[c].rationAB = 0.2; c++;
//	butterfly[c].iA = 13;		butterfly[c].iB = 14;		butterfly[c].scale += 0.05;		butterfly[c].rationAB = 0.2;	c++;
//	butterfly[c].iA = 13;		butterfly[c].iB = 14;		butterfly[c].scale += 0.05;		butterfly[c].rationAB = 0.5;	c++;
//	butterfly[c].iA = 14;		butterfly[c].iB = 15;		butterfly[c].scale += 0.02;		butterfly[c].rationAB = 0.8;	c++;
//	butterfly[c].iA = 14;		butterfly[c].iB = 15;		butterfly[c].scale += 0.02;		butterfly[c].rationAB = 0.2;	c++;
//	//0-20 till here:21
//
//	//right hand
//	butterfly[c].iA = 8;		butterfly[c].iB = 9;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.4;	c++;
//	butterfly[c].iA = 8;		butterfly[c].iB = 9;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.2;	c++;
//	butterfly[c].iA = 8;		butterfly[c].iB = 9;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.9;	c++;
//	butterfly[c].iA = 9;		butterfly[c].iB = 10;		butterfly[c].scale += 0.02;		butterfly[c].rationAB = 0.5;	c++;
//	butterfly[c].iA = 10;		butterfly[c].iB = 10;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.0;	c++;
//	butterfly[c].iA = 10;		butterfly[c].iB = 11;		butterfly[c].scale += 0.02;		butterfly[c].rationAB = 0.5;	c++;
//
//	//left hand
//	butterfly[c].iA = 4;		butterfly[c].iB = 5;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.6;	c++;
//	butterfly[c].iA = 5;		butterfly[c].iB = 6;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.3;	c++;
//	butterfly[c].iA = 5;		butterfly[c].iB = 6;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.5;	c++;
//	butterfly[c].iA = 6;		butterfly[c].iB = 7;		butterfly[c].scale += 0.02;		butterfly[c].rationAB = 0.5;	c++;
//	//here 31
//	this->actual_butterfly_size = c;
//}
//
//void ButterflyScene::update(double frametime)
//{
//
//	for (int ii = 0; ii < actual_butterfly_size; ii++)
//	{
//		vec3 pos = mix(tracked_body_entity->getJoint(tracked_body_entity->forecastfact, butterfly[ii].iA), tracked_body_entity->getJoint(tracked_body_entity->forecastfact, butterfly[ii].iB), butterfly[ii].rationAB);
//		mat4 Sc = scale(mat4(1), vec3(butterfly[ii].scale));
//		mat4 Rz = rotate(mat4(1), butterfly[ii].rotz, vec3(0, 0, 1));
//		vec3 t = tracked_body_entity->getJoint(tracked_body_entity->forecastfact, ii);
//		if (butterfly[ii].startanim >= (-1))
//			butterfly[ii].startanim -= frametime;
//		if (butterfly[ii].startanim <= 0)
//		{
//			float animprogress = -butterfly[ii].startanim * 2.;
//			if (animprogress >= 1.)
//			{
//				animprogress = 1.;
//				texoff = vec4(4, 4, 3, 3);
//			}
//			else
//			{
//				float tileprogress = animprogress * 16.;
//				int tx = (int)tileprogress % 4;
//				int ty = (int)tileprogress / 4;
//				texoff = vec4(4, 4, tx, ty);
//			}
//		}
//	}
//}
//
//void ButterflyScene::draw(Program* prog)
//{
//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//	glUniform1f(prog->getUniform("totaltime"), this->effect_time);
//	this->activateTexture();
//
//	for (int ii = 0; ii < actual_butterfly_size; ii++)
//	{
//
//		glUniform4fv(prog->getUniform("texsplit"), 1, &texoff.x);
//		glUniform3fv(prog->getUniform("redmul"), 1, &butterfly[ii].red.x);
//		glUniform3fv(prog->getUniform("bluemul"), 1, &butterfly[ii].blue.x);
//		glUniform3fv(prog->getUniform("greenmul"), 1, &butterfly[ii].green.x);
//
//		mat4 Mr = M * translate(mat4(1), pos) * Rz * Sc;
//		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &Mr[0][0]);
//		glDrawArrays(GL_TRIANGLES, 0, 6);
//	}
//
//}
