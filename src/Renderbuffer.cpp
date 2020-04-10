#include "Renderbuffer.h"

void Renderbuffer::initParams()
{
	//create depth buffer
	glBindRenderbuffer(GL_RENDERBUFFER, rbID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);
	
}
