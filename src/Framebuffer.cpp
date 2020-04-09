#include "Framebuffer.h"
#include "HelperFunctions.h"
#include <iostream>

void Framebuffer::attach(Renderbuffer& rb, GLenum attachment)
{
	//Attach depth buffer to FBO
	depth_attachment = rb;
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rb.getTextureID());
}

void Framebuffer::attach(FramebufferObject& fbo, GLenum attachment, int level)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbID);
	color_attachments.emplace(attachment, fbo);
	buffers.push_back(attachment);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, fbo.getTextureID(), level);
}

void Framebuffer::setDrawBuffers(int num_buffers)
{
	if (num_buffers > buffers.size()) {
		std::cerr << "number of requested buffers is greater than the object's" << std::endl;
		exit(1);
	}
	GLenum* buffer_array;
	if (num_buffers < buffers.size())
		buffer_array = &sub_vector(buffers, 0, num_buffers - 1)[0];
	else
		buffer_array = &buffers[0];

	glBindFramebuffer(GL_FRAMEBUFFER, fbID);
	glDrawBuffers(num_buffers, buffer_array);

}

void Framebuffer::writeToDrawBuffers()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	for (std::pair<GLenum, FramebufferObject> attachment : color_attachments) {
		glBindTexture(GL_TEXTURE_2D, (attachment.second).getTextureID());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}
