#include "GeometryComponent.h"


void GeometryComponent::startGPUUpload() {
	vertex_array_object.bind();
}

void GeometryComponent::init()
{
	this->uploadToGPU();
	this->setupForDraw();
}

void GeometryComponent::draw(Program* prog)
{
	this->startGPUUpload();
	GLuint body_size = getElements().size();
	glDrawElements(GL_TRIANGLES, (int)body_size, GL_UNSIGNED_SHORT, (const void*)0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void GeometryComponent::setupForDraw()
{
	vertex_array_object.setupBuffersToDraw();
}

void GeometryComponent::cleanupAfterDraw()
{
	vertex_array_object.disableBuffersAfterDraw();
}

void GeometryComponent::update(double frametime)
{

}

void GeometryComponent::uploadToGPU()
{
	this->startGPUUpload();
	this->uploadData();
	this->finishGPUUpload();
}

void GeometryComponent::uploadData() {
    vertex_pos_buffer.bufferData(3, GL_FLOAT, cpu_vertices.size() * sizeof(glm::vec3), cpu_vertices.data());
	if(cpu_elements.size()>0)
		vertex_elements_buffer.bufferData(1, GL_SHORT, cpu_elements.size() * sizeof(GLushort), cpu_elements.data());
}

void GeometryComponent::finishGPUUpload() {
	vertex_array_object.unbind();
}

void TexturedGeomComponent::uploadData() {
	GeometryComponent::uploadData();
	/*if (cpu_normals.size() > 0)
		vertex_normals_buffer.bufferData(3, GL_FLOAT, cpu_normals.size() * sizeof(glm::vec3), cpu_normals.data());*/
	if (cpu_texture_coords.size() > 0)
		vertex_texture_buffer.bufferData(2, GL_FLOAT, cpu_texture_coords.size() * sizeof(glm::vec2), cpu_texture_coords.data());

}