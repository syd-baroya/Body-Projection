#pragma once
#ifndef RENDERBUFFER_HPP_
#define RENDERBUFFER_HPP_

#include "Texture.h"

class Renderbuffer{
public:
	Renderbuffer() {
		glGenRenderbuffers(1, &textureID);
	}

	void initParams();
	const GLuint getTextureID() { return(textureID); }
	const int getWidth() { return(width); }
	const int getHeight() { return(height); }

private:
	GLuint textureID;
	int width = 0;
	int height = 0;
};

#endif