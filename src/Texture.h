#pragma once
#include "stb_image.h"
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "GLSL.h"

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
	void setColorOrder(GLushort color) { colororder = color; initParams(); }
	void setDataType(GLushort data) { datatype = data; initParams(); }
	void setWrap(GLushort wrap_) { wrap_ = wrap; initParams(); }
	void setMinFilter(GLushort filter) { filter = minfilter; initParams(); }
	void setMagFilter(GLushort filter) { filter = magfilter; initParams(); }

protected:
	GLuint textureID;
	GLushort colortype = GL_RGBA8;
	int width = 0;
	int height = 0;
	GLushort colororder = GL_RGBA;
	GLushort datatype = GL_UNSIGNED_BYTE;
	unsigned char* data = NULL;
	GLushort wrap = GL_CLAMP_TO_BORDER;
	GLushort minfilter = GL_LINEAR_MIPMAP_LINEAR;
	GLushort magfilter = GL_LINEAR;
};

class SimpleTexture2D : public Texture {
public:
	SimpleTexture2D(std::string resource_dir, std::string file) : Texture() {
		file_name = resource_dir + file;
		data = stbi_load(file_name.c_str(), &width, &height, &channels, 4);
		wrap = GL_REPEAT;
	}
	void setFile(std::string file) { file_name = file; data = stbi_load(file_name.c_str(), &width, &height, &channels, 4); initParams(); }

private:
	std::string file_name = "default.png";
	int channels;
};

class TextureArray : public Texture {
public:
	TextureArray(std::string resource_dir, std::string file) : Texture() {
		file_name = resource_dir + file;
		minfilter = GL_LINEAR;
	}
	void initParams();
	void initSubImageParams();
	void setFile(std::string file) { file_name = file; initParams(); }
private:
	std::string file_name = "default.png";
	int channels;
	GLsizei levels;
	GLsizei images_width;
	GLsizei images_height;
	GLsizei num_images;
	GLint mipmapnumber = 0;

};

#endif