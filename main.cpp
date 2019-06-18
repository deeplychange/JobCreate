#include <windows.h>
#include <memory>
#include <assert.h>
#include <tchar.h>
#include "Game_lib.h"
#include "Scene.h"
#include "SceneMain.h"


//------------------------------------------------------------------------------
// WinMainä÷êî
//------------------------------------------------------------------------------
INT WINAPI wWinMain(HINSTANCE instance, HINSTANCE prev_instance, LPWSTR cmd_line, INT cmd_show)
{

	UNREFERENCED_PARAMETER(instance);
	UNREFERENCED_PARAMETER(prev_instance);
	UNREFERENCED_PARAMETER(cmd_line);
	UNREFERENCED_PARAMETER(cmd_show);
	
	//âπäy
	//PlaySound(L"./Data/Sound/BF.wav", NULL, SND_FILENAME || SND_ASYNC || SND_LOOP);

	//GameLib::OpenDebugWindow();

	SceneManager sceneManager;
	sceneManager.execute(SCENE_MAIN);

	GameLib::CloseDebugWindow();

}
