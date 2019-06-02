#include "pch.h"

// Clear color
glm::vec4 DisplayManager::ClearColor(0.1, 0.1, 0.1, 1.0);/// (0.4, 0.7, 1.0, 1.0);
// FPS Management
Uint32 DisplayManager::StartTime(0);
Uint32 DisplayManager::DeltaTime(0);
float DisplayManager::FpsCap(1000/60);			// (1000 / FpsDesired)
// Window and OpenGl
SDL_GLContext DisplayManager::Settings;
SDL_Window * DisplayManager::Window(nullptr);
float DisplayManager::Width(0.f);
float DisplayManager::Height(0.f);
glm::mat4 DisplayManager::Projection;
double DisplayManager::Fovy(45.0);
// Internal state boolean
bool DisplayManager::IsInstantiated(false);
bool DisplayManager::IsRunning(true);

void DisplayManager::Create(float w, float h)
{
	if (!IsInstantiated)
	{
		// Initialize the SDL
		if (SDL_Init( SDL_INIT_EVERYTHING ) < 0)
		{
			std::cerr << "Failed to initialize SDL, error in file " << __FILE__ << ", line " << __LINE__ << std::endl;
			throw std::exception("Failed to initialize SDL");
		}

		// Set OpenGL's version
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		
		// Set OpenGL' profile to core
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		// Set double buffering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		Width = w;
		Height = h;

		// Create the window
		Window = SDL_CreateWindow("Hogwart's RPG in OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
		if (!Window)
		{
			std::cerr << "Failed to create the SDL_Window, error in file " << __FILE__ << ", line " << __LINE__ << std::endl;
			SDL_Quit();
			throw std::exception("Failed to create the SDL_Window");
		}

		// Create the OpenGL's context
		Settings = SDL_GL_CreateContext(Window);
		if (!Settings)
		{
			std::cerr << "Failed to create the SDL_Context, error in file " << __FILE__ << ", line " << __LINE__ << std::endl;
			SDL_DestroyWindow(Window);
			SDL_Quit();
			throw std::exception("Failed to create the SDL_Context");
		}

		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
		{
			std::cerr << "Failed to initialize GLAD" << std::endl;
			throw std::exception("Failed to initialize GLAD");
		}

		glViewport(0, 0, Width, Height);
		glClearColor(ClearColor.x, ClearColor.y, ClearColor.z, ClearColor.w);
		glEnable(GL_DEPTH_TEST);
		// Disable the drawing of back faces // DO IT WHEN NORMALS ARE USED
		/*glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);*/


		Projection = glm::perspective(glm::radians(Fovy), static_cast<double>(Width) / Height, 0.1, 100.0);

		IsInstantiated = true;
	}
	// Only one window can be created
	else
	{
		std::cerr << "A window has already been instantiated\n";
	}
}

void DisplayManager::StartLoop()
{
	if (IsInstantiated)
	{
		StartTime = SDL_GetTicks();
		InputManager::Update();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

void DisplayManager::EndLoop()
{
	if (IsInstantiated)
	{
		SDL_GL_SwapWindow(Window);

		// Managing FPS
		DeltaTime = SDL_GetTicks() - StartTime;
		// Display the fps -> 
		SDL_SetWindowTitle(Window, ("FPS : " + std::to_string(( 1 / (DeltaTime / 1000.f)))).c_str());
		if (DeltaTime < FpsCap)
			SDL_Delay(FpsCap - DeltaTime);
	}
}

void DisplayManager::Destroy()
{
	if (IsInstantiated)
	{
		IsInstantiated = false;
		SDL_GL_DeleteContext(Settings);
		SDL_DestroyWindow(Window);
		SDL_Quit();
	}
}


struct Zooms
{
	double none = 45.0;
	double in = 20.0;
	double out = 70.0;
} Zooms;

void DisplayManager::Zoom(const bool & in)
{
	if (in && (Fovy == static_cast<double>(Zooms.none)))
		Fovy = static_cast<double>(Zooms.in);
	else if (in && (Fovy == static_cast<double>(Zooms.out)))
		Fovy = static_cast<double>(Zooms.none);
	else if (!in && (Fovy == static_cast<double>(Zooms.none)))
		Fovy = static_cast<double>(Zooms.out);
	else if (!in && (Fovy == static_cast<double>(Zooms.in)))
		Fovy = static_cast<double>(Zooms.none);

	Projection = glm::perspective(glm::radians(Fovy), static_cast<double>(Width) / Height, 0.1, 100.0);
}