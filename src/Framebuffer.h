#pragma once
#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

//#include "GLSL.h"
#include "Texture.h"
#include <map>
#include <vector>
#include "Renderbuffer.h"
#include "HelperFunctions.h"

using namespace HelperFunctions;

class FramebufferObject : public Texture {
public:
	FramebufferObject(int w, int h) : Texture() {
		width = w;
		height = h;
	}

};


class Framebuffer {
public:
	Framebuffer() {
		glGenFramebuffers(1, &fbID);
	}

	void attach(Renderbuffer& rb, GLenum attachment);
	void attach(FramebufferObject& fbo, GLenum attachment, int level);
	void setDrawBuffers(int num_buffers);
	void writeToDrawBuffers();
	void deleteBuffer();

private:
	GLuint fbID;
	std::vector<GLenum> buffers;
	std::map<GLenum, FramebufferObject> color_attachments;
	Renderbuffer depth_attachment;
};


#endif