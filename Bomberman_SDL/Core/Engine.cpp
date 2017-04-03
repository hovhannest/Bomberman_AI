
#include "Engine.h"

#ifdef _WIN32
#include <Windows.h>
#include <PathCch.h>
#endif //_WIN32

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <time.h>
#include <string>

#include "Log\Log.h"
#include "Utils\Exception.h"
#include "..\Game\Constants.h"
#include "Utils\OperatingSystem.h"

Bomberman::Engine::Engine()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	
	TTF_Init();

	setPath();

	srand(static_cast<unsigned>(time(0)));
}

Bomberman::Engine::~Engine()
{
	SDL_Quit();
}

void Bomberman::Engine::setPath()
{
#if defined(_WIN32)
	char exePath[MAX_PATH];
	int result;

	result = GetModuleFileNameA(NULL, exePath, MAX_PATH);

	if (ERROR_INSUFFICIENT_BUFFER == result || 0 == result) {
		Log::get() << "Could not get executable path" << NativeOperationException();
	}

	std::string resourcesPath(exePath);
	size_t separatorPos = resourcesPath.rfind(dirSeparator);

	if (std::string::npos == separatorPos) {
		Log::get() << "Executable path is incorrect" << NativeOperationException();
	}

	resourcesPath.erase(separatorPos + 1); // Do not delete the '\'
	resourcesPath.append(Constants::DIR_RESOURCES);

	if (!SetCurrentDirectoryA(resourcesPath.c_str())) {
		Log::get() << "Could not set resources path" << NativeOperationException();
	}
#endif
}

