#define GLFW_DLL

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/glut.h"
#include "Vector3.h"
#include "Game.h"
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "OBJStructs.h"
#include "GameObject.h"
#include "Camera.h"
#include "BoxCollider.h"
#include <time.h>
#include "FreeImage.h"
#include "Skeleton.h"
#include "Clip.h"
#include "Rigidbody.h"

Game* Game::instance = NULL;
int Game::msID = 0;

Vector3 gray(.25f, .25f, .25f);
Vector3 red(1, 0, 0);
Vector3 blue(0, 0, 1);
Vector3 green(0, 1, 0);
Vector3 white(1, 1, 1);

const int PASS_NUM = 4;

const int SKELE_PARTS = 29;

void Game::CrossProduct(Vector3 one, Vector3 two, Vector3* product){
	product->x = one.y * two.z - one.z * two.y; 
	product->y = one.x * two.z - one.z * two.x; 
	product->z = one.x * two.y - one.y * two.x; 
}

void Game::ClearGL(){
	glClearColor(bgColor.x, bgColor.y, bgColor.z, 1.0);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
}

void Game::Init(int width, int height){
	if(inited){
		return;
	}

	newTime = oldTime = std::chrono::high_resolution_clock::now();

	bgColor = gray;

	//Initialize glut and glew
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	screenWidth = width;
	screenHeight = height;
	glutCreateWindow("Camel 3D");

	//Create the right click menu
	int colorMenu = glutCreateMenu(ColorMenuWrapper);
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Green", 2);
	glutAddMenuEntry("Blue", 3);

	int mainMenu = glutCreateMenu(MenuWrapper);
	glutAddMenuEntry("Create Cube", 1);
	glutAddMenuEntry("Create Sphere", 2);
	glutAddMenuEntry("Create Pyramid", 3);
	glutAddMenuEntry("Create Camera", 4);
	glutAddMenuEntry("Attach Collider to Selected Object", 5);
	glutAddSubMenu("Change Color of Selected Object", colorMenu);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	GLenum err = glewInit();
	if (GLEW_OK != err){
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
	}
	std::cout << "Using GLEW: " << glewGetString(GLEW_VERSION) << std::endl;

	std::string filename = "diver.obj";

	//Create some basic objects and a camera
	//NewObject(GameObject::Type::CUBE, red, Vector3(1.5f, 0.0f, -10.0f), VECTOR3_ONE, Vector3(90, 0, 0), "Cube");
	NewObject(GameObject::Type::CUBE, white, Vector3(-1.5f, 0.5f, -10.0f), Vector3(5.0f, .01f, 5.0f), VECTOR3_ZERO, "Plane", true);
	NewObject(GameObject::Type::DODECAHEDRON, Vector3(1.0f, 1.0f, 0.0f), Vector3(-6.5f, 0.0f, -10.0f), VECTOR3_ONE, VECTOR3_ZERO, "Dodecahedron", false, false);
	NewObject(GameObject::Type::CUBE, blue, Vector3(-3.0f, -1.1f, -10.0f), Vector3(20.0f, .01f, 20.0f), VECTOR3_ZERO, "BasePlane", true);
	NewObject(GameObject::Type::TEAPOT, Vector3(0.0, 1.0f, 1.0f), Vector3(5.0f, 0.0f, -10.0f), VECTOR3_ONE, VECTOR3_ZERO, "Teapot", false);
	NewObject(GameObject::Type::OBJ, (char*)filename.c_str(), green, Vector3(-10.0f, -1.0f, -10.0f), Vector3(0.03f, 0.03f, 0.03f), VECTOR3_ZERO, "Diver", true);
	NewCamera(Vector3(0, 2, 1), Vector3(0, 0, -1));

	GameObject *teapot = FindObject("Teapot");
	teapot->AttachRigidbody(1);

	teapot->GetRigidbody()->AddForce(Vector3(1, 0, 1));

	//Create the skeleton

	NewSkeletonPart(Vector3(-1.5f, 3, -6), VECTOR3_ZERO, VECTOR3_ONE, "SkeleRoot", "BasicSkele", nullptr, true, true);
	NewSkeletonPart(Vector3(0, -0.4f, 0), VECTOR3_ZERO, VECTOR3_ONE, "SkeleNeck", "BasicSkele", FindObject("SkeleRoot"), false, true);
	NewSkeletonPart(Vector3(0, -0.2f, 0), VECTOR3_ZERO, VECTOR3_ONE, "SkeleClavicle", "BasicSkele", FindObject("SkeleNeck"), false, false);
	NewSkeletonPart(Vector3(0, -0.8f, 0), VECTOR3_ZERO, VECTOR3_ONE, "SkeleMidspine", "BasicSkele", FindObject("SkeleClavicle"), false, true);
	NewSkeletonPart(Vector3(0, -0.8f, 0), VECTOR3_ZERO, VECTOR3_ONE, "SkeleLowerSpine", "BasicSkele", FindObject("SkeleMidspine"), false, true);

	NewSkeletonPart(Vector3(-.2f, -.2f, 0), VECTOR3_ZERO, VECTOR3_ONE, "SkeleShoulderbladeR", "BasicSkele", FindObject("SkeleClavicle"), true, false);
	NewSkeletonPart(Vector3(-.2f, .2f, 0), VECTOR3_ZERO, VECTOR3_ONE, "SkeleShoulderR", "BasicSkele", FindObject("SkeleShoulderbladeR"), false, true);
	NewSkeletonPart(Vector3(-.65f, -.5f, 0), VECTOR3_ZERO, VECTOR3_ONE, "SkeleElbowR", "BasicSkele", FindObject("SkeleShoulderR"), false, true);
	NewSkeletonPart(Vector3(-.4f, -.3f, 0), VECTOR3_ZERO, VECTOR3_ONE, "SkeleForearmR", "BasicSkele", FindObject("SkeleElbowR"), false, false);
	NewSkeletonPart(Vector3(-.4f, -.3f, 0), VECTOR3_ZERO, VECTOR3_ONE, "SkeleWristR", "BasicSkele", FindObject("SkeleForearmR"), false, true);
	NewSkeletonPart(Vector3(-.1f, -.1f, 0), VECTOR3_ZERO, VECTOR3_ONE, "SkeleHandR", "BasicSkele", FindObject("SkeleWristR"), false, false);

	NewSkeletonPart(Vector3(.2f, -.2f, 0), VECTOR3_ZERO, VECTOR3_ONE, "SkeleShoulderbladeL", "BasicSkele", FindObject("SkeleClavicle"), true, false);
	NewSkeletonPart(Vector3(.2f, .2f, 0), VECTOR3_ZERO, VECTOR3_ONE, "SkeleShoulderL", "BasicSkele", FindObject("SkeleShoulderbladeL"), false, true);
	NewSkeletonPart(Vector3(.65f, -.5f, 0), VECTOR3_ZERO, VECTOR3_ONE, "SkeleElbowL", "BasicSkele", FindObject("SkeleShoulderL"), false, true);
	NewSkeletonPart(Vector3(.4f, -.3f, 0), VECTOR3_ZERO, VECTOR3_ONE, "SkeleForearmL", "BasicSkele", FindObject("SkeleElbowL"), false, false);
	NewSkeletonPart(Vector3(.4f, -.3f, 0), VECTOR3_ZERO, VECTOR3_ONE, "SkeleWristL", "BasicSkele", FindObject("SkeleForearmL"), false, true);
	NewSkeletonPart(Vector3(.1f, -.1f, 0), VECTOR3_ZERO, VECTOR3_ONE, "SkeleHandL", "BasicSkele", FindObject("SkeleWristL"), false, false);

	NewSkeletonPart(Vector3(-.4f, 0.15f, 0), VECTOR3_ZERO, VECTOR3_ONE, "SkeleHipR", "BasicSkele", FindObject("SkeleLowerSpine"), false, true);
	NewSkeletonPart(Vector3(0, -.8f, 0), VECTOR3_ZERO, VECTOR3_ONE, "SkeleKneeR", "BasicSkele", FindObject("SkeleHipR"), false, true);
	NewSkeletonPart(Vector3(0, -.35f, 0), VECTOR3_ZERO, VECTOR3_ONE, "SkeleShinR", "BasicSkele", FindObject("SkeleKneeR"), false, false);
	NewSkeletonPart(Vector3(0, -.35f, 0), VECTOR3_ZERO, VECTOR3_ONE, "SkeleAnkleR", "BasicSkele", FindObject("SkeleShinR"), false, true);
	NewSkeletonPart(Vector3(0, -.05f, .15f), VECTOR3_ZERO, VECTOR3_ONE, "SkeleFootR", "BasicSkele", FindObject("SkeleAnkleR"), false, true);
	NewSkeletonPart(Vector3(0, -.05f, .15f), VECTOR3_ZERO, VECTOR3_ONE, "SkeleToesR", "BasicSkele", FindObject("SkeleFootR"), false, false);

	NewSkeletonPart(Vector3(.4f, 0.15f, 0), VECTOR3_ZERO, VECTOR3_ONE, "SkeleHipL", "BasicSkele", FindObject("SkeleLowerSpine"), false, true);
	NewSkeletonPart(Vector3(0, -.8f, 0), VECTOR3_ZERO, VECTOR3_ONE, "SkeleKneeL", "BasicSkele", FindObject("SkeleHipL"), false, true);
	NewSkeletonPart(Vector3(0, -.35f, 0), VECTOR3_ZERO, VECTOR3_ONE, "SkeleShinL", "BasicSkele", FindObject("SkeleKneeL"), false, false);
	NewSkeletonPart(Vector3(0, -.35f, 0), VECTOR3_ZERO, VECTOR3_ONE, "SkeleAnkleL", "BasicSkele", FindObject("SkeleShinL"), false, true);
	NewSkeletonPart(Vector3(0, -.05f, .15f), VECTOR3_ZERO, VECTOR3_ONE, "SkeleFootL", "BasicSkele", FindObject("SkeleAnkleL"), false, true);
	NewSkeletonPart(Vector3(0, -.05f, .15f), VECTOR3_ZERO, VECTOR3_ONE, "SkeleToesL", "BasicSkele", FindObject("SkeleFootL"), false, false);

	//skeletons["BasicSkele"].WriteToFile("BasicSkele.txt");

	//skeletons["BasicSkele"].ReadFromFile("BasicSkele.txt");

	starting = new Clip<Transform>(Keyframe<Transform>(SKELE_PARTS, 5));

	starting->blendType = BlendType::BlendLerp;

	auto it = std::next(starting->keys.begin(), 0);

	//Working in relative space. Convert from world space beforehand if you want to do that.
	for(int i = 0; i < SKELE_PARTS; i++){
		it->data[i].objName = skeletons["BasicSkele"].parts[FindIndexSkeleton("SkeleRoot", "BasicSkele") + i]->name;
		it->data[i].pos = VECTOR3_ZERO;
		it->data[i].rot = VECTOR3_ZERO;
		it->data[i].scale = VECTOR3_ONE;
	}

	it->data[6].rot = Vector3(0, 0, -69);
	it->data[7].rot = Vector3(0, 0, -140);

	starting->AddKeyframe(Keyframe<Transform>(SKELE_PARTS, 5));

	it = std::next(starting->keys.begin(), 1);

	for(int i = 0; i < SKELE_PARTS; i++){
		it->data[i].objName = skeletons["BasicSkele"].parts[FindIndexSkeleton("SkeleRoot", "BasicSkele") + i]->name;
		it->data[i].pos = VECTOR3_ZERO;
		it->data[i].rot = VECTOR3_ZERO;
		it->data[i].scale = VECTOR3_ONE;
	}

	it->data[0].pos = Vector3(1, 0, 0);

	//Blend between multiple keyframe poses

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glEnable(GL_TEXTURE_2D);

	//Generate FBOS
	CreateBasicFBO("offscreen");
	CreateTexture("terrain.bmp", "terrain");

	//Create each program individually. Create - Attach - Link - Compile - Uniforms - Detach

	CreateProgram("defaultProgram");
	CreateShader("default-frag", "fragment.glsl", GL_FRAGMENT_SHADER);
	CreateShader("default-vert", "vertex.glsl", GL_VERTEX_SHADER);
	AttachShader("defaultProgram", "default-vert", "default-frag");
	LinkProgram("defaultProgram");
	CompileShader("defaultProgram");

	CreateProgram("noiseProgram");
	CreateShader("noiseProgram-vert", "NoiseVertex.glsl", GL_VERTEX_SHADER);
	CreateShader("noiseProgram-frag", "NoiseFragment.glsl", GL_FRAGMENT_SHADER);
	AttachShader("noiseProgram", "noiseProgram-vert", "noiseProgram-frag");
	LinkProgram("noiseProgram");
	CompileShader("noiseProgram");

	glUseProgram(programs["noiseProgram"].id);
	GLint loc = glGetUniformLocation(programs["noiseProgram"].id, "terrain");
	glUniform1i(loc, 0);

	GLuint texCoordLoc = glGetAttribLocation(programs["noiseProgram"].id, "texCoord");

	glEnableVertexAttribArray(texCoordLoc);
	glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, 0, 0, 0);

	CreateProgram("blueProgram");
	CreateShader("blueProgram-vert", "vertex3.glsl", GL_VERTEX_SHADER);
	AttachShader("blueProgram", "blueProgram-vert", "default-frag");
	LinkProgram("blueProgram");
	CompileShader("blueProgram");

	glUseProgram(programs["blueProgram"].id);
	loc = glGetUniformLocation(programs["blueProgram"].id, "newColor");
	glUniform4f(loc, 0.0, 0.0, 1.0, 1.0);

	CreateProgram("DiffuseProgram");
	CreateShader("Diffuse-vert", "PhongVertex.glsl", GL_VERTEX_SHADER);
	CreateShader("Diffuse-frag", "PhongFragment.glsl", GL_FRAGMENT_SHADER);
	AttachShader("DiffuseProgram", "Diffuse-vert", "Diffuse-frag");
	LinkProgram("DiffuseProgram");
	CompileShader("DiffuseProgram");

	glUseProgram(programs["DiffuseProgram"].id);
	loc = glGetUniformLocation(programs["DiffuseProgram"].id, "position");
	glUniform3f(loc, 0.0, 0.0, 10.0);

	CreateProgram("CoolTeapotProgram");
	CreateShader("CoolTeapot-vert", "CoolTeapotVertex.glsl", GL_VERTEX_SHADER);
	CreateShader("CoolTeapot-frag", "CoolTeapotFragment.glsl", GL_FRAGMENT_SHADER);
	AttachShader("CoolTeapotProgram", "CoolTeapot-vert", "CoolTeapot-frag");
	LinkProgram("CoolTeapotProgram");
	CompileShader("CoolTeapotProgram");

	currentProgram = "defaultProgram";
	inited = true;
}

void Game::Cleanup(){
	if(!inited){
		return;
	}

	for(auto it = objects.begin() ; it != objects.end(); ++it){
		delete (*it);
	} 
	objects.clear();

	for(auto it = cameras.begin() ; it != cameras.end(); ++it){
		delete (*it);
	} 
	cameras.clear();

	for(auto it = connectors.begin(); it != connectors.end(); ++it) {
		delete (*it);
	}
	connectors.clear();

	DetachShaders("DiffuseProgram");
	DetachShaders("blueProgram");
	DetachShaders("noiseProgram");
	DetachShaders("defaultProgram");
	DetachShaders("PostProcessProgram");
	
	delete starting;

	inited = false;
	stopped = true;
}

Game::~Game(){}

Game::Game() : mID(msID){
	msID++;
}

void Game::GameLoop(){
	while(!stopped){
		glutMouseFunc(MouseInputWrapper);
		glutMotionFunc(MoveCameraWrapper);
		glutKeyboardFunc(KeyboardInputWrapper);
		glutSpecialFunc(SpecialKeyboardWrapper);
		glutIdleFunc(UpdateWrapper);
		glutDisplayFunc(DisplayWrapper);
		glutReshapeFunc(ReshapeWrapper);
		ClearGL();
		glutMainLoop();
	}
}

void Game::DrawFBO(std::string name){
	glMatrixMode(GL_MODELVIEW);
	ResetPosition();

	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbos[name].fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glBlitFramebuffer(0, 0, screenWidth, screenHeight, 0, 0, screenWidth, screenHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void Game::Update(){
	Animate(starting, "BasicSkele");

	starting->elapsedTime += (deltaTime.count() / 1000) * starting->dir;
	timeElapsed += deltaTime.count();

	//TODO: Update each object
	FindObject("Teapot")->Update(deltaTime.count() / 1000);
}

void Game::Display(){

	oldTime = newTime;
	newTime = std::chrono::high_resolution_clock::now();
	deltaTime = newTime - oldTime;

	int format = (FI_RGBA_RED == 0) ? GL_RGB : GL_BGR;

	for(int i = 0; i < PASS_NUM; i++){
		//Pass 1: Draw the objects
		if(i == 0){
			glBindFramebuffer(GL_FRAMEBUFFER, fbos["offscreen"].fbo);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glMatrixMode(GL_MODELVIEW);

			glUseProgram(programs[currentProgram].id);

			if(textures["terrain"].data){
				glEnable(GL_TEXTURE_2D);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textures["terrain"].width, textures["terrain"].height, 
							 0, format, GL_UNSIGNED_BYTE, textures["terrain"].data);
			}
			else {
				glDisable(GL_TEXTURE_2D);
			}

			for(unsigned int c = 0; c < objects.size(); c++){
				ResetPosition();
				objects[c]->Draw();
			}
			for(unsigned int c = 0; c < connectors.size(); c++){
				ResetPosition();
				connectors[c]->Draw();
			}
			for(std::map<std::string, Skeleton>::iterator it = skeletons.begin(); it != skeletons.end(); ++it){
				//Draw the skeleton
				for(int j = 0; j < it->second.parts.size(); j++){
					ResetPosition();
					it->second.parts[j]->Draw();
				}
				for(int j = 0; j < it->second.connectors.size(); j++){
					ResetPosition();
					it->second.connectors[j]->Draw();
				}
			}
		}
		//Pass Almost Max: Draw Colliders
		if(i == PASS_NUM - 2){
			DrawFBO("offscreen");
			glBindFramebuffer(GL_FRAMEBUFFER, fbos["offscreen"].fbo);
			glUseProgram(programs["defaultProgram"].id);
			if(drawColliders){
				for(unsigned int c = 0; c < objects.size(); c++){
					ResetPosition();
					if(objects[c]->GetCollider() != nullptr){
						if(objects[c]->GetParent() != nullptr){
							objects[c]->GetParent()->TranslateFromParent();
						}
						objects[c]->GetCollider()->DrawCollider();
					}
				}
			}
		}
		//Pass Max: Draw Text
		if(i == PASS_NUM - 1){
			DrawFBO("offscreen");
			glBindFramebuffer(GL_FRAMEBUFFER, fbos["offscreen"].fbo);
			glUseProgram(programs["defaultProgram"].id);

			if(skeleChoose){
				DrawText(Vector3(10.0f, (float)(screenHeight - 20), 0.0f), red, "Current selected skelepart: " + skeletons["BasicSkele"].parts[partIndex]->name);
			}
			else {
				DrawText(Vector3(10.0f, (float)(screenHeight - 20), 0.0f), red, "Current selected object: " + objects[objectIndex]->name);
			}
			DrawText(Vector3(10.0f, (float)(screenHeight - 40), 0.0f), red, "Current Shader: " + currentProgram);

			if(!showInfo){
				DrawText(Vector3(10.0f, (float)(screenHeight - 60), 0.0f), red, "Press i to show controls");
			}
			else {
				DrawText(Vector3(10.0f, (float)(screenHeight - 60), 0.0f), red, "Press i to hide controls");
				DrawText(Vector3(10.0f, (float)(screenHeight - 80), 0.0f), red, "1, 2, 3 to control X, Y, Z.");
				if(skeleChoose){
					DrawText(Vector3(10.0f, (float)(screenHeight - 100), 0.0f), red, "Current X, Y, Z Position: " + std::to_string(skeletons["BasicSkele"].parts[partIndex]->GetPos().x) + ", " +
						std::to_string(skeletons["BasicSkele"].parts[partIndex]->GetPos().y) + ", " + std::to_string(skeletons["BasicSkele"].parts[partIndex]->GetPos().z));
					DrawText(Vector3(10.0f, (float)(screenHeight - 160), 0.0f), red, "Current X, Y, Z Rotation: " + std::to_string(skeletons["BasicSkele"].parts[partIndex]->GetRot().x) + ", " +
						std::to_string(skeletons["BasicSkele"].parts[partIndex]->GetRot().y) + ", " + std::to_string(skeletons["BasicSkele"].parts[partIndex]->GetRot().z));
				}
				else {
					DrawText(Vector3(10.0f, (float)(screenHeight - 100), 0.0f), red, "Current X, Y, Z Position: " + std::to_string(objects[objectIndex]->GetPos().x) + ", " +
						std::to_string(objects[objectIndex]->GetPos().y) + ", " + std::to_string(objects[objectIndex]->GetPos().z));
					DrawText(Vector3(10.0f, (float)(screenHeight - 160), 0.0f), red, "Current X, Y, Z Rotation: " + std::to_string(objects[objectIndex]->GetRot().x) + ", " +
						std::to_string(objects[objectIndex]->GetRot().y) + ", " + std::to_string(objects[objectIndex]->GetRot().z));
				}
				
				DrawText(Vector3(10.0f, (float)(screenHeight - 120), 0.0f), red, "!, @, # to control -X, -Y, -Z");
				DrawText(Vector3(10.0f, (float)(screenHeight - 140), 0.0f), red, "4, 5, 6 to control rotational X, Y, Z");

				DrawText(Vector3(10.0f, (float)(screenHeight - 180), 0.0f), red, "$, %, ^ to control rotational -X, -Y, -Z");
				DrawText(Vector3(10.0f, (float)(screenHeight - 200), 0.0f), red, "Up/Down to select objects");
				DrawText(Vector3(10.0f, (float)(screenHeight - 220), 0.0f), red, "Right/Left to select cameras");
				DrawText(Vector3(10.0f, (float)(screenHeight - 240), 0.0f), red, "Shift to change control of skeletons/objects");
				DrawText(Vector3(10.0f, (float)(screenHeight - 260), 0.0f), red, "Enter to change shader");
				DrawText(Vector3(10.0f, (float)(screenHeight - 280), 0.0f), red, "WASD to control camera movement");
				DrawText(Vector3(10.0f, (float)(screenHeight - 300), 0.0f), red, "c to draw colliders");
				DrawText(Vector3(10.0f, (float)(screenHeight - 320), 0.0f), red, "z to start/stop. x to reverse/stop");
				if(starting->dir == -1){
					DrawText(Vector3(10.0f, (float)(screenHeight - 340), 0.0f), red, "Reverse. Time elapsed: " + std::to_string(starting->elapsedTime));
				}
				else if(starting->dir == 0){
					DrawText(Vector3(10.0f, (float)(screenHeight - 340), 0.0f), red, "Stopped. Time elapsed: " + std::to_string(starting->elapsedTime));
				}
				else if (starting->dir == 1){
					DrawText(Vector3(10.0f, (float)(screenHeight - 340), 0.0f), red, "Forward. Time elapsed: " + std::to_string(starting->elapsedTime));
				}
			}
		}
	}

	//Draw the FBO and swap buffers
	DrawFBO("offscreen");

	glutSwapBuffers();
}

//Animate a transform clip
void Game::Animate(Clip<Transform>* clip, std::string skeleName){
	auto it = std::next(clip->keys.begin(), clip->index);

	//Depending on the direction of the clip, move the skeleton
	switch(clip->dir){
	case 0:
		//If we are stopped just break
		break;
	case 1:
		if(clip->elapsedTime <= it->loadupTime){
			auto it2 = std::next(clip->keys.begin(), 0);
			if(clip->index > 0){
				it2 = std::next(clip->keys.begin(), clip->index);
			}
			for(int i = 0; i < it->arraySize; i++){
				switch(clip->blendType){
				case BlendType::BlendLerp:
					//If we are moving forward and are in the current clip, move/rotate/scale the skeleton over time
					MoveObjectLerp(FindIndexSkeleton(it->data[i].objName, skeleName), it->data[i].pos, clip->elapsedTime / 100, skeleName, clip->posRelative);
					RotateObjectLerp(FindIndexSkeleton(it->data[i].objName, skeleName), it->data[i].rot, skeleName, clip->elapsedTime / 100);
					ScaleObjectLerp(FindIndexSkeleton(it->data[i].objName, skeleName), it->data[i].scale, skeleName, clip->elapsedTime / 100);
					break;
				case BlendType::Add:
					//Add two poses together. If we are at size zero use the base pose to add
					if(clip->index == 0){
						MoveObjectAdd(FindIndexSkeleton(it->data[i].objName, skeleName), skeletons[skeleName].basePose[i].pos, it->data[i].pos);
						RotateObjectAdd(FindIndexSkeleton(it->data[i].objName, skeleName), skeletons[skeleName].basePose[i].rot, it->data[i].rot);
						ScaleObjectAdd(FindIndexSkeleton(it->data[i].objName, skeleName), skeletons[skeleName].basePose[i].scale, it->data[i].scale);
					}
					else {
						MoveObjectAdd(FindIndexSkeleton(it->data[i].objName, skeleName), it2->data[i].pos, it->data[i].pos);
						RotateObjectAdd(FindIndexSkeleton(it->data[i].objName, skeleName), it2->data[i].rot, it->data[i].rot);
						ScaleObjectAdd(FindIndexSkeleton(it->data[i].objName, skeleName), it2->data[i].scale, it->data[i].scale);
					}
					break;
				case BlendType::Average:
					break;
				case BlendType::Scale:
					break;
				}
				
			}
		}
		//If we are at the end of the clip, move in reverse
		else if(clip->index + 1 == clip->keys.size()){
			clip->dir = ClipDirection::Reverse;
			if(clip->keys.size() > 1){
				clip->index = clip->keys.size() - 1;
			}
			it = std::next(clip->keys.begin(), clip->index);
			clip->elapsedTime = it->loadupTime;
		}
		//If none of the above are true move to the next keyframe
		else {
			clip->index++;
			clip->elapsedTime = 0;
		}
		break;
	case -1:
		//If we are moving in reverse and are in the current clip, move/rotate/scale the skeleton over time (time > 0)
		if(clip->elapsedTime > 0){
			//If the size is one than animate based on the base pose position
			if(clip->keys.size() == 1){
				auto it2 = std::next(clip->keys.begin(), 0);
				if(clip->index + 1 < clip->keys.size()){
					it2 = std::next(clip->keys.begin(), clip->index + 1);
				}
				for(int i = 0; i < it->arraySize; i++){
					switch (clip->blendType){
					case BlendType::BlendLerp:
						//Move the pose with respect to the root (zero is root) then rotate/scale over time
						if(i == 0){
							//MoveObjectLerp(FindIndexSkeleton(it->data[i].objName, skeleName), skeletons[skeleName].parts[i]->GetPos(false), clip->elapsedTime / 100, skeleName, clip->posRelative);
						}
						else {
							MoveObjectLerp(FindIndexSkeleton(it->data[i].objName, skeleName), skeletons[skeleName].basePose[i].pos - skeletons[skeleName].parts[i]->GetPos(true), clip->elapsedTime / 100, skeleName, clip->posRelative);
						}
						RotateObjectLerp(FindIndexSkeleton(it->data[i].objName, skeleName), skeletons[skeleName].basePose[i].rot, skeleName, clip->elapsedTime / 100);
						ScaleObjectLerp(FindIndexSkeleton(it->data[i].objName, skeleName), skeletons[skeleName].basePose[i].scale, skeleName, clip->elapsedTime / 100);
						break;
					case BlendType::Add:
						//Add two poses together. If we are at the max size use the base pose to add
						if(clip->index == clip->keys.size() - 1){
							MoveObjectAdd(FindIndexSkeleton(it->data[i].objName, skeleName), skeletons[skeleName].basePose[i].pos, it->data[i].pos);
							RotateObjectAdd(FindIndexSkeleton(it->data[i].objName, skeleName), skeletons[skeleName].basePose[i].rot, it->data[i].rot);
							ScaleObjectAdd(FindIndexSkeleton(it->data[i].objName, skeleName), skeletons[skeleName].basePose[i].scale, it->data[i].scale);
						}
						else {
							MoveObjectAdd(FindIndexSkeleton(it->data[i].objName, skeleName), it2->data[i].pos, it->data[i].pos);
							RotateObjectAdd(FindIndexSkeleton(it->data[i].objName, skeleName), it2->data[i].rot, it->data[i].rot);
							ScaleObjectAdd(FindIndexSkeleton(it->data[i].objName, skeleName), it2->data[i].scale, it->data[i].scale);
						}
						break;
					case BlendType::Average:
						break;
					case BlendType::Scale:
						break;
					}
				}
			}
			//Otherwise use the last position to animate
			else {
				auto it2 = std::next(clip->keys.begin(), 0);
				switch (clip->blendType){
				case BlendType::BlendLerp:
					if(clip->index != 0){
						it2 = std::next(clip->keys.begin(), clip->index - 1);
					}
					break;
				case BlendType::Add:
					if(clip->index + 1 < clip->keys.size()){
						it2 = std::next(clip->keys.begin(), clip->index + 1);
					}
					break;

				}
				for(int i = 0; i < it->arraySize; i++){
					switch(clip->blendType){
					case BlendType::BlendLerp:
						MoveObjectLerp(FindIndexSkeleton(it->data[i].objName, skeleName), it2->data[i].pos, (it->loadupTime - clip->elapsedTime) / 100, skeleName, clip->posRelative);
						RotateObjectLerp(FindIndexSkeleton(it->data[i].objName, skeleName), it2->data[i].rot, skeleName, (it->loadupTime - clip->elapsedTime) / 100);
						ScaleObjectLerp(FindIndexSkeleton(it->data[i].objName, skeleName), it2->data[i].scale, skeleName, (it->loadupTime - clip->elapsedTime) / 100);
						break;
					case BlendType::Add:
						if(clip->index == clip->keys.size() - 1){
							MoveObjectAdd(FindIndexSkeleton(it->data[i].objName, skeleName), skeletons[skeleName].basePose[i].pos, it->data[i].pos);
							RotateObjectAdd(FindIndexSkeleton(it->data[i].objName, skeleName), skeletons[skeleName].basePose[i].rot, it->data[i].rot);
							ScaleObjectAdd(FindIndexSkeleton(it->data[i].objName, skeleName), skeletons[skeleName].basePose[i].scale, it->data[i].scale);
						}
						else {
							MoveObjectAdd(FindIndexSkeleton(it->data[i].objName, skeleName), it2->data[i].pos, it->data[i].pos);
							RotateObjectAdd(FindIndexSkeleton(it->data[i].objName, skeleName), it2->data[i].rot, it->data[i].rot);
							ScaleObjectAdd(FindIndexSkeleton(it->data[i].objName, skeleName), it2->data[i].scale, it->data[i].scale);
						}
						break;
					case BlendType::Average:
						break;
					case BlendType::Scale:
						break;
					}
				}
			}
		}
		//If we are at the beginning, move it in forward motion
		else if(it == clip->keys.begin()){
			clip->dir = ClipDirection::Forward;
			clip->elapsedTime = 0;
			if(clip->keys.size() != 1){
				clip->index = 1;
			}
		}
		//Otherwise just subtract from the index and reset time
		else {
			clip->index--;
			it = std::next(clip->keys.begin(), clip->index);
			clip->elapsedTime = it->loadupTime;
		}
		break;
	}
}

//Helper functions to stop/start/switch animation clips
template<class T>
void Game::StopAnimation(Clip<T>* clip){
	clip->dir = ClipDirection::Stop;
}

template<class T>
void Game::StartAnimation(Clip<T>* clip){
	clip->dir = ClipDirection::Forward;
}

template<class T>
void Game::SwitchForward(Clip<T>* clip){
	if(clip->dir != ClipDirection::Stop){
		clip->dir = ClipDirection::Stop;
	}
	else {
		clip->dir = ClipDirection::Forward;
	}
}

template<class T>
void Game::SwitchReverse(Clip<T>* clip){
	if(clip->dir != ClipDirection::Stop){
		clip->dir = ClipDirection::Stop;
	}
	else {
		clip->dir = ClipDirection::Reverse;
	}
}

//These are menu options (right click). The examples given are to spawn objects, a camera, or attach a collider to the current object
void Game::Menu(int id){
	switch(id){
	case 1:
		NewObject(GameObject::Type::CUBE, red, Vector3(2, 0.0f + (spawnNumber * 1.1f), -10.0f), VECTOR3_ONE, VECTOR3_ZERO, "Cube" + std::to_string(spawnNumber), false);
		spawnNumber++;
		break;
	case 2:
		NewObject(GameObject::Type::SPHERE, red, Vector3(2, 0.0f + (spawnNumber * 1.1f), -10.0f), Vector3(0.5f, 0.5f, 0.5f), VECTOR3_ZERO, "Sphere" + std::to_string(spawnNumber), false);
		spawnNumber++;
		break;
	case 3:
		NewObject(GameObject::Type::PYRAMID, red, Vector3(2, 0.0f + (spawnNumber * 1.1f), -10.0f), Vector3(0.5f, 0.5f, 0.5f), VECTOR3_ZERO, "Pyramid" + std::to_string(spawnNumber), false);
		spawnNumber++;
		break;
	case 4:
		NewCamera(Vector3(0, 3, 1), Vector3(0, 0, -1));
		break;
	case 5:
		objects[objectIndex]->AttachBoxCollider();
		break;
	}
}

//This is a menu to set the color of the object
void Game::ColorMenu(int id){
	switch(id){
	case 1:
		objects[objectIndex]->SetColor(red);
		break;
	case 2:
		objects[objectIndex]->SetColor(green);
		break;
	case 3:
		objects[objectIndex]->SetColor(blue);
		break;
	}
}

//This will just translate the current object and check collisions. If it collides move it back the same amount (to prevent clipping)
void Game::MoveCurrentObject(Vector3 pos, bool relative){
	if(skeleChoose){
		skeletons["BasicSkele"].parts[partIndex]->Translate(pos, relative);
	}
	else {
		objects[objectIndex]->Translate(pos, relative);
		GameObject* collider = CheckAllCollisions();
		if(collider){
			if(!collider->isStatic){
				collider->Translate(Vector3(pos), relative);
			}
			else {
				objects[objectIndex]->Translate(Vector3(pos * -1.0f), relative);
			}
		}
	}
	
}

//This rotates the current object (Todo: Check collisions on children)
void Game::RotateCurrentObject(Vector3 rot, bool relative){
	if(skeleChoose){
		skeletons["BasicSkele"].parts[partIndex]->Rotate(rot, relative);
	}
	else {
		objects[objectIndex]->Rotate(rot, relative);
	}
}

//This checks all collisions ofr the current object
GameObject* Game::CheckAllCollisions(){
	for(unsigned int i = 0; i < objects.size(); i++){
		if(objects[objectIndex] != objects[i]){
			if(objects[objectIndex]->GetCollider() != nullptr && objects[i]->GetCollider() != nullptr){
				if(objects[objectIndex]->GetCollider()->CheckCollision(objects[i])){
					return objects[i];
				}
			}
		}
	}
	return false;
}

//This checks all collisions for object at index
GameObject* Game::CheckAllCollisions(int objIndex){
	for(unsigned int i = 0; i < objects.size(); i++){
		if(objects[objIndex] != objects[i]){
			if(objects[objectIndex]->GetType() != GameObject::Type::CONNECTOR && objects[i]->GetType() != GameObject::Type::CONNECTOR){
				if(objects[objIndex]->GetCollider()->CheckCollision(objects[i])){
					return objects[i];
				}
			}
		}
	}
	return false;
}

//Draw text helper function
void Game::DrawText(Vector3 pos, Vector3 color, std::string text, void* font){
	ResetPosition();
	glColor3f(color.x, color.y, color.z);
	glWindowPos3f(pos.x, pos.y, pos.z);
	const unsigned char* newText = reinterpret_cast<const unsigned char *>(text.c_str());
	glutBitmapString(font, newText);
}

//Instance functions (so game can be accessed anywhere)
Game* Game::GetInstance(){
	assert(instance != NULL);
	return instance;
}

void Game::InitInstance(){
	assert(instance == NULL);
	instance = new Game();
}

void Game::CleanupInstance(){
	assert(instance != NULL);
	delete instance;
	instance = NULL;
}

//Create and FBO
void Game::CreateBasicFBO(std::string name){

	//Generate the things we need for fbos
	glGenFramebuffers(1, &fbos[name].fbo);
	glGenTextures(1, &fbos[name].texture);
	glGenRenderbuffers(1, &fbos[name].fbo);

	//Bind it so we can use it. Create the whole screen as the texture width/height
	glBindFramebuffer(GL_FRAMEBUFFER, fbos[name].fbo);

	glBindTexture(GL_TEXTURE_2D, fbos[name].texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	//Parameters for texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbos[name].texture, 0);

	//Bind the renderbuffer to the FBO
	glBindRenderbuffer(GL_RENDERBUFFER, fbos[name].fbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, screenWidth, screenHeight);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbos[name].fbo);

	//If its complete state it on the console
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE){
		std::cout << "Created framebuffer with name: " << name << std::endl;
	}
	else {
		std::cout << "Error creating framebuffer with name: " << name << std::endl;
	}
}

//Create a texture
void Game::CreateTexture(std::string fileName, std::string texName){

	//Creates bitmap images
	textures[texName].data = 0;

	//Load it in from freeimage
	FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(fileName.c_str());

	FIBITMAP* bitmap = FreeImage_Load(format, fileName.c_str(), 0);

	//Error checking
	if(!bitmap){
		std::cout << "Failed to load image: " + fileName << std::endl;
		return;
	}

	//Convert it to 24 bits
	FIBITMAP* bitmap2 = FreeImage_ConvertTo24Bits(bitmap);
	FreeImage_Unload(bitmap);

	//Set the texture data
	textures[texName].data = FreeImage_GetBits(bitmap2);
	textures[texName].width = FreeImage_GetWidth(bitmap2);
	textures[texName].height = FreeImage_GetHeight(bitmap2);

	//If we loaded it properly output to console
	if(textures[texName].data){
		std::cout << "Loaded image: " + fileName << std::endl;
	}
	else {
		std::cout << "Failed to get image data: " + fileName << std::endl;
	}
}

//Create a program using the given name
void Game::CreateProgram(std::string name){
	programs[name].id = glCreateProgram();
	if(programs[name].id == 0){
		std::cout << "ERROR CREATING SHADER PROGRAM: " + name << std::endl;
	}
	else {
		std::cout << "Created program: " + name << std::endl;
	}
}

//Create a shader using the current filepath and type
void Game::CreateShader(std::string name, const char* path, GLenum type){
	shaders[name] = glCreateShader(type);

	std::string contents = ReadFile(path);

	char const* newContents = contents.c_str();
	glShaderSource(shaders[name], 1, &newContents, NULL);

	std::cout << "Created shader: " + name << std::endl;
}

//Compile an already loaded shader
void Game::CompileShader(std::string name){
	glCompileShader(shaders[name]);
	GLint compiled;
	glGetShaderiv(shaders[name], GL_COMPILE_STATUS, &compiled);

	if(!compiled){
		std::cout << "ERROR! Shader: " + name + " DOES NOT COMPILE!\n";
	}
	else {
		std::cout << "Compiled shader: " + name << std::endl;
	}
}

//Attach two shaders (vertex and fragment) to an existing program
void Game::AttachShader(std::string programName, std::string vertexName, std::string fragmentName){
	glAttachShader(programs[programName].id, shaders[vertexName]);
	glAttachShader(programs[programName].id, shaders[fragmentName]);

	programs[programName].fragName = fragmentName;
	programs[programName].vertName = vertexName;
}

//Link a program all together
void Game::LinkProgram(std::string programName){
	int programId = programs[programName].id;

	glLinkProgram(programId);

	GLint isLinked = 0;
	glGetProgramiv(programId, GL_LINK_STATUS, &isLinked);
	if(!isLinked){
		std::cout << "ERROR! Program: " + programName + " DOES NOT LINK!\n";
	}
	else {
		std::cout << "Compiled Program: " + programName << std::endl;
	}
}

//Detach the shaders from said program (but keep the name referenced)
void Game::DetachShaders(std::string programName){
	int programId = programs[programName].id;

	glDetachShader(programId, shaders[programs[programName].fragName]);
	glDetachShader(programId, shaders[programs[programName].vertName]);
}

//Keyboard wrappers
void Game::KeyboardInputWrapper(unsigned char key, int x, int y){
	instance->KeyboardInput(key, x, y);
}

void Game::SpecialKeyboardWrapper(int key, int x, int y){
	instance->SpecialKeyboard(key, x, y);
}

//Read a file using a path name
std::string Game::ReadFile(std::string path){
	std::ifstream file;

	file.open(path);

	if(!file.is_open()){
		std::cout << "ERROR! COULD NOT READ FILE!" << path << std::endl;
		return nullptr;
	}

	std::stringstream buffer;
	std::string line;
	while(std::getline(file, line)){
		buffer << line << "\n";
	}

	file.close();
	return buffer.str();
}

//Lerp an object over time (either relative or world space)
void Game::MoveObjectLerp(int index, Vector3 newMove, float time, bool relative){
	if(relative){
		if(newMove == VECTOR3_ZERO){
			return;
		}
		objects[index]->Translate(Lerp(VECTOR3_ZERO, newMove, time), true);
	}
	else {
		if(newMove == objects[index]->GetPos()){
			return;
		}
		objects[index]->Translate(Lerp(objects[index]->GetPos(), newMove, time) - objects[index]->GetPos(), true);
	}
}

//Move a skelepart over time (either relative or world space)
void Game::MoveObjectLerp(int index, Vector3 newMove, float time, std::string skeleName, bool relative){
	if(relative){
		if(newMove == VECTOR3_ZERO){
			return;
		}
		skeletons[skeleName].parts[index]->Translate(Lerp(VECTOR3_ZERO, newMove, time), true);
	}
	else {
		if(newMove == objects[index]->GetPos()){
			return;
		}
		skeletons[skeleName].parts[index]->Translate(Lerp(skeletons[skeleName].parts[index]->GetPos(), newMove, time) - skeletons[skeleName].parts[index]->GetPos(), true);
	}
}

//Rotate an object over time
void Game::RotateObjectLerp(int index, Vector3 newRot, float time){
	objects[index]->Rotate(Lerp(objects[index]->GetRot(true), newRot, time));
}

//Rotate a skelepart over time
void Game::RotateObjectLerp(int index, Vector3 newRot, std::string skeleName, float time){
	skeletons[skeleName].parts[index]->Rotate(Lerp(skeletons[skeleName].parts[index]->GetRot(true), newRot, time));
}

//Scale an object over time
void Game::ScaleObjectLerp(int index, Vector3 newScale, float time){
	Vector3 oldScale = objects[index]->GetScale(true);
	newScale = Vector3(oldScale.x * newScale.x, oldScale.y * newScale.y, oldScale.z * newScale.z);
	objects[index]->Scale(Lerp(objects[index]->GetScale(true), newScale, time));
}

//Scale a skeleton over time
void Game::ScaleObjectLerp(int index, Vector3 newScale, std::string skeleName, float time){
	Vector3 oldScale = skeletons[skeleName].parts[index]->GetScale(true);
	newScale = Vector3(oldScale.x * newScale.x, oldScale.y * newScale.y, oldScale.z * newScale.z);
	skeletons[skeleName].parts[index]->Scale(Lerp(skeletons[skeleName].parts[index]->GetScale(true), newScale, time));
}

//Move an object by adding two poses
void Game::MoveObjectAdd(int index, Vector3 pose0, Vector3 pose1){
	objects[index]->Translate(pose0 + pose1, false);
}

//Move a skelepart by adding two poses
void Game::MoveObjectAdd(int index, Vector3 pose0, Vector3 pose1, std::string skeleName){
	skeletons[skeleName].parts[index]->Translate(pose0 + pose1, false);
}

//Rotate an object by adding two rotations
void Game::RotateObjectAdd(int index, Vector3 pose0, Vector3 pose1){
	objects[index]->Rotate(pose0 + pose1);
}

//Rotate a skelepart by adding two rotations
void Game::RotateObjectAdd(int index, Vector3 pose0, Vector3 pose1, std::string skeleName){
	skeletons[skeleName].parts[index]->Rotate(pose0 + pose1);
}

void Game::ScaleObjectAdd(int index, Vector3 pose0, Vector3 pose1){
	objects[index]->Scale(Vector3(pose0.x * pose1.x, pose0.y * pose1.y, pose0.z * pose1.z));
}

void Game::ScaleObjectAdd(int index, Vector3 pose0, Vector3 pose1, std::string skeleName){
	skeletons[skeleName].parts[index]->Scale(Vector3(pose0.x * pose1.x, pose0.y * pose1.y, pose0.z * pose1.z));
}

//Basic keyboard inputs
void Game::KeyboardInput(unsigned char key, int x, int y){
	//Calculate the up, and right vectors for the camera (based on rotation)
	float fraction = 0.15f;
	Vector3* right = new Vector3(0, 0, 0);
	Vector3* up = new Vector3(0, 0, 0);
	Vector3 worldUp(0, 1, 0);
	CrossProduct(*cameras[cameraIndex]->rot, worldUp, right);
	CrossProduct(*right, *cameras[cameraIndex]->rot, up);

	std::map<std::string, Program>::iterator it;

	switch(key){
		//Escape button is pressed. Die.
	case 27:
		Cleanup();
		exit(0);
		break;

		//Move the camera
		//Shift key (the captial key names) move it faster (by a factor of two)
	case 'w':
		cameras[cameraIndex]->pos->x += cameras[cameraIndex]->rot->x * fraction;
		cameras[cameraIndex]->pos->y += cameras[cameraIndex]->rot->y * fraction;
		cameras[cameraIndex]->pos->z += cameras[cameraIndex]->rot->z * fraction;
		break;
	case 'W':
		cameras[cameraIndex]->pos->x += cameras[cameraIndex]->rot->x * fraction * 2;
		cameras[cameraIndex]->pos->y += cameras[cameraIndex]->rot->y * fraction * 2;
		cameras[cameraIndex]->pos->z += cameras[cameraIndex]->rot->z * fraction * 2;
		break;
	case 'a':
		cameras[cameraIndex]->pos->x -= right->x * fraction;
		cameras[cameraIndex]->pos->z -= right->z * fraction;
		break;
	case 'A':
		cameras[cameraIndex]->pos->x -= right->x * fraction * 2;
		cameras[cameraIndex]->pos->z -= right->z * fraction * 2;
		break;
	case 'd':
		cameras[cameraIndex]->pos->x += right->x * fraction * 2;
		cameras[cameraIndex]->pos->z += right->z * fraction * 2;
		break;
	case 'D':
		cameras[cameraIndex]->pos->x += right->x * fraction * 2;
		cameras[cameraIndex]->pos->z += right->z * fraction * 2;
		break;
	case 's':
		cameras[cameraIndex]->pos->x -= cameras[cameraIndex]->rot->x * fraction;
		cameras[cameraIndex]->pos->y -= cameras[cameraIndex]->rot->y * fraction;
		cameras[cameraIndex]->pos->z -= cameras[cameraIndex]->rot->z * fraction;
		break;
	case 'S':
		cameras[cameraIndex]->pos->x -= cameras[cameraIndex]->rot->x * fraction * 2;
		cameras[cameraIndex]->pos->y -= cameras[cameraIndex]->rot->y * fraction * 2;
		cameras[cameraIndex]->pos->z -= cameras[cameraIndex]->rot->z * fraction * 2;
		break;
	case 'q':
		cameras[cameraIndex]->pos->y -= up->y * fraction;
		break;
	case 'e':
		cameras[cameraIndex]->pos->y += up->y * fraction;
		break;
	case 'r':
		//cameras[cameraIndex]->doRotate = !cameras[cameraIndex]->doRotate;
		//cameras[cameraIndex]->SetRotateObject(objects[objectIndex]->GetPos());
		break;

	//i to show information
	case 'i':
		showInfo = !showInfo;
		break;

	//Numbers to control selected object
	case '1':
		MoveCurrentObject(Vector3(0.05f, 0, 0), true);
		break;
	case '!':
		MoveCurrentObject(Vector3(-0.05f, 0, 0), true);
		break;
	case '2':
		MoveCurrentObject(Vector3(0, 0.05f, 0), true);
		break;
	case '@':
		MoveCurrentObject(Vector3(0, -0.05f, 0), true);
		break;
	case '3':
		MoveCurrentObject(Vector3(0, 0, 0.05f), true);
		break;
	case '#':
		MoveCurrentObject(Vector3(0, 0, -0.05f), true);
		break;
	case '4':
		RotateCurrentObject(Vector3(1, 0, 0), true);
		break;
	case '$':
		RotateCurrentObject(Vector3(-1, 0, 0), true);
		break;
	case '5':
		RotateCurrentObject(Vector3(0, 1, 0), true);
		break;
	case '%':
		RotateCurrentObject(Vector3(0, -1, 0), true);
		break;
	case '6':
		RotateCurrentObject(Vector3(0, 0, 1), true);
		break;
	case '^':
		RotateCurrentObject(Vector3(0, 0, -1), true);
		break;

	//Enter to change shader
	case '\r':
		it = programs.find(currentProgram);
		it++;
		if(it == programs.end()){
			it = programs.begin();
		}
		currentProgram = it->first;
		break;

	//c to draw colliders
	case 'c':
		drawColliders = !drawColliders;
		break;

	//z switches from stop/forward
	case 'z':
		SwitchForward(starting);
		break;
	//x switches from stop/reverse
	case 'x':
		SwitchReverse(starting);
		break;
	default:
		break;
	}

	delete right;
	delete up;
}

//Special keyboard inputs (ups, downs, left, right)
void Game::SpecialKeyboard(int key, int x, int y) {
	switch (key) {
		//Up/down to change selected object
	case GLUT_KEY_UP:
		if(skeleChoose){
			partIndex++;
			if((unsigned int)partIndex >= skeletons["BasicSkele"].parts.size()){
				partIndex = 0;
			}
		}
		else {
			objectIndex++;
			if((unsigned int)objectIndex >= objects.size()){
				objectIndex = 0;
			}
			if(objects[objectIndex]->GetType() == GameObject::Type::CONNECTOR) objectIndex++;
			if((unsigned int)objectIndex >= objects.size()){
				objectIndex = 0;
			}
		}
		break;
	case GLUT_KEY_DOWN:
		if(skeleChoose){
			partIndex--;
			if(partIndex < 0){
				partIndex = (int)skeletons["BasicSkele"].parts.size() - 1;
			}
		}
		else {
			objectIndex--;
			if(objectIndex < 0){
				objectIndex = (int)objects.size() - 1;
			}
			if(objects[objectIndex]->GetType() == GameObject::Type::CONNECTOR) objectIndex--;
			if(objectIndex < 0){
				objectIndex = (int)objects.size() - 1;
			}
		}
		break;
	case GLUT_KEY_LEFT:
		cameraIndex++;
		if ((unsigned int)cameraIndex >= cameras.size()) {
			cameraIndex = 0;
		}
		break;
	case GLUT_KEY_RIGHT:
		cameraIndex--;
		if (cameraIndex < 0) {
			cameraIndex = (int)cameras.size() - 1;
		}
		break;

	case GLUT_KEY_SHIFT_L:
		skeleChoose = !skeleChoose;
		break;
	default:
		break;
	}
}

//Reset the position of GL and the camera
void Game::ResetPosition(){
	glLoadIdentity();
	cameras[cameraIndex]->ResetCamera();
}

//Find an object based on name
GameObject* Game::FindObject(std::string name){
	for(unsigned int i = 0; i < objects.size(); i++){
		if(objects[i]->name == name) return objects[i];
	}
	for(std::map<std::string, Skeleton>::iterator it = skeletons.begin(); it != skeletons.end(); ++it){
		for(unsigned int i = 0; i < it->second.parts.size(); i++){
			if (it->second.parts[i]->name == name) return it->second.parts[i];
		}
	}
	return nullptr;
}

//Find an index based on name
int Game::FindIndex(std::string name){
	for(unsigned int i = 0; i < objects.size(); i++){
		if (objects[i]->name == name) return i;
	}
	
	return -1;
}

//Find a skelepart based on name and skelename
int Game::FindIndexSkeleton(std::string partName, std::string skeleName){
	for(unsigned int i = 0; i < skeletons[skeleName].parts.size(); i++){
		if (skeletons[skeleName].parts[i]->name == partName) return i;
	}

	return -1;
}

//Move the camera
void Game::MoveCameraWrapper(int x, int y){
	instance->MoveCamera(x, y);
}

void Game::MoveCamera(int x, int y){
	cameras[cameraIndex]->Move(x, y, screenWidth, screenHeight);
}

//Wrappers (since we need to statically call each part but can't from the main)
void Game::DisplayWrapper(){
	instance->Display();
}

void Game::UpdateWrapper(){
	glutPostRedisplay();
	instance->Update();
}

void Game::ReshapeWrapper(GLsizei width, GLsizei height){
	instance->Reshape(width, height);
}

void Game::MenuWrapper(int id){
	instance->Menu(id);
}

void Game::ColorMenuWrapper(int id){
	instance->ColorMenu(id);
}

//Reshape the window
void Game::Reshape(GLsizei width, GLsizei height){

	if (height == 0) height = 1;
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

//Mouse input
void Game::MouseInputWrapper(int button, int state, int x, int y){
	instance->MouseInput(button, state, x, y);
}

void Game::MouseInput(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON){
		if(state == GLUT_UP){
			cameras[cameraIndex]->cameraX = -1;
			cameras[cameraIndex]->cameraY = -1;
		}
		else {
			cameras[cameraIndex]->cameraX = x;
			cameras[cameraIndex]->cameraY = y;
		}
	}
	else if(button == GLUT_MIDDLE_BUTTON){
		//Object selection here

		float z;
		double modelview[16], projection[16];
		int viewport[4];
		double objx, objy, objz;

		//Credit to https://www.experts-exchange.com/questions/24814973/gluunproject-with-glut-c.html for help with gluUnProject

		//Get depth
		glReadPixels(x, screenHeight - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);

		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

		glGetDoublev(GL_PROJECTION_MATRIX, projection);
		glGetIntegerv(GL_VIEWPORT, viewport);

		gluUnProject(x, viewport[3]-y, z, modelview, projection, viewport, &objx, &objy, &objz);
	}
}

void Game::NewObject(GameObject::Type newType, Vector3 newColor, Vector3 newPos, Vector3 newScale, Vector3 newRot, std::string name, bool isStatic, bool hasCollider){
	objects.push_back(new GameObject(newType, newColor, newPos, newScale, newRot, name, isStatic, hasCollider));
}

void Game::NewObject(GameObject::Type newType, Vector3 newColor, Vector3 newPos, Vector3 newScale, Vector3 newRot, std::string name, GameObject* parent, bool isStatic, bool hasCollider){
	objects.push_back(new GameObject(newType, newColor, newPos, newScale, newRot, name, parent, isStatic, hasCollider));
	parent->SetChild(objects[objects.size() - 1]);
}

void Game::NewObject(GameObject::Type newType, char* filename, Vector3 newColor, Vector3 newPos, Vector3 newScale, Vector3 newRot, std::string name, bool isStatic, bool hasCollider){
	objects.push_back(new GameObject(newType, filename, newColor, newPos, newScale, newRot, name, isStatic, hasCollider));
}

void Game::NewObject(Vector3 newPos, Vector3 newScale, Vector3 newRot, std::string newName){
	objects.push_back(new GameObject(newPos, newScale, newRot, newName));
}

void Game::NewObject(GameObject* one, GameObject* two){
	objects.push_back(new GameObject(one, two));
}

void Game::NewCamera(Vector3 pos, Vector3 direction){
	cameras.push_back(new Camera(pos, direction));
}

void Game::NewSkeletonPart(Vector3 newPos, Vector3 newRot, Vector3 newScale, std::string name, std::string skeleName, GameObject* parent){
	if(parent != nullptr){
		objects.push_back(new GameObject(GameObject::SPHERE, Vector3(1.0, 0.0, 1.0), newPos, Vector3(0.05f, 0.05f, 0.05f), newRot, name, parent, false, false));
		GameObject* child = objects[objects.size() - 1];
		parent->SetChild(child);
		connectors.push_back(new GameObject(parent, child));
	}
	else {
		objects.push_back(new GameObject(GameObject::SPHERE, Vector3(1.0, 0.0, 1.0), newPos, Vector3(0.05f, 0.05f, 0.05f), newRot, name, false, false));
	}
}

void Game::NewSkeletonPart(Vector3 newPos, Vector3 newRot, Vector3 newScale, std::string name, std::string skeleName, GameObject* parent, bool canTranslate, bool canRotate){

	Vector3 scaleTmp = Vector3(0.05f * newScale.x, 0.05f * newScale.y, 0.05f * newScale.z);

	if(parent != nullptr){
		skeletons[skeleName].parts.push_back(new GameObject(GameObject::SPHERE, Vector3(1.0, 0.0, 1.0), newPos, scaleTmp, newRot, name, parent, canTranslate, canRotate));
		GameObject* child = skeletons[skeleName].parts[skeletons[skeleName].parts.size() - 1];
		child->SetCanRotate(canRotate);
		child->SetCanTranslate(canTranslate);
		parent->SetChild(child);
		skeletons[skeleName].connectors.push_back(new GameObject(parent, child));
	}
	else {
		skeletons[skeleName].parts.push_back(new GameObject(GameObject::SPHERE, Vector3(1.0, 0.0, 1.0), newPos, Vector3(0.05f * newScale.x, 0.05f * newScale.y, 0.05f * newScale.z), newRot, name, canTranslate, canRotate));
		GameObject* child = skeletons[skeleName].parts[skeletons[skeleName].parts.size() - 1];
		child->SetCanRotate(canRotate);
		child->SetCanTranslate(canTranslate);
	}

	int index = skeletons[skeleName].parts.size() - 1;

	//TODO: MAKE ROOT RELATIVE
	Transform tmpTrans;
	tmpTrans.pos = newPos;
	tmpTrans.rot = newRot;
	tmpTrans.scale = newScale;

	skeletons[skeleName].basePose.push_back(tmpTrans);
	
}