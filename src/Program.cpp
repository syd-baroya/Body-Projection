
#include "Program.h"
#include <iostream>
#include <cassert>
#include <fstream>

#include "GLSL.h"


std::string readFile(std::istream& fileHandle)
{
	std::string result;
	//std::ifstream fileHandle(fileName);

	fileHandle.seekg(0, std::ios::end);
	result.reserve((size_t) fileHandle.tellg());
	fileHandle.seekg(0, std::ios::beg);

	result.assign((std::istreambuf_iterator<char>(fileHandle)), std::istreambuf_iterator<char>());

	return result;
}

Program::Program() :
	pid(0),
	verbose(true)
{
}


Program::Program(const std::string& vpath, const std::string& fpath)
{
	std::ifstream vertex = std::ifstream(vpath);
	std::ifstream fragment = std::ifstream(fpath);
	if (!vertex.is_open()) {
		fprintf(stderr, "Warning!: Couldn't open given shader file: ");
		std::cerr << vpath << std::endl;
	}
	else if (!fragment.is_open()) {
		fprintf(stderr, "Warning!: Couldn't open given shader file: ");
		std::cerr << fpath << std::endl;
	}
	if (!buildProgram(vertex, fragment)) {
		fprintf(stderr, "Warning!: Failed building VsFs program given in constructor.\n");
		return;
	}
	buildSuccess = true;
}

Program::Program(const std::string& cpath)
{
	std::ifstream compute = std::ifstream(cpath);
	if (!compute.is_open()) {
		fprintf(stderr, "Warning!: Couldn't open given shader file: ");
		std::cerr << cpath << std::endl;
	}
	if (!buildProgram(compute)) {
		fprintf(stderr, "Warning!: Failed building Cs program given in constructor.\n");
		return;
	}
	buildSuccess = true;
}


bool Program::buildProgram(std::istream& vertex, std::istream& fragment)
{
	GLint rc;

	// Create shader handles
	GLuint VS = glCreateShader(GL_VERTEX_SHADER);
	GLuint FS = glCreateShader(GL_FRAGMENT_SHADER);

	// Read shader sources
	std::string vShaderString = readFile(vertex);
	std::string fShaderString = readFile(fragment);
	const char* vshader = vShaderString.c_str();
	const char* fshader = fShaderString.c_str();
	CHECKED_GL_CALL(glShaderSource(VS, 1, &vshader, NULL));
	CHECKED_GL_CALL(glShaderSource(FS, 1, &fshader, NULL));

	// Compile shaders
	if (!GLSL::compileAndCheck(VS, verbose)) {
		fprintf(stderr, "Compiling vertex shader %s failed!\n", vshader);
		return(false);
	}
	if (!GLSL::compileAndCheck(FS, verbose)) {
		fprintf(stderr, "Compiling fragment shader %s failed!\n", fshader);
		return(false);
	}

	// Create the program and link
	pid = glCreateProgram();
	CHECKED_GL_CALL(glAttachShader(pid, VS));
	CHECKED_GL_CALL(glAttachShader(pid, FS));
	if (!GLSL::linkAndCheck(pid, verbose)) {
		fprintf(stderr, "Linking shaders %s and %s failed!\n", vshader, fshader);
		return(false);
	}

	GLSL::checkError(GET_FILE_LINE);
	return true;
}

bool ComputeProgram::buildProgram(std::istream& compute)
{
	//load the compute shader
	std::string cShaderString = readFile(compute);
	const char* cshader = cShaderString.c_str();
	GLuint CS = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(CS, 1, &cshader, nullptr);

	if (!GLSL::compileAndCheck(CS, verbose)) {
		fprintf(stderr, "Compiling vertex shader %s failed!\n", cshader);
		return(false);
	}

	pid = glCreateProgram();
	glAttachShader(pid, CS);
	GLSL::linkAndCheck(pid, (GLuint)2);
	return true;
}

void Program::bind()
{
	CHECKED_GL_CALL(glUseProgram(pid));
}

void Program::unbind()
{
	CHECKED_GL_CALL(glUseProgram(0));
}

void Program::addAttribute(const std::string &name)
{
	attributes[name] = GLSL::getAttribLocation(pid, name.c_str(), isVerbose());
}

void Program::addUniform(const std::string &name)
{
	uniforms[name] = GLSL::getUniformLocation(pid, name.c_str(), isVerbose());
}

GLint Program::getAttribute(const std::string &name) const
{
	std::map<std::string, GLint>::const_iterator attribute = attributes.find(name.c_str());
	if (attribute == attributes.end())
	{
		if (isVerbose())
		{
			std::cout << name << " is not an attribute variable" << std::endl;
		}
		return -1;
	}
	return attribute->second;
}

GLint Program::getUniform(const std::string &name) const
{
	std::map<std::string, GLint>::const_iterator uniform = uniforms.find(name.c_str());
	if (uniform == uniforms.end())
	{
		if (isVerbose())
		{
			std::cout << name << " is not a uniform variable" << std::endl;
		}
		return -1;
	}
	return uniform->second;
}

void ComputeProgram::dispatch(AtomicCounterBuffer acbo, ShaderStorageBuffer ssbo)
{
	GLuint block_index = 0;
	block_index = glGetProgramResourceIndex(pid, GL_SHADER_STORAGE_BLOCK, "shader_data");
	GLuint ssbo_binding_point_index = 0;
	glShaderStorageBlockBinding(pid, block_index, ssbo_binding_point_index);
	ssbo.bindBufferBase(0);
	glUseProgram(pid);
	//activate atomic counter
	acbo.bind();
	acbo.bindBufferBase(0);

	glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);				//start compute shader
	ssbo.unbindBufferBase(0);
}
