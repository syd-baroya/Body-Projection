#pragma once
#ifndef BUFFERS_H
#define BUFFERS_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>

class SimpleBuffer {
public:
	SimpleBuffer(){
		glGenBuffers(1, &id);
		bind();
		unbind();
	}
	SimpleBuffer(GLenum buffer_type, GLenum draw_type) : buffer_type(buffer_type), draw_type(draw_type) {
		glGenBuffers(1, &id);
		//bind();
		//unbind();
	}

	virtual void bind() { glBindBuffer(buffer_type, id); }
	virtual void unbind() { glBindBuffer(buffer_type, 0); }
	inline GLenum getBufferType() const { return(buffer_type); }
	inline GLuint getID() const { return(id); }
	virtual void deleteBuffer() {	glDeleteBuffers(1, &id);	}
	inline GLuint getNumPerElem() { return(num_per_elem); }
	inline GLenum getElemType() { return(elem_type); }

	template<class T>
	void bufferData(GLsizeiptr size, T* data)
	{
		//bind();
		glBufferData(buffer_type, size, data, draw_type);
		//unbind();
	}

	template<class T>
	void bufferSubData(GLintptr offset, GLsizeiptr size, T* data)
	{
		//bind();
		glBufferSubData(buffer_type, offset, size, data);
		//unbind();
	}


protected:
	GLuint num_per_elem = 0;
	GLuint id;
	GLenum elem_type = GL_INVALID_ENUM;
	GLenum buffer_type = GL_INVALID_ENUM;
	GLenum draw_type = GL_INVALID_ENUM;
};

class ComplexBuffer : public SimpleBuffer {
public:
	ComplexBuffer() : SimpleBuffer(){}
	ComplexBuffer(GLenum buffer_type, GLenum draw_type) : SimpleBuffer(buffer_type, draw_type) {}
	auto* mapBufferRange(GLintptr offset, GLsizeiptr length, GLbitfield access);
	GLvoid* mapBuffer(GLenum access);
	void unMapBuffer();
	void bindBufferBase(GLuint index);
	void unbindBufferBase(GLuint index);

	template<class T>
	void bufferData(GLsizeiptr size, T* data)
	{
		//bind();
		glBufferData(getBufferType(), size, data, GL_DYNAMIC_DRAW);
		bindBufferBase(0);
		//unbind();
	}
};

class ArrayBuffer : public SimpleBuffer {
public:
	ArrayBuffer() : SimpleBuffer() {}

	ArrayBuffer(GLenum draw_type) : SimpleBuffer(GL_ARRAY_BUFFER, draw_type) {}

};

class AtomicCounterBuffer : public ComplexBuffer {
public:
	AtomicCounterBuffer() : ComplexBuffer(GL_ATOMIC_COUNTER_BUFFER, GL_DYNAMIC_DRAW){}
	/*void init_atomic(); ------> bufferData()*/
	void reset_atomic();
	void read_atomic();

};

class ElementArrayBuffer : public SimpleBuffer {
public:
	ElementArrayBuffer() : SimpleBuffer() {}
	ElementArrayBuffer(GLenum draw_type) : SimpleBuffer(GL_ELEMENT_ARRAY_BUFFER, draw_type){}
};


class ShaderStorageBuffer : public ComplexBuffer {
public:
	ShaderStorageBuffer() : ComplexBuffer(GL_SHADER_STORAGE_BUFFER, GL_DYNAMIC_DRAW) {}

	void create_SSBO(int total_size)
	{
		this->bind();
		glBufferData(getBufferType(), total_size, NULL, GL_DYNAMIC_DRAW);
		this->unbind();
	}

	template<class T>
	void create_SSBO(T* ssbo_data, int total_size)
	{
		this->bind();
		glBufferData(getBufferType(), total_size, ssbo_data, GL_DYNAMIC_DRAW);
		this->unbind();
	}

	template<class T>
	void load_SSBO(T* ssbo_data, int offset, int total_size)
	{
		this->bind();
		glBufferSubData(getBufferType(), offset, total_size, ssbo_data);
		this->unbind();
	}

	template<class T>
	void get_SSBO_back(T* ssbo_data, int total_size)
	{
		// Get SSBO back
		this->bind();
		GLvoid* p = this->mapBuffer(GL_READ_ONLY);
		memcpy(ssbo_data, p, total_size);
		this->unMapBuffer();
	}
};

class TextureBuffer : public SimpleBuffer {
public:
	TextureBuffer() : SimpleBuffer() {}
	TextureBuffer(GLenum draw_type) : SimpleBuffer(GL_TEXTURE_BUFFER, draw_type){}

	void genTexture();
	void bindTexture();
	void texBuffer();
	void draw();

private:
	GLuint texture;
};

#endif