#pragma once
#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "GLSL.h"
#include <vector>
#include <iostream>


class Texture {
public:
	Texture() {
		glGenTextures(1, &textureID);
	}

	virtual void initParams();

	const GLuint getTextureID() { return(textureID); }
	const int getWidth() { return(width); }
	const int getHeight() { return(height); }
	const GLushort getColorOrder() { return(colororder); }
	const GLushort getDataType() { return(datatype); }
	const unsigned char* getData() { return(data); }
	const GLushort getWrap() { return(wrap); }
	const GLushort getMinFilter() { return(minfilter); }
	const GLushort getMagFilter() { return(magfilter); }
	void setColorOrder(GLushort color) { colororder = color;}
	void setDataType(GLushort data) { datatype = data; }
	void setWrap(GLushort wrap_) { wrap_ = wrap; }
	void setMinFilter(GLushort filter) { filter = minfilter; }
	void setMagFilter(GLushort filter) { filter = magfilter;}
	void deleteTexture() { glDeleteTextures(1, &textureID); }
protected:
	GLuint textureID;
	GLushort colortype = GL_RGBA8;
	int width = 0;
	int height = 0;
	GLushort colororder = GL_RGBA;
	GLushort datatype = GL_UNSIGNED_BYTE;
	unsigned char* data;
	GLushort wrap = GL_CLAMP_TO_BORDER;
	GLushort minfilter = GL_LINEAR_MIPMAP_LINEAR;
	GLushort magfilter = GL_LINEAR;
};


class SimpleTexture2D : public Texture {
public:
	SimpleTexture2D(){}
	SimpleTexture2D(std::string file);
	void setTexName(const GLchar* name);
	void setFile(std::string file);
	void uploadToGPU(GLuint pid, int location_num);

private:
	char filepath[100];
	int channels;
	const GLchar* tex_name = "tex";
};

class TextureArray : public Texture {
public:
	
	TextureArray(){}
	TextureArray(std::string file) : Texture() {
		strcpy(filepath,  file.c_str());
		minfilter = GL_LINEAR;
	}
	void initParams();
	void initSubImageParams();
	void setImageWidth(GLsizei iwidth) { images_width = iwidth; }
	void setImageHeight(GLsizei iheight) { images_height = iheight; }
	void setLevels(GLsizei lev) { levels = lev; }
	void setNumImages(GLsizei num) { num_images = num; }

	void setFile(std::string file) { strcpy(filepath, file.c_str());}
private:
	char filepath[100];
	int channels;
	GLsizei levels;
	GLsizei images_width;
	GLsizei images_height;
	GLsizei num_images;
	GLint mipmapnumber = 0;

};

#endif