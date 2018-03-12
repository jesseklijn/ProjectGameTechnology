#include "GameManager.h"
#include <utility>

//Dll files
#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

GameManager::GameManager()
{

}

GameManager::~GameManager()
{

}

void GameManager::Start()
{

}

void GameManager::Awake()
{
	
}

void GameManager::Update()
{
	
}

void GameManager::Draw()
{
	
}


void GameManager::GameStateTransition(GameState StateToLoad)
{

}

void GameManager::UnloadGameState(GameState StateToCleanUp)
{

}
