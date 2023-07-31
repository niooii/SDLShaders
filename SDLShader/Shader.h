#pragma once

#include <cstring>
#include <algorithm>
#include <vector>
#include "SDL_gpu.h"
#include <iostream>

class Shader {
public:
	std::string v_str, f_str, id;
	std::vector<std::pair<std::string, Uint32>> variables;
	GPU_Image* img;
	Shader()
	{
		//do nothing, so i can set it later
	}

	Shader(const std::string& id, const std::string& v_str, const std::string& f_str) : id(id), img(NULL) {
		v = GPU_LoadShader(GPU_VERTEX_SHADER, v_str.c_str());
		
		if (!v)
			std::cout << "Failed to load vertex shader: " << GPU_GetShaderMessage() << "\n";

		f = GPU_LoadShader(GPU_FRAGMENT_SHADER, f_str.c_str());

		if (!f)
			std::cout << "Failed to load fragment shader: " << GPU_GetShaderMessage() << "\n";

		p = GPU_LinkShaders(v, f);

		std::cout << GPU_GetShaderMessage() << "\n";
		if (!p) {
			std::cout << "Failed to link shader program: " << GPU_GetShaderMessage() << "\n";
		}

		block = GPU_LoadShaderBlock(p, "gpu_Vertex", "gpu_TexCoord", NULL, "gpu_ModelViewProjectionMatrix");

		this->v_str = v_str;
		this->f_str = f_str;
	}

	~Shader() {

	}

	void freeImg() {
		GPU_FreeImage(img);
	}

	void addImg(std::string path) {
		img = GPU_LoadImage(path.c_str());

		GPU_SetSnapMode(img, GPU_SNAP_NONE);
		GPU_SetWrapMode(img, GPU_WRAP_REPEAT, GPU_WRAP_REPEAT);
	}

	void addVariable(std::string idV) {
		Uint32 location = GPU_GetUniformLocation(p, idV.c_str());
		variables.push_back(std::make_pair(idV, location));
	}

	std::string getId() { return id; }

	void setImgShader() {
		GPU_SetShaderImage(img, getVar("tex1"), 1);
	}

	void activate() {
		GPU_ActivateShaderProgram(p, &block);
	}

	void deactivate() {
		GPU_DeactivateShaderProgram();
	}

	void destroy()
	{
		GPU_FreeShader(v);
		GPU_FreeShader(f);
		GPU_FreeShaderProgram(p);
		GPU_FreeImage(img);
	}

	//Shader getReloadedShader()
	//{
	//	Shader s(id, v_str, f_str);
	//	s.variables = this->variables;
	//	GPU_Image imgt = *img;
	//	s.img = this->img;
	//	return s;
	//}

	void reload()
	{
		GPU_FreeShader(v);
		GPU_FreeShader(f);
		GPU_FreeShaderProgram(p);

		v = GPU_LoadShader(GPU_VERTEX_SHADER, v_str.c_str());

		if (!v)
			std::cout << "Failed to load vertex shader: " << GPU_GetShaderMessage() << "\n";

		f = GPU_LoadShader(GPU_FRAGMENT_SHADER, f_str.c_str());

		if (!f)
			std::cout << "Failed to load fragment shader: " << GPU_GetShaderMessage() << "\n";

		p = GPU_LinkShaders(v, f);

		std::cout << GPU_GetShaderMessage() << "\n";
		if (!p) {
			std::cout << "Failed to link shader program: " << GPU_GetShaderMessage() << "\n";
		}

		block = GPU_LoadShaderBlock(p, "gpu_Vertex", "gpu_TexCoord", NULL, "gpu_ModelViewProjectionMatrix");
	}

	Uint32 getVar(std::string idV) {
		auto it = std::find_if(variables.begin(), variables.end(), [idV](std::pair<std::string, Uint32> p) { return p.first == idV; });

		if (it != variables.end())
			return it->second;

		return (Uint32)(-1);
	}

private:
	Uint32 v, f, p;
	GPU_ShaderBlock block;
};
