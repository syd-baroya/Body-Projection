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
	GeometryComponent(const std::vector<glm::vec3>& vertices, const std::vector<GLuint>& elements) {
		cpu_vertices = vertices;
		cpu_elements = elements;
		vertex_array_object.addBuffer(vertex_pos_buffer);
		vertex_array_object.addBuffer(vertex_elements_buffer);
	}

	void setupForDraw();
	void cleanupAfterDraw();
	void uploadData();
	void startGPUUpload();
	void finishGPUUpload();

protected:
	VertexArrayObject vertex_array_object;
	ArrayBuffer vertex_pos_buffer;
	ElementArrayBuffer vertex_elements_buffer;

private:
	std::vector<glm::vec3> cpu_vertices;
	std::vector<GLuint> cpu_elements;
	GLenum draw_type = GL_STATIC_DRAW;
};

class TexturedGeomComponent : public GeometryComponent {
public:
	TexturedGeomComponent() : GeometryComponent() {
		vertex_array_object.addBuffer(vertex_texture_buffer);

	}
	TexturedGeomComponent(const std::vector<glm::vec3>& vertices, const std::vector<GLuint>& elements, 
		std::vector<glm::vec3>& normals, std::vector<glm::vec2>& tex_coords);

	void uploadData();
protected:
	ArrayBuffer vertex_normals_buffer;
	ArrayBuffer vertex_texture_buffer;

private:
	std::vector<glm::vec3> cpu_normals;
	std::vector<glm::vec2> cpu_texture_coords;

};

#endif