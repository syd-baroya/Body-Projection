#pragma once

#ifndef GEOMETRY_COMPONENT_H
#define GEOMETRY_COMPONENT_H

#include "Component.h"
#include "VertexArrayObject.h"
#include <vector>
#include <glm\glm.hpp>

class GeometryComponent : public Component {
public:
	GeometryComponent() {
		vertex_array_object.addBuffer(vertex_pos_buffer);
	}
	GeometryComponent(const std::vector<glm::vec3> vertices, const std::vector<GLushort> elements) {
		cpu_vertices = vertices;
		cpu_elements = elements;
		vertex_array_object.addBuffer(vertex_pos_buffer);
		vertex_array_object.addBuffer(vertex_elements_buffer);
	}

	
	std::vector<glm::vec3>& getMutableVertices() { return(cpu_vertices); }
	std::vector<glm::vec3> getVertices() const { return(cpu_vertices); }
	std::vector<GLuint>& getMutableElements() { return(cpu_elements); }
	std::vector<GLuint> getElements() const { return(cpu_elements); }

	void init() override;
	void draw(Program* prog);
	void setupForDraw();
	void cleanupAfterDraw();
	void update(double frametime) override;
	void startGPUUpload();
	void finishGPUUpload();

protected:
	void uploadToGPU();
	virtual void uploadData();

	VertexArrayObject vertex_array_object;
	ArrayBuffer vertex_pos_buffer;
	ElementArrayBuffer vertex_elements_buffer;

private:
	std::vector<glm::vec3> cpu_vertices;
	std::vector<GLushort> cpu_elements;
	//GLenum draw_type = GL_STATIC_DRAW;
};

class TexturedGeomComponent : public GeometryComponent {
public:
	TexturedGeomComponent() : GeometryComponent() {
		vertex_array_object.addBuffer(vertex_texture_buffer);

	}
	TexturedGeomComponent(std::vector<glm::vec3> vertices, std::vector<GLushort> elements, 
		std::vector<glm::vec3> normals, std::vector<glm::vec2> tex_coords);

protected:
	void uploadData() override;

	ArrayBuffer vertex_normals_buffer;
	ArrayBuffer vertex_texture_buffer;

private:
	std::vector<glm::vec3> cpu_normals;
	std::vector<glm::vec2> cpu_texture_coords;

};

#endif