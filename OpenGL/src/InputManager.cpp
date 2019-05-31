#include "pch.h"

// Internal state boolean
SDL_Event InputManager::Event;
bool InputManager::IsInstantiated = false;
// Inputs vectors
std::vector<bool> InputManager::Keys;
std::vector<bool> InputManager::Mouse;
// Mouse positions
int InputManager::PreviousKey(-1);
int InputManager::MouseX(0);
int InputManager::MouseY(0);
int InputManager::MouseRelX(0);
int InputManager::MouseRelY(0);


void InputManager::Create() 
{
	try
	{
		if (!IsInstantiated)
		{
			IsInstantiated = true;
			for (unsigned i = 0; i < SDL_NUM_SCANCODES; ++i)
			{
				Keys.push_back(false);
			}
			for (unsigned i = 0; i < 8; ++i)		// 8 = Number of buttons supported by the SDL
			{
				Mouse.push_back(false);
			}

			HideCursor(true);
			CatchCursor(true);
		}
		else
			throw std::exception("An InputManager has already been instantiated\n");
	}
	catch (std::exception & e)
	{
		std::cout << e.what();
	}
}

void InputManager::Update() 
{
	if (IsInstantiated)
	{
		// Reset relative position of the mouse (if there is no event, it has to stay at 0)
		MouseRelX = 0;
		MouseRelY = 0;
		while (SDL_PollEvent(&Event))
		{
			if (Event.window.event == SDL_WINDOWEVENT_CLOSE)
			{
				DisplayManager::IsRunning = false;
			}
			else if (Event.type == SDL_KEYUP)
			{
				Keys[Event.key.keysym.scancode] = false;
				switch (Event.key.keysym.scancode)
				{
					case SDL_SCANCODE_ESCAPE:	DisplayManager::IsRunning = false;  break;
					default:														break;
				}
			}
			else if (Event.type == SDL_KEYDOWN)
			{
				Keys[Event.key.keysym.scancode] = true;
			}
			else if (Event.type == SDL_MOUSEBUTTONUP)
			{
				Mouse[Event.button.button] = false;
				if (Event.button.button == SDL_BUTTON_RIGHT)
					DisplayManager::Zoom(false);
			}
			else if (Event.type == SDL_MOUSEBUTTONDOWN)
			{
				Mouse[Event.button.button] = true;
				if (Event.button.button == SDL_BUTTON_RIGHT)
					DisplayManager::Zoom(true);
			}
			else if (Event.type == SDL_MOUSEMOTION)
			{
				MouseX		= Event.motion.x;
				MouseY		= Event.motion.y;
				MouseRelX	= Event.motion.xrel;
				MouseRelY	= Event.motion.yrel;
			}
			else if (Event.type == SDL_MOUSEWHEEL)
			{
				if (Event.wheel.y > 0)
					DisplayManager::Zoom(true);
				else if (Event.wheel.y < 0)
					DisplayManager::Zoom(false);
			}
		}
	}
}

void InputManager::Destroy() 
{
	if (IsInstantiated)
		IsInstantiated = false;
}

bool InputManager::IsKeyPressed(const int & keyCode)
{
	if (IsInstantiated)
	{
		if (keyCode < Keys.size())
			return Keys[keyCode];
		return false;
	}
}

bool InputManager::IsKeyPressedOnce(const int & keyCode)
{
	if (IsInstantiated)
	{
		if (Keys[keyCode] && keyCode != PreviousKey)
		{
			PreviousKey = keyCode;
			return true;
		}
		if (PreviousKey != -1 && !Keys[PreviousKey])
			PreviousKey = -1;
		return false;
	}
}

bool InputManager::IsButtonPressed(const int & buttonCode)
{
	if (IsInstantiated)
	{
		if (buttonCode < Mouse.size())
			return Mouse[buttonCode];
		return false;
	}
}

bool InputManager::DidMouseMove()
{
	if (IsInstantiated)
		return MouseRelX || MouseRelY;
}

glm::vec2 InputManager::GetMousePosition()
{
	if (IsInstantiated)
		return glm::vec2(MouseX, MouseY);
}

glm::vec2 InputManager::GetMouseRelativePosition()
{
	if (IsInstantiated)
		return glm::vec2(MouseRelX, MouseRelY);
}

void InputManager::HideCursor(const bool & hide)
{
	if (IsInstantiated)
	{
		if (hide)
			SDL_ShowCursor(SDL_ENABLE);
		else
			SDL_ShowCursor(SDL_DISABLE);
	}
}

void InputManager::CatchCursor(const bool & _catch)
{
	if (IsInstantiated)
	{
		if (_catch)
			SDL_SetRelativeMouseMode(SDL_TRUE);
		else
			SDL_SetRelativeMouseMode(SDL_FALSE);
	}
}
