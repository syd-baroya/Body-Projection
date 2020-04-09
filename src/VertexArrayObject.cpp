#include "VertexArrayObject.h"

void VertexArrayObject::bind()
{
	glBindVertexArray(id);
}

void VertexArrayObject::unbind()
{
	glBindVertexArray(0);
}

void VertexArrayObject::enableVertexAttribArray(GLuint index)
{
	glEnableVertexAttribArray(index);

}

void VertexArrayObject::disableVertexAttribArray(GLuint index)
{
	glDisableVertexAttribArray(index);
}

void VertexArrayObject::setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
{
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);

}
