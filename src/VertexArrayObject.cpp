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

void VertexArrayObject::disableAllVertAttribArr()
{
	this->bind();
	for (size_t i = 0; i < this->array_buffer_objects.size(); i++) {
		glDisableVertexAttribArray(i);
	}
	this->unbind();
}

void VertexArrayObject::setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
{
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);

}

void VertexArrayObject::addBuffer(ArrayBuffer buffer)
{
	this->array_buffer_objects.emplace(this->array_buffer_objects.size(), buffer);
}

void VertexArrayObject::addBuffer(ElementArrayBuffer buffer)
{
	this->element_buffer = buffer;
}

void VertexArrayObject::deleteVAO()
{
	glDeleteVertexArrays(1, &this->id);
}

void VertexArrayObject::setupBuffersToDraw()
{
	bind();
	for(std::pair<GLuint, ArrayBuffer> buffer_pair : this->array_buffer_objects)
	{
		ArrayBuffer buffer = buffer_pair.second;
		GLuint attrib_num = buffer_pair.first;

		buffer.bind();
		enableVertexAttribArray(attrib_num);
		setVertexAttribPointer(attrib_num, buffer.getNumPerElem(), buffer.getElemType(), GL_FALSE, 0, (void*)0);

	}
	this->element_buffer.bind();
	unbind();

}

void VertexArrayObject::disableBuffersAfterDraw()
{
	this->disableAllVertAttribArr();

	this->unbind();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
