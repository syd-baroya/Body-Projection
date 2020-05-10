
#pragma  once

#ifndef LAB471_PROGRAM_H_INCLUDED
#define LAB471_PROGRAM_H_INCLUDED

#include <map>
#include <string>

#include <istream>
#include "Buffers.h"
#include <iostream>
#include <cassert>
#include <fstream>

#include "GLSL.h"


std::string readFile(std::istream& fileHandle);

class Program
{

public:

	Program();
	virtual void Delete();
	Program(const std::string& vpath, const std::string& fpath);

	void setVerbose(const bool v) { verbose = v; }
	bool isVerbose() const { return verbose; }
	GLuint getPID() { return(pid); }
	bool wasBuildSuccessful() { return(buildSuccess); }

	bool buildProgram(std::istream& vertex, std::istream& fragment);

	virtual void bind();
	virtual void unbind();

	void addAttribute(const std::string &name);
	void addUniform(const std::string &name);
	GLint getAttribute(const std::string &name) const;
	GLint getUniform(const std::string &name) const;

protected:
	GLuint pid = 0;
	bool verbose;
	bool buildSuccess = false;
private:
	GLuint VS, FS;
	std::map<std::string, GLint> attributes;
	std::map<std::string, GLint> uniforms;
	std::string vShaderName;
	std::string fShaderName;
};

class ComputeProgram : public Program {
public:
	ComputeProgram(GLuint x, GLuint y, GLuint z) : Program() {
		num_groups_x = x;
		num_groups_y = y;
		num_groups_z = z;
	}
	ComputeProgram(GLuint x, GLuint y, GLuint z, const std::string& cpath);
	void Delete() override;
	bool buildProgram(std::istream& compute);
	void startUpload();
	void dispatch();
	glm::vec3 getNumGroups() { return(glm::vec3(num_groups_x, num_groups_y, num_groups_z)); }
private:
	GLuint CS;
	std::string cShaderName;
	GLuint num_groups_x;
	GLuint num_groups_y;
	GLuint num_groups_z;

};

#endif // LAB471_PROGRAM_H_INCLUDED
