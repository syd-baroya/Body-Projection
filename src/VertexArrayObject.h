#pragma once
#include <glad\glad.h>
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

private:
	GLuint id;
};

#endif