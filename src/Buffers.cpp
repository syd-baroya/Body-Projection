#include "Buffers.h"
#include <vcruntime_string.h>


auto* ComplexBuffer::mapBufferRange(GLintptr offset, GLsizeiptr length, GLbitfield access)
{
	GLuint* userCounters;
	// map the buffer, userCounters will point to the buffers data
	userCounters = (GLuint*)glMapBufferRange(getBufferType(), offset, length, access);
	return userCounters;
}

GLvoid* ComplexBuffer::mapBuffer(GLenum access)
{
	return glMapBuffer(getBufferType(), access);
}

void ComplexBuffer::unMapBuffer()
{
	glUnmapBuffer(getBufferType());
}

void ComplexBuffer::bindBufferBase(GLuint index)
{
	glBindBufferBase(getBufferType(), index, getID());
}

void ComplexBuffer::unbindBufferBase(GLuint index)
{
	glBindBufferBase(getBufferType(), index, 0);
}

void AtomicCounterBuffer::reset_atomic()
{
	GLuint* userCounters;
	bind();
	// map the buffer, userCounters will point to the buffers data

	userCounters = mapBufferRange(0, sizeof(GLuint), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	// set the memory to zeros, resetting the values in the buffer
	memset(userCounters, 0, sizeof(GLuint));
	// unmap the buffer
	unMapBuffer();
}
void  AtomicCounterBuffer::read_atomic()
{
	GLuint* userCounters;
	bind();
	// again we map the buffer to userCounters, but this time for read-only access
	userCounters = mapBufferRange(0, sizeof(GLuint), GL_MAP_READ_BIT);
	// copy the values to other variables because...
	//std::cout << std::endl << *userCounters << std::endl;
	// ... as soon as we unmap the buffer
	// the pointer userCounters becomes invalid.
	unMapBuffer();
}

void TextureBuffer::genTexture()
{
	glGenTextures(1, &texture);
}

void TextureBuffer::bindTexture()
{
	glBindTexture(GL_TEXTURE_BUFFER, texture);
}

void TextureBuffer::texBuffer()
{
	glTexBuffer(GL_TEXTURE_BUFFER, getElemType(), this->getID());
}

void TextureBuffer::draw()
{
	glActiveTexture(GL_TEXTURE0);		
	glBindTexture(GL_TEXTURE_2D, texture);
}
