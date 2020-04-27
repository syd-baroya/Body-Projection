#include "Buffers.h"
#include <vcruntime_string.h>


void SimpleBuffer::bufferData(GLuint npe, GLenum type, GLsizeiptr size, const void* data)
{
	num_per_elem = npe;
	elem_type = type;
	bind();
	glBufferData(buffer_type, size, data, draw_type);
	unbind();
}

void SimpleBuffer::bufferSubData(GLuint npe, GLenum type, GLintptr offset, GLsizeiptr size, const void* data)
{
	num_per_elem = npe;
	elem_type = type;
	bind();
	glBufferSubData(buffer_type, offset, size, data);
	unbind();
}

void ComplexBuffer::bufferData(GLsizeiptr size, const void* data)
{
	bind();
	glBufferData(getBufferType(), size, data, GL_DYNAMIC_DRAW);
	bindBufferBase(0);
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

void ShaderStorageBuffer::create_SSBO(ssbo_data& ssbo_data)
{
	for (int i = 0; i < sizeof(ssbo_data); i++) {
		ssbo_data.positions_list[i] = glm::ivec4(i, 0, 0, 0);
	}
	bufferData(sizeof(ssbo_data), &ssbo_data);
}

void ShaderStorageBuffer::get_SSBO_back(ssbo_data& ssbo_data)
{
	// Get SSBO back
	bind();
	GLvoid* p = mapBuffer(GL_READ_ONLY);
	int siz = sizeof(ssbo_data);
	memcpy(&ssbo_data, p, siz);
	unMapBuffer();
}
