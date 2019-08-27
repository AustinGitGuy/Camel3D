// Camel3D.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "GL/freeglut.h"
#include "GL/glut.h"
#include "Game.h"

int main(int argc, char** argv){
	glutInit(&argc, argv);
	Game::InitInstance();
	Game* ptr = Game::GetInstance();
	ptr->Init(1280, 960);
	ptr->GameLoop();
	ptr->Cleanup();
	ptr->CleanupInstance();
	return 0;
}