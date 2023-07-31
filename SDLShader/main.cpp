#include <SDL.h>
#undef main
#include <iostream>
#include <SDL_gpu.h>
#include <GL/glew.h>
#include "Shader.h"
#include "ShaderWindow.h"

int main(int argc, char *argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);

	ShaderWindow sWindow;

	// Initialize the shader
	Shader shad("shader", "v1.vert", "fragment_shaders/first.frag");
	shad.addVariable("time");
	shad.addVariable("resolution");
	//shad.addImg("channel0.psd");

	sWindow.setShader(shad);

	sWindow.MainLoop();

	return 0;
}