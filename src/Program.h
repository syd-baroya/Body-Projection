
#pragma  once

#ifndef LAB471_PROGRAM_H_INCLUDED
#define LAB471_PROGRAM_H_INCLUDED

#include <map>
#include <string>

#include <istream>
#include "Buffers.h"


std::string readFile(std::istream& fileHandle);

class Program
{

public:

	Program();
	Program(const std::string& vpath, const std::string& fpath);

	void setVerbose(const bool v) { verbose = v; }
	bool isVerbose() const { return verbose; }
	GLuint getPID() const { return(pid); }
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

	bool buildProgram(std::istream& compute);
	void dispatch(AtomicCounterBuffer acbo, ShaderStorageBuffer ssbo);
private:
	std::string cShaderName;
	GLuint num_groups_x;
	GLuint num_groups_y;
	GLuint num_groups_z;

};

#endif // LAB471_PROGRAM_H_INCLUDED
