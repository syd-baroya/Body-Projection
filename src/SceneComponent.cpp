#include "SceneComponent.h"
#include <glm/glm.hpp>

#include <glm\ext\matrix_transform.hpp>
#include <math.h>


void SceneComponent::init()
{
	red_tone = normalize(vec3(frand(), frand(), frand()));
	green_tone = normalize(vec3(frand(), frand(), frand()));
	blue_tone = normalize(vec3(frand(), frand(), frand()));

	color_scaling = normalize(vec3(frand(), frand(), frand()));
	red_tone *= color_scaling.x;
	green_tone *= color_scaling.y;
	blue_tone *= color_scaling.z;
}

void SceneComponent::update(double frametime)
{

}

void SceneComponent::startDraw(Program* prog, GeometryComponent* geometry, Entity* entity) 
{
	TrackedBodyEntity* tracked_body_entity = dynamic_cast<TrackedBodyEntity*>(entity);

	prog->bind();
	M = glm::translate(glm::mat4(1.0f), tracked_body_entity->modelpos) * glm::scale(glm::mat4(1.0f), tracked_body_entity->modelscale);
	glUniform1f(prog->getUniform("totaltime"), this->effect_time);
	glUniform1f(prog->getUniform("texblend"), 0);
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &P[0][0]);
	glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
	glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glUniform3fv(prog->getUniform("redmul"), 1, &red_tone.x);
	glUniform3fv(prog->getUniform("greenmul"), 1, &green_tone.x);
	glUniform3fv(prog->getUniform("bluemul"), 1, &blue_tone.x);

	geometry->startGPUUpload();
}

void SceneComponent::finishDraw(Program* prog, GeometryComponent* geometry) 
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, (int)geometry->getElements().size(), GL_UNSIGNED_SHORT, (const void*)0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	prog->unbind();
}


void SceneComponent::draw(Program* prog, GeometryComponent* geometry, Entity* entity, double frametime)
{
	this->startDraw(prog, geometry, entity);
	this->finishDraw(prog, geometry);
}

void SceneComponent::activateTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->scene_texture.getTextureID());
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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

void ButterflyScene::init()
{
	SceneComponent::init();

	for (int ii = 0; ii < BUTTERFLYCOUNT; ii++)
	{
		butterfly[ii].rotz = frand() * 3.1415926 * 2.0;
		butterfly[ii].scale = frand() * 0.05;
		butterfly[ii].red = vec3(frand(), frand(), frand());
		butterfly[ii].green = vec3(frand(), frand(), frand());
		butterfly[ii].blue = vec3(frand(), frand(), frand());
		butterfly[ii].startanim = 1 + frand() * 4;
	}

	//torso
	int c = 13;
	butterfly[c].iA = 20;		butterfly[c].iB = 12;		butterfly[c].scale += 0.13;		butterfly[c].rationAB = 0.6; c++;
	butterfly[c].iA = 1;		butterfly[c].iB = 8;		butterfly[c].scale += 0.07;		butterfly[c].rationAB = 0.3; c++;
	butterfly[c].iA = 1;		butterfly[c].iB = 8;		butterfly[c].scale += 0.05;		butterfly[c].rationAB = 0.9; c++;
	butterfly[c].iA = 20;		butterfly[c].iB = 8;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.2; c++;
	butterfly[c].iA = 4;		butterfly[c].iB = 1;		butterfly[c].scale += 0.06;		butterfly[c].rationAB = 0.7; c++;
	butterfly[c].iA = 4;		butterfly[c].iB = 20;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.5; c++;
	//head:
	butterfly[c].iA = 3;		butterfly[c].iB = 2;		butterfly[c].scale += 0.06;		butterfly[c].rationAB = 0.4; c++;
	//hip
	butterfly[c].iA = 16;		butterfly[c].iB = 16;		butterfly[c].scale += 0.04;		butterfly[c].rationAB = 0.5; c++;
	//rightleg
	butterfly[c].iA = 16;		butterfly[c].iB = 17;		butterfly[c].scale += 0.07;		butterfly[c].rationAB = 0.5; c++;
	butterfly[c].iA = 17;		butterfly[c].iB = 18;		butterfly[c].scale += 0.04;		butterfly[c].rationAB = 0.2; c++;
	butterfly[c].iA = 17;		butterfly[c].iB = 18;		butterfly[c].scale += 0.05;		butterfly[c].rationAB = 0.5; c++;
	butterfly[c].iA = 17;		butterfly[c].iB = 18;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.9; c++;
	butterfly[c].iA = 18;		butterfly[c].iB = 19;		butterfly[c].scale += 0.04;		butterfly[c].rationAB = 0.5; c++;

	//leftleg
	butterfly[c].iA = 12;		butterfly[c].iB = 13;		butterfly[c].scale += 0.06;		butterfly[c].rationAB = 0.6; c++;
	butterfly[c].iA = 12;		butterfly[c].iB = 13;		butterfly[c].scale += 0.05;		butterfly[c].rationAB = 0.2; c++;
	butterfly[c].iA = 13;		butterfly[c].iB = 14;		butterfly[c].scale += 0.05;		butterfly[c].rationAB = 0.2;	c++;
	butterfly[c].iA = 13;		butterfly[c].iB = 14;		butterfly[c].scale += 0.05;		butterfly[c].rationAB = 0.5;	c++;
	butterfly[c].iA = 14;		butterfly[c].iB = 15;		butterfly[c].scale += 0.02;		butterfly[c].rationAB = 0.8;	c++;
	butterfly[c].iA = 14;		butterfly[c].iB = 15;		butterfly[c].scale += 0.02;		butterfly[c].rationAB = 0.2;	c++;
	//0-20 till here:21

	//right hand
	butterfly[c].iA = 8;		butterfly[c].iB = 9;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.4;	c++;
	butterfly[c].iA = 8;		butterfly[c].iB = 9;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.2;	c++;
	butterfly[c].iA = 8;		butterfly[c].iB = 9;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.9;	c++;
	butterfly[c].iA = 9;		butterfly[c].iB = 10;		butterfly[c].scale += 0.02;		butterfly[c].rationAB = 0.5;	c++;
	butterfly[c].iA = 10;		butterfly[c].iB = 10;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.0;	c++;
	butterfly[c].iA = 10;		butterfly[c].iB = 11;		butterfly[c].scale += 0.02;		butterfly[c].rationAB = 0.5;	c++;

	//left hand
	butterfly[c].iA = 4;		butterfly[c].iB = 5;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.6;	c++;
	butterfly[c].iA = 5;		butterfly[c].iB = 6;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.3;	c++;
	butterfly[c].iA = 5;		butterfly[c].iB = 6;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.5;	c++;
	butterfly[c].iA = 6;		butterfly[c].iB = 7;		butterfly[c].scale += 0.02;		butterfly[c].rationAB = 0.5;	c++;
	//here 31
	this->actual_butterfly_size = c;
}

void ButterflyScene::draw(Program* prog, GeometryComponent* geometry, Entity* entity, double frametime)
{
	TrackedBodyEntity* tracked_body_entity = dynamic_cast<TrackedBodyEntity*>(entity);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	prog->bind();
	glUniform1f(prog->getUniform("totaltime"), this->effect_time);
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &P[0][0]);
	glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
	glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
	this->activateTexture();
	geometry->startGPUUpload();
	M = glm::translate(glm::mat4(1.0f), tracked_body_entity->modelpos) * glm::scale(glm::mat4(1.0f), tracked_body_entity->modelscale);

	for (int ii = 0; ii < actual_butterfly_size; ii++)
	{
		vec3 pos = mix(tracked_body_entity->getJoint(tracked_body_entity->forecastfact, butterfly[ii].iA), tracked_body_entity->getJoint(tracked_body_entity->forecastfact, butterfly[ii].iB), butterfly[ii].rationAB);
		mat4 Sc = scale(mat4(1), vec3(butterfly[ii].scale));
		mat4 Rz = rotate(mat4(1), butterfly[ii].rotz, vec3(0, 0, 1));
		vec3 t = tracked_body_entity->getJoint(tracked_body_entity->forecastfact, ii);
		if (butterfly[ii].startanim >= (-1))
			butterfly[ii].startanim -= frametime;
		vec4 texoff = vec4(4, 4, 0, 0);
		if (butterfly[ii].startanim <= 0)
		{
			float animprogress = -butterfly[ii].startanim * 2.;
			if (animprogress >= 1.)
			{
				animprogress = 1.;
				texoff = vec4(4, 4, 3, 3);
			}
			else
			{
				float tileprogress = animprogress * 16.;
				int tx = (int)tileprogress % 4;
				int ty = (int)tileprogress / 4;
				texoff = vec4(4, 4, tx, ty);
			}
		}
		glUniform4fv(prog->getUniform("texsplit"), 1, &texoff.x);
		glUniform3fv(prog->getUniform("redmul"), 1, &butterfly[ii].red.x);
		glUniform3fv(prog->getUniform("bluemul"), 1, &butterfly[ii].blue.x);
		glUniform3fv(prog->getUniform("greenmul"), 1, &butterfly[ii].green.x);

		mat4 Mr = M * translate(mat4(1), pos) * Rz * Sc;
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &Mr[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	geometry->finishGPUUpload();
	prog->unbind();
}
