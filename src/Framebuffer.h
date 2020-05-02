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
	Framebuffer(){}
	Framebuffer(std::vector<std::string> fbos, bool has_depth_buffer, glm::ivec2 screen_size);

	void bind();
	void unbind();
	void attach(Renderbuffer* rb, GLenum attachment);
	void attach(FramebufferObject* fbo, std::string fbo_name, GLenum attachment, int level);
	void setDrawBuffers();
	void drawBuffers();
	void writeToDrawBuffers();
	void deleteBuffer();
	FramebufferObject* getFBO(std::string name) { return(color_attachments.at(name)); }
private:
	GLuint fbID;
	GLenum* buffers;
	std::map<std::string, FramebufferObject*> color_attachments;
	Renderbuffer* depth_attachment;
};


#endif