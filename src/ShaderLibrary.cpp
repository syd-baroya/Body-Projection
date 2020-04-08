#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include "ShaderLibrary.hpp"
#include "Program.h"

using namespace std;

ShaderLibrary::ShaderLibrary(){
	istringstream evs(errorvs);
	istringstream efs(errorfs);

	if(!fallback.buildProgram(evs,efs)){
		exit(1);
	}
	fallback.setVerbose(false);
	fallback.bind();
	active = &fallback;
}

void ShaderLibrary::add(const string &name, Program* prog){
	programs[name] = prog;
}

bool ShaderLibrary::loadFromShaderCollection(string path, bool silent) {
	ifstream shaderfile = ifstream(path);
	if (!shaderfile.is_open()) {
		fprintf(stderr, "Failed to open shaders json file!\n");
		exit(3);
	}

	bool status = true;

	if (!silent) std::cout << "ShaderLibrary loading from " << path << " {" << std::endl;

	string tp, name, v_file_name, f_file_name;
	size_t pos = 0;

	while (getline(shaderfile, tp)) { //read data from file object and put it into string.
		//get name of program
		pos = tp.find(":");
		name = tp.substr(0, pos);
		tp.erase(0, pos + 1);

		//get vertex and fragment shader file names
		if ((pos = tp.find(",")) != std::string::npos) {
			v_file_name = tp.substr(0, pos);
			tp.erase(0, pos + 1);
			f_file_name = tp;
			bool build_status = buildAndAdd(name, v_file_name, f_file_name);
			status &= build_status;
			if (build_status) {
				if (!silent) std::cout << "Loaded shaders: " << v_file_name << " and " << f_file_name << std::endl;
			}
			else {
				if (!silent) std::cout << "Failed to load shaders: " <<  v_file_name << " and " << f_file_name << std::endl;
			}
		}

		//get compute shader file names
		else {
			bool build_status = buildAndAdd(name, tp);
			status &= build_status;
			if (build_status) {
				if (!silent) std::cout << "Loaded compute shader: " << tp << std::endl;
			}
			else {
				if (!silent) std::cout << "Failed to load compute shader: " << tp << std::endl;
			}
		}

	}
	shaderfile.close(); //close the file object.

	if (!silent) std::cout << "Finished loading shaders from json" << std::endl << std::endl;
	return(status);
}


bool ShaderLibrary::buildAndAdd(string name, const string &vpath, const string &fpath){
	Program* newprog = new Program(vpath, fpath);
	bool status = newprog && newprog->wasBuildSuccessful();
	if(status){
    	programs[name] = newprog; 
	}
	return(status);
}

bool ShaderLibrary::buildAndAdd(string name, const string& cpath) {
	Program* newprog = new Program(cpath);
	bool status = newprog && newprog->wasBuildSuccessful();
	if (status) {
		programs[name] = newprog;
	}
	return(status);
}

void ShaderLibrary::printLibary() const {
	std::cout << "ShaderLibrary{" << std::endl;
	for(std::pair< std::string, Program*> element : programs){
		std:: cout << "   " << element.first << std::endl;
	}
	std::cout << "}" << std::endl;
}

void ShaderLibrary::makeActive(const string &name){
	unordered_map<string,Program*>::iterator iter;

	if((iter = programs.find(name)) == programs.end()){
		fprintf(stderr, "Invalid name for program %s\n", name.c_str());
		active = &fallback;
		return;
	}else{
		// This shader is not already active so switch. 
		active->unbind();
		active = iter->second;
		active->bind();
	}
}

void ShaderLibrary::fastActivate(Program* prog){
	if(prog == NULL){
		fprintf(stderr, "Tried to swap to null shader!\n");
		active = &fallback;
	}else if(prog != active){
		(active->unbind());
		active = prog;
		(active->bind());
	}
}

Program& ShaderLibrary::getActive(){
	if(active == &fallback){
		fprintf(stderr,"Warning! Returning error shader from ShaderLibrary.getActive()!\n");
	}
	return(*active);
}

void ShaderLibrary::dumpBinary(const string& name){
	Program* todump = nullptr;
	if(programs.find(name) == programs.end()){
		fprintf(stderr, "Warning! Could not fetch shader to dump!\n");
		return;
	}else{
		todump = programs[name];
	}
	size_t bin_buffer_size = 1<<20;
	char* binary = new char[bin_buffer_size];
	GLsizei binlen;
	GLenum binformat;
	glGetProgramBinary(todump->getPID(), 1<<20, &binlen, &binformat, binary);
	FILE* bfile = fopen((name+"_dump.sasm").c_str(), "wb");
	fwrite(binary, sizeof(char), binlen, bfile);
	fclose(bfile);
	delete[] binary;
}

Program* ShaderLibrary::getActivePtr(){
	return(active);
}

Program* ShaderLibrary::getPtr(const string& name){
	if(programs.find(name) == programs.end()){
		fprintf(stderr, "Warning! Could not fetch shader pointer!\n");
		return(NULL);
	}else{
		return(programs[name]);
	}
}