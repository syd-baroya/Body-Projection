#pragma once
#ifndef RENDERBUFFER_HPP_
#define RENDERBUFFER_HPP_

//#include "Texture.h"
#include "GLSL.h"

class Renderbuffer{
public:
	Renderbuffer(int w, int h) {
		glGenRenderbuffers(1, &rbID);
		this->width = w;
		this->height = h;
	}

	void initParams();
	const GLuint getID() { return(rbID); }
	const int getWidth() { return(width); }
	const int getHeight() { return(height); }
	void deleteBuffer() { glDeleteRenderbuffers(1, &rbID); }
private:
	GLuint rbID;
	int width = 0;
	int height = 0;
};

#endif