#pragma once
#include "Buffers.h"
#include <map>
#ifndef VERTEX_ARRAY_OBJECT_H
#define VERTEX_ARRAY_OBJECT_H

class VertexArrayObject {
public:
	VertexArrayObject() {
		glGenVertexArrays(1, &id);
	}

	void bind();
	void unbind();
	void enableVertexAttribArray(GLuint index);
	void disableVertexAttribArray(GLuint index);
	void disableAllVertAttribArr();
	void setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
	void addBuffer(ArrayBuffer buffer);
	void addBuffer(ElementArrayBuffer buffer);
	void deleteVAO();

	void setupBuffersToDraw();
	void disableBuffersAfterDraw();


private:
	GLuint id;
	std::map<GLuint, ArrayBuffer> array_buffer_objects;
	ElementArrayBuffer element_buffer;
};

#endif