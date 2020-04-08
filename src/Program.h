
#pragma  once

#ifndef LAB471_PROGRAM_H_INCLUDED
#define LAB471_PROGRAM_H_INCLUDED

#include <map>
#include <string>

#include <glad/glad.h>
#include <istream>


std::string readFile(std::istream& fileHandle);

class Program
{

public:

	Program();
	Program(const std::string& vpath, const std::string& fpath);
	Program(const std::string& cpath);

	void setVerbose(const bool v) { verbose = v; }
	bool isVerbose() const { return verbose; }
	GLuint getPID() const { return(pid); }
	bool wasBuildSuccessful() { return(buildSuccess); }

	bool buildProgram(std::istream& vertex, std::istream& fragment);
	bool buildProgram(std::istream& compute);

	virtual void bind();
	virtual void unbind();

	void addAttribute(const std::string &name);
	void addUniform(const std::string &name);
	GLint getAttribute(const std::string &name) const;
	GLint getUniform(const std::string &name) const;
	GLuint pid = 0;
protected:

	std::string vShaderName;
	std::string fShaderName;
	std::string cShaderName;

private:

	GLuint pid;
	std::map<std::string, GLint> attributes;
	std::map<std::string, GLint> uniforms;
	bool verbose;
	bool buildSuccess = false;
};

#endif // LAB471_PROGRAM_H_INCLUDED
