#include "ShaderWindow.h"

ShaderWindow::ShaderWindow()
{
	SDL_GetDisplayMode(0, 0, &display);

	/* scale it down a little */
	resolution[0] = display.h / 3.0;
	resolution[1] = display.h / 3.0;

	window = SDL_CreateWindow("Shader Renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, display.w, display.h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	GPU_SetInitWindow(SDL_GetWindowID(window));
	/* do not ask why i put it as 1 and it actually works better for some reason! */
	target = GPU_InitRenderer(GPU_RENDERER_OPENGL_3, static_cast<Uint16>(1), static_cast<Uint16>(1), GPU_DEFAULT_INIT_FLAGS);

	GLenum err = glewInit();

	if (target  == NULL || err != GLEW_OK) {
		//throw new std::exception("error initialization OpenGL");
		std::cout << "error initialization OpenGL" << "\n";
	}

	/* scale window down */
	SDL_SetWindowSize(window, resolution[0], resolution[1]);
	SDL_SetWindowPosition(window, display.w/2.0 - resolution[0] / 2.0, display.h/2.0 - resolution[1] / 2.0);
}

void ShaderWindow::setShader(Shader shad)
{
	this->shader = shad;
}



void ShaderWindow::HandleEvents()
{
	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_QUIT:
			Quit();
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				Quit();
				break;
			case SDLK_r:
				std::cout << "reloading shaders...\n";
				/*shader.reload();*/

				// Initialize the shader
				std::string id{shader.id};
				std::string v_str{shader.v_str};
				std::string f_str{shader.f_str};

				

				Shader shad = Shader(shader.id, shader.v_str, shader.f_str);

				for (auto& var : shader.variables)
				{
					shad.addVariable(var.first);
					std::cout << "Readding var: " << var.first << '\n';
				}

				//destroy old shader resources
				shader.destroy();
				
				shader = shad;

				break;
			}
			break;

		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				resolution[0] = event.window.data1;
				resolution[1] = event.window.data2;
				std::cout << "set x to " << resolution[0] << ", set y to " << resolution[1] << '\n';
			}
			if (event.window.event == SDL_WINDOWEVENT_MOVED)
			{
				windowPos.x = event.window.data1;
				windowPos.y = event.window.data2;
			}
			break;
		}
	}
}

void ShaderWindow::MainLoop()
{
	while (true) {
		HandleEvents();

		GPU_ClearRGB(target, 0, 0, 0); // Clear the screen to black.

		GLfloat time = ((GLfloat)SDL_GetTicks()) / 1000.0;

		shader.activate();

		/* set shader uniform vars */
		GPU_SetUniformf(shader.getVar("time"), time);
		GPU_SetUniformfv(shader.getVar("resolution"), 2, 1, resolution);

		shader.setImgShader();
		//GPU_Blit(field, NULL, window, field->w / 2.f, field->h / 2.f);
		/* render a white rect */
		GPU_RectangleFilled(target, 0, 0, resolution[0], resolution[1], GPU_MakeColor(255, 255, 255, 255));

		shader.deactivate();

		GPU_Flip(target);
	}
}

void ShaderWindow::Quit()
{
	shader.freeImg();

	GPU_Quit();
	SDL_Quit();
	exit(0);
}