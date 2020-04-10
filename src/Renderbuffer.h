#pragma once
#ifndef RENDERBUFFER_HPP_
#define RENDERBUFFER_HPP_

#include "Texture.h"

class Renderbuffer{
public:
	Renderbuffer() {
		glGenRenderbuffers(1, &rbID);
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