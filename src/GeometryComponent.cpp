#include "GeometryComponent.h"


void GeometryComponent::startGPUUpload() {
	vertex_array_object.bind();
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
	this->uploadToGPU();
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

TexturedGeomComponent::TexturedGeomComponent(const std::vector<glm::vec3>& vertices, const std::vector<GLuint>& elements, 
	std::vector<glm::vec3>& normals, std::vector<glm::vec2>& tex_coords) : GeometryComponent(vertices, elements)
{
	vertex_array_object.addBuffer(vertex_texture_buffer);
	vertex_array_object.addBuffer(vertex_normals_buffer);
}


void TexturedGeomComponent::uploadData() {
	GeometryComponent::uploadData();
	if (cpu_normals.size() > 0)
		vertex_normals_buffer.bufferData(3, GL_FLOAT, cpu_normals.size() * sizeof(glm::vec3), cpu_normals.data());
	if (cpu_texture_coords.size() > 0)
		vertex_texture_buffer.bufferData(2, GL_FLOAT, cpu_texture_coords.size() * sizeof(glm::vec2), cpu_texture_coords.data());

}