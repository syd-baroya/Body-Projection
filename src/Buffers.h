#pragma once
#ifndef BUFFERS_H
#define BUFFERS_H
#include <glad\glad.h>

class SimpleBuffer {
public:
	SimpleBuffer(){
		glGenBuffers(1, &id);
		bind();
		unbind();
	}
	SimpleBuffer(GLenum buffer_type, GLenum draw_type) : buffer_type(buffer_type), draw_type(draw_type) {
		glGenBuffers(1, &id);
		bind();
		unbind();
	}

	virtual void bind() { glBindBuffer(buffer_type, id); }
	virtual void unbind() { glBindBuffer(buffer_type, 0); }
	virtual void bufferData(GLuint npe, GLenum type, GLsizeiptr size, const void* data);
	virtual void bufferSubData(GLuint npe, GLenum type, GLintptr offset, GLsizeiptr size, const void* data);
	inline GLenum getBufferType() const { return(buffer_type); }
	inline GLuint getID() const { return(id); }
	virtual void deleteBuffer() {	glDeleteBuffers(1, &id);	}
	inline GLuint getNumPerElem() { return(num_per_elem); }
	inline GLenum getElemType() { return(elem_type); }
private:
	GLuint num_per_elem;
	GLenum elem_type;
	GLenum buffer_type = GL_INVALID_ENUM;
	GLuint id;
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
	template <typename T>
	void create_SSBO(T& ssbo_data);
	template <typename T>
	void get_SSBO_back(T& ssbo_data);
};
#endif