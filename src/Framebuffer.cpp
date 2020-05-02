#include "Framebuffer.h"
#include <iostream>


Framebuffer::Framebuffer(std::vector<std::string> fbos, bool has_depth_buffer, ivec2 screen_size) {
	glGenFramebuffers(1, &fbID);
	this->bind();
	buffers = new GLenum[fbos.size()];
	int count = 0;
	for (std::string fbo_str : fbos)
	{
		FramebufferObject* fbo = new FramebufferObject(screen_size.x, screen_size.y);
		fbo->initParams();
		this->attach(fbo, fbo_str, GL_COLOR_ATTACHMENT0 + count, 0);
		buffers[count] = GL_COLOR_ATTACHMENT0 + count;
		count++;
	}
	if (has_depth_buffer)
	{
		this->depth_attachment = new Renderbuffer(screen_size.x, screen_size.y);
		this->depth_attachment->initParams();
		this->attach(depth_attachment, GL_DEPTH_ATTACHMENT);
	}

	this->unbind();
}

void Framebuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbID);
}

void Framebuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::attach(Renderbuffer* rb, GLenum attachment)
{
	//Attach depth buffer to FBO
	//depth_attachment = rb;
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rb->getID());
}

void Framebuffer::attach(FramebufferObject* fbo, std::string fbo_name, GLenum attachment, int level)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, fbo->getTextureID(), level);

	color_attachments.emplace(fbo_name, fbo);
}

void Framebuffer::setDrawBuffers()
{

	glBindFramebuffer(GL_FRAMEBUFFER, fbID);
	int count = 0;
	for (std::pair < std::string, FramebufferObject* > attachment : color_attachments) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+count, GL_TEXTURE_2D, (attachment.second)->getTextureID(), 0);
		count++;
	}
	glDrawBuffers(count, buffers);

}

void Framebuffer::drawBuffers()
{
	int count = 0;
	for (std::pair < std::string, FramebufferObject* > attachment : color_attachments) {
		glActiveTexture(GL_TEXTURE0+count);
		glBindTexture(GL_TEXTURE_2D,(attachment.second)->getTextureID());
		count++;
	}
}

void Framebuffer::writeToDrawBuffers()
{
	this->unbind();
	for (std::pair < std::string , FramebufferObject* > attachment : color_attachments) {
		glBindTexture(GL_TEXTURE_2D, (attachment.second)->getTextureID());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}

void Framebuffer::deleteBuffer()
{
	glDeleteFramebuffers(1, &fbID);
}
