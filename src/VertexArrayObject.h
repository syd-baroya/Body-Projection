#pragma once
#include <glad\glad.h>
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
	void setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
	void addBuffer(SimpleBuffer buffer);
	void deleteVAO();

private:
	GLuint id;
	std::map<GLuint, SimpleBuffer> buffer_objects;
};

#endif