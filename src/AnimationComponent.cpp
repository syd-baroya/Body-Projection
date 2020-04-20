#include "AnimationComponent.h"


void AnimationComponent::init() 
{
}

void AnimationComponent::update(double phaseprogresstotaltime)
{
	this->phaseprogresstotaltime = phaseprogresstotaltime;
}

void AnimationComponent::draw(Program* prog)
{
	glUniform1f(prog->getUniform("totaltime"), phaseprogresstotaltime);
}

void AnimationComponent::activateTexture()
{
	glActiveTexture(GL_TEXTURE0);		glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);		glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE2);		glBindTexture(GL_TEXTURE_2D, animation_texture.getTextureID());
	glActiveTexture(GL_TEXTURE3);		glBindTexture(GL_TEXTURE_2D_ARRAY, animation_texture_array.getTextureID());
}

void AnimationComponent::pauseAll()
{
}

void AnimationComponent::unpauseAll()
{
}

void AnimationComponent::reset()
{
}

void FireAnimation::init()
{
}

void FireAnimation::update(double frametime)
{
	float tick = phaseprogresstotaltime * 25.;
	int texframe_x = (int)tick % 8;
	int texframe_y = (int)tick / 8;
	texoff.x = (float)texframe_x;
	texoff.y = (float)texframe_y;
	firescale = 100. - pow(phaseprogresstotaltime, 0.06) * 92;
	if (firescale <= 0)
	{
		//reset
		firescale = 0.1;
		//roll_dice();
	}
}

void FireAnimation::draw(Program* prog)
{
	glUniform1f(prog->getUniform("firescale"), firescale);
	glUniform2fv(prog->getUniform("texsplit"), 1, &texoff.x);
}
