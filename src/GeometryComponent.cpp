#include "GeometryComponent.h"


void GeometryComponent::startGPUUpload() {
	//vertex_array_object->bind();
	glBindVertexArray(vertex_array_object);
}

void GeometryComponent::init()
{
	this->uploadToGPU();
	//this->setupForDraw();
}

void GeometryComponent::draw(Program* prog)
{
	startGPUUpload();
	GLuint body_size = getElements().size();
	glDrawElements(GL_TRIANGLES, (int)body_size, GL_UNSIGNED_SHORT, (const void*)0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void GeometryComponent::setupForDraw()
{
	//vertex_array_object->setupBuffersToDraw();
}

void GeometryComponent::cleanupAfterDraw()
{
	//vertex_array_object->disableBuffersAfterDraw();
}

void GeometryComponent::update(double frametime)
{
	if (pos_draw_type == GL_DYNAMIC_DRAW) {
		glBindBuffer(GL_ARRAY_BUFFER, vertex_pos_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, cpu_vertices.size() * sizeof(glm::vec3), cpu_vertices.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void GeometryComponent::uploadToGPU()
{
	this->startGPUUpload();
	this->uploadData();
	this->uploadElementData();
	this->finishGPUUpload();
}

void GeometryComponent::uploadData() {
	/*vertex_pos_buffer->bind();
    vertex_pos_buffer->bufferData(3, GL_FLOAT, cpu_vertices.size() * sizeof(glm::vec3), cpu_vertices.data());
	vertex_array_object->enableVertexAttribArray(0);
	vertex_array_object->setVertexAttribPointer(0, vertex_pos_buffer->getNumPerElem(), vertex_pos_buffer->getElemType(), GL_FALSE, 0, (void*)0);*/
	//vertex_pos_buffer.unbind();

	glBindBuffer(GL_ARRAY_BUFFER, vertex_pos_buffer);
	glBufferData(GL_ARRAY_BUFFER, cpu_vertices.size() * sizeof(glm::vec3), cpu_vertices.data(), pos_draw_type);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	
}

void GeometryComponent::uploadElementData() 
{
	/*if (cpu_elements.size() > 0) {
		vertex_elements_buffer->bind();
		vertex_elements_buffer->bufferData(1, GL_SHORT, cpu_elements.size() * sizeof(GLushort), cpu_elements.data());*/
		//vertex_elements_buffer.unbind();}
	if (cpu_elements.size() > 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_elements_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * cpu_elements.size(), cpu_elements.data(), elem_draw_type);
	}
}

void GeometryComponent::finishGPUUpload() {
	//vertex_array_object->unbind();
	glBindVertexArray(0);

}

void TexturedGeomComponent::uploadData() {
	GeometryComponent::uploadData();
	/*if (cpu_normals.size() > 0)
		vertex_normals_buffer.bufferData(3, GL_FLOAT, cpu_normals.size() * sizeof(glm::vec3), cpu_normals.data());*/
	//if (cpu_texture_coords.size() > 0) {
	//vertex_texture_buffer->bind();
	//vertex_texture_buffer->bufferData(2, GL_FLOAT, cpu_texture_coords.size() * sizeof(glm::vec2), cpu_texture_coords.data());
	//vertex_array_object->enableVertexAttribArray(1);
	//vertex_array_object->setVertexAttribPointer(1, vertex_texture_buffer->getNumPerElem(), vertex_texture_buffer->getElemType(), GL_FALSE, 0, (void*)0);
		//vertex_texture_buffer.unbind();}


	glBindBuffer(GL_ARRAY_BUFFER, vertex_texture_buffer);
	glBufferData(GL_ARRAY_BUFFER, cpu_texture_coords.size() * sizeof(glm::vec2), cpu_texture_coords.data(), tex_draw_type);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

}