#include "Buffers.h"
#include <vcruntime_string.h>

void SimpleBuffer::addData(GLsizeiptr size, const void* data, GLenum usage)
{
	bind();
	bufferData(size, data, usage);
}

void SimpleBuffer::bufferData(GLsizeiptr size, const void* data, GLenum usage)
{
	bind();
	glBufferData(buffer_type, size, data, usage);
	unbind();
}

void SimpleBuffer::bufferSubData(GLintptr offset, GLsizeiptr size, const void* data)
{
	bind();
	glBufferSubData(buffer_type, offset, size, data);
	unbind();
}

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
	//cout << endl << *userCounters << endl;
	// ... as soon as we unmap the buffer
	// the pointer userCounters becomes invalid.
	unMapBuffer();
}

template <typename T>
void ShaderStorageBuffer::create_SSBO(T& ssbo_data)
{
	for (int i = 0; i < sizeof(T); i++) {
		ssbo_data.positions_list[i] = ivec4(i, 0, 0, 0);
	}

	glGenBuffers(1, &getID());
	bind();
	bufferData(sizeof(T, &ssbo_data, GL_DYNAMIC_DRAW);
	bindBufferBase(0);
	unbind();
}

template <typename T>
void ShaderStorageBuffer::get_SSBO_back(T& ssbo_data)
{
	// Get SSBO back
	bind();
	GLvoid* p = mapBuffer(GL_READ_ONLY);
	int siz = sizeof(T);
	memcpy(&ssbo_data, p, siz);
	unMapBuffer();
}


