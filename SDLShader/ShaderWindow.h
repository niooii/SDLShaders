#pragma once

#include "Shader.h"
#include <SDL.h>
#include <GL/glew.h>
#include <stdexcept>

class ShaderWindow
{
private:
	SDL_Window* window{ nullptr };
	GPU_Target* target{ nullptr };

	SDL_DisplayMode display{};
	SDL_Event event;
	float resolution[2]{0,0};
	SDL_Point windowPos;

	Shader shader;
public:
	ShaderWindow();
	void setShader(Shader shad);

	void MainLoop();
	void HandleEvents();
	void Quit();
};