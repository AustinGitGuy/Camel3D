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
#include "IL/ilut.h"

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

	newTime = oldTime = time(NULL);

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

	ilInit();

	ilutRenderer(ILUT_OPENGL);

	std::string filename = "diver.obj";

	//Create some basic objects and a camera
	//NewObject(GameObject::Type::CUBE, red, Vector3(1.5f, 0.0f, -10.0f), VECTOR3_ONE, Vector3(90, 0, 0), "Cube");
	NewObject(GameObject::Type::CUBE, white, Vector3(-1.5f, 0.5f, -10.0f), Vector3(5.0f, .01f, 5.0f), VECTOR3_ZERO, "Plane", true);
	NewObject(GameObject::Type::DODECAHEDRON, Vector3(1.0f, 1.0f, 0.0f), Vector3(-6.5f, 0.0f, -10.0f), VECTOR3_ONE, VECTOR3_ZERO, "Dodecahedron", false, false);
	NewObject(GameObject::Type::CUBE, blue, Vector3(-3.0f, -1.1f, -10.0f), Vector3(20.0f, .01f, 20.0f), VECTOR3_ZERO, "BasePlane", true);
	NewObject(GameObject::Type::TEAPOT, Vector3(0.0, 1.0f, 1.0f), Vector3(5.0f, 0.0f, -10.0f), VECTOR3_ONE, VECTOR3_ZERO, "Teapot", false);
	NewObject(GameObject::Type::OBJ, (char*)filename.c_str(), green, Vector3(-10.0f, -1.0f, -10.0f), Vector3(0.03f, 0.03f, 0.03f), VECTOR3_ZERO, "Diver", true);
	NewCamera(Vector3(0, 2, 1), Vector3(0, 0, -1));

	//Create the skeleton
	NewSkeletonPart(Vector3(-1.5f, 3, -6), VECTOR3_ZERO, "SkeleRoot");
	NewSkeletonPart(Vector3(0, -0.4f, 0), VECTOR3_ZERO, "SkeleNeck", FindObject("SkeleRoot"));
	NewSkeletonPart(Vector3(0, -0.2f, 0), VECTOR3_ZERO, "SkeleClavicle", FindObject("SkeleNeck"));
	NewSkeletonPart(Vector3(0, -0.8f, 0), VECTOR3_ZERO, "SkeleMidspine", FindObject("SkeleClavicle"));
	NewSkeletonPart(Vector3(0, -0.8f, 0), VECTOR3_ZERO, "SkeleLowerSpine", FindObject("SkeleMidspine"));

	NewSkeletonPart(Vector3(-.2f, -.2f, 0), VECTOR3_ZERO, "SkeleShoulderbladeR", FindObject("SkeleClavicle"));
	NewSkeletonPart(Vector3(-.2f, .2f, 0), VECTOR3_ZERO, "SkeleShoulderR", FindObject("SkeleShoulderbladeR"));
	NewSkeletonPart(Vector3(-.65f, -.5f, 0), VECTOR3_ZERO, "SkeleElbowR", FindObject("SkeleShoulderR"));
	NewSkeletonPart(Vector3(-.4f, -.3f, 0), VECTOR3_ZERO, "SkeleForearmR", FindObject("SkeleElbowR"));
	NewSkeletonPart(Vector3(-.4f, -.3f, 0), VECTOR3_ZERO, "SkeleWristR", FindObject("SkeleForearmR"));
	NewSkeletonPart(Vector3(-.1f, -.1f, 0), VECTOR3_ZERO, "SkeleHandR", FindObject("SkeleWristR"));

	NewSkeletonPart(Vector3(.2f, -.2f, 0), VECTOR3_ZERO, "SkeleShoulderbladeL", FindObject("SkeleClavicle"));
	NewSkeletonPart(Vector3(.2f, .2f, 0), VECTOR3_ZERO, "SkeleShoulderL", FindObject("SkeleShoulderbladeL"));
	NewSkeletonPart(Vector3(.65f, -.5f, 0), VECTOR3_ZERO, "SkeleElbowL", FindObject("SkeleShoulderL"));
	NewSkeletonPart(Vector3(.4f, -.3f, 0), VECTOR3_ZERO, "SkeleForearmL", FindObject("SkeleElbowL"));
	NewSkeletonPart(Vector3(.4f, -.3f, 0), VECTOR3_ZERO, "SkeleWristL", FindObject("SkeleForearmL"));
	NewSkeletonPart(Vector3(.1f, -.1f, 0), VECTOR3_ZERO, "SkeleHandL", FindObject("SkeleWristL"));

	NewSkeletonPart(Vector3(-.4f, 0.15f, 0), VECTOR3_ZERO, "SkeleHipR", FindObject("SkeleLowerSpine"));
	NewSkeletonPart(Vector3(0, -.8f, 0), VECTOR3_ZERO, "SkeleKneeR", FindObject("SkeleHipR"));
	NewSkeletonPart(Vector3(0, -.35f, 0), VECTOR3_ZERO, "SkeleShinR", FindObject("SkeleKneeR"));
	NewSkeletonPart(Vector3(0, -.35f, 0), VECTOR3_ZERO, "SkeleAnkleR", FindObject("SkeleShinR"));
	NewSkeletonPart(Vector3(0, -.05f, .15f), VECTOR3_ZERO, "SkeleFootR", FindObject("SkeleAnkleR"));
	NewSkeletonPart(Vector3(0, -.05f, .15f), VECTOR3_ZERO, "SkeleToesR", FindObject("SkeleFootR"));

	NewSkeletonPart(Vector3(.4f, 0.15f, 0), VECTOR3_ZERO, "SkeleHipL", FindObject("SkeleLowerSpine"));
	NewSkeletonPart(Vector3(0, -.8f, 0), VECTOR3_ZERO, "SkeleKneeL", FindObject("SkeleHipL"));
	NewSkeletonPart(Vector3(0, -.35f, 0), VECTOR3_ZERO, "SkeleShinL", FindObject("SkeleKneeL"));
	NewSkeletonPart(Vector3(0, -.35f, 0), VECTOR3_ZERO, "SkeleAnkleL", FindObject("SkeleShinL"));
	NewSkeletonPart(Vector3(0, -.05f, .15f), VECTOR3_ZERO, "SkeleFootL", FindObject("SkeleAnkleL"));
	NewSkeletonPart(Vector3(0, -.05f, .15f), VECTOR3_ZERO, "SkeleToesL", FindObject("SkeleFootL"));

	bgColor = gray;

	skeleRotations = new Vector3[SKELE_PARTS];

	for(int i = 0; i < SKELE_PARTS; i++){
		skeleRotations[i] = VECTOR3_ZERO;
	}

	skeleRotations[6] = Vector3(0, 0, -69);
	skeleRotations[7] = Vector3(0, 0, -140);

	//Generate FBOS
	CreateBasicFBO("offscreen");

	ilGenImages(1, &textures["terrain"]);
	ilBindImage(textures["terrain"]);
	ilLoadImage("terrain.bmp");

	//Create each program individually. Create - Attach - Link - Compile - Uniforms - Detach

	CreateProgram("defaultProgram");
	CreateShader("default-frag", "fragment.glsl", GL_FRAGMENT_SHADER);
	CreateShader("default-vert", "vertex.glsl", GL_VERTEX_SHADER);
	AttachShader("defaultProgram", "default-vert", "default-frag");
	LinkProgram("defaultProgram");
	CompileShader("defaultProgram");

	CreateProgram("noiseProgram", true);
	CreateShader("noiseProgram-vert", "NoiseVertex.glsl", GL_VERTEX_SHADER);
	CreateShader("noiseProgram-frag", "NoiseFragment.glsl", GL_FRAGMENT_SHADER);
	AttachShader("noiseProgram", "noiseProgram-vert", "noiseProgram-frag");
	LinkProgram("noiseProgram");
	CompileShader("noiseProgram");

	CreateProgram("blueProgram");
	CreateShader("blueProgram-vert", "vertex3.glsl", GL_VERTEX_SHADER);
	AttachShader("blueProgram", "blueProgram-vert", "default-frag");
	LinkProgram("blueProgram");
	CompileShader("blueProgram");

	glUseProgram(programs["blueProgram"].id);
	GLint loc = glGetUniformLocation(programs["blueProgram"].id, "newColor");
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

	CreateProgram("SnapProgram");
	CreateShader("Snap-vert", "SnapVertex.glsl", GL_VERTEX_SHADER);
	CreateShader("Snap-frag", "SnapFragment.glsl", GL_FRAGMENT_SHADER);
	AttachShader("SnapProgram", "Snap-vert", "Snap-frag");
	LinkProgram("SnapProgram");
	CompileShader("SnapProgram");

	currentProgram = "defaultProgram";
	inited = true;
	
}

void Game::Cleanup(){
	if(!inited){
		return;
	}

	for (std::vector<GameObject*>::iterator it = objects.begin() ; it != objects.end(); ++it){
		delete (*it);
	} 
	objects.clear();

	for (std::vector<Camera*>::iterator it = cameras.begin() ; it != cameras.end(); ++it){
		delete (*it);
	} 
	cameras.clear();

	for (std::vector<GameObject*>::iterator it = connectors.begin(); it != connectors.end(); ++it) {
		delete (*it);
	}
	connectors.clear();

	delete[] skeleRotations;

	DetachShaders("DiffuseProgram");
	DetachShaders("blueProgram");
	DetachShaders("noiseProgram");
	DetachShaders("defaultProgram");
	DetachShaders("PostProcessProgram");

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
	for (int i = 0; i < SKELE_PARTS; i++) {
		if (timeElapsed <= 5) {
			RotateObjectTime(FindIndex("SkeleRoot") + i, skeleRotations[i], timeElapsed / 100);
		}
	}

	if(snap){
		snapTime += deltaTime;
		glUseProgram(programs["SnapProgram"].id);
		GLint loc = glGetUniformLocation(programs["SnapProgram"].id, "uTime");
		glUniform1f(loc, snapTime);
	}

	timeElapsed += deltaTime;
}

void Game::Display(){

	oldTime = newTime;
	newTime = time(NULL);
	deltaTime = newTime - oldTime;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for(int i = 0; i < PASS_NUM; i++){	
		if(i == 0){
			glBindFramebuffer(GL_FRAMEBUFFER, fbos["offscreen"].fbo);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glMatrixMode(GL_MODELVIEW);

			glUseProgram(programs[currentProgram].id);

			if(snap){
				for(unsigned int c = 0; c < objects.size(); c+=2){
					ResetPosition();
					objects[c]->Draw();
				}
				for(unsigned int c = 0; c < connectors.size(); c+=2){
					ResetPosition();
					connectors[c]->Draw();
				}
			}
			else if(programs[currentProgram].oneDraw){
				ResetPosition();
				objects[0]->Draw();
			}
			else {
				for(unsigned int c = 0; c < objects.size(); c++){
					ResetPosition();
					objects[c]->Draw();
				}
				for(unsigned int c = 0; c < connectors.size(); c++){
					ResetPosition();
					connectors[c]->Draw();
				}
			}
		}
		if(i == 1){
			if(snap){
				DrawFBO("offscreen");
				glBindFramebuffer(GL_FRAMEBUFFER, fbos["offscreen"].fbo);
				glUseProgram(programs["defaultProgram"].id);
				for(unsigned int c = 1; c < objects.size(); c+=2){
					ResetPosition();
					objects[c]->Draw();
					ResetPosition();
				}
				for (unsigned int c = 1; c < connectors.size(); c+=2) {
					ResetPosition();
					connectors[c]->Draw();
				}
			}
			if(programs[currentProgram].oneDraw){
				DrawFBO("offscreen");
				glBindFramebuffer(GL_FRAMEBUFFER, fbos["offscreen"].fbo);
				glUseProgram(programs["defaultProgram"].id);
				for(unsigned int c = 1; c < objects.size(); c++){
					ResetPosition();
					objects[c]->Draw();
					ResetPosition();
				}
				for (unsigned int c = 0; c < connectors.size(); c++) {
					ResetPosition();
					connectors[c]->Draw();
				}
			}
		}
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
		if(i == PASS_NUM - 1){
			DrawFBO("offscreen");
			glBindFramebuffer(GL_FRAMEBUFFER, fbos["offscreen"].fbo);
			glUseProgram(programs["defaultProgram"].id);

			if(!snap){
				DrawText(Vector3(10.0f, (float)(screenHeight - 20), 0.0f), red, "Current selected object: " + objects[objectIndex]->name);
				DrawText(Vector3(10.0f, (float)(screenHeight - 40), 0.0f), red, "Current Shader: " + currentProgram);

				if(!showInfo){
					DrawText(Vector3(10.0f, (float)(screenHeight - 60), 0.0f), red, "Press i to show controls");
				}
				else {
					DrawText(Vector3(10.0f, (float)(screenHeight - 60), 0.0f), red, "Press i to hide controls");
					DrawText(Vector3(10.0f, (float)(screenHeight - 80), 0.0f), red, "1, 2, 3 to control X, Y, Z.");
					DrawText(Vector3(10.0f, (float)(screenHeight - 100), 0.0f), red, "Current X, Y, Z Position: " + std::to_string(objects[objectIndex]->GetPos().x) + ", " +
						std::to_string(objects[objectIndex]->GetPos().y) + ", " + std::to_string(objects[objectIndex]->GetPos().z));
					DrawText(Vector3(10.0f, (float)(screenHeight - 120), 0.0f), red, "!, @, # to control -X, -Y, -Z");
					DrawText(Vector3(10.0f, (float)(screenHeight - 140), 0.0f), red, "4, 5, 6 to control rotational X, Y, Z");
					DrawText(Vector3(10.0f, (float)(screenHeight - 160), 0.0f), red, "Current X, Y, Z Rotation: " + std::to_string(objects[objectIndex]->GetRot().x) + ", " +
						std::to_string(objects[objectIndex]->GetRot().y) + ", " + std::to_string(objects[objectIndex]->GetRot().z));
					DrawText(Vector3(10.0f, (float)(screenHeight - 180), 0.0f), red, "$, %, ^ to control rotational -X, -Y, -Z");
					DrawText(Vector3(10.0f, (float)(screenHeight - 200), 0.0f), red, "Up/Down to select objects");
					DrawText(Vector3(10.0f, (float)(screenHeight - 220), 0.0f), red, "Right/Left to select cameras");
					DrawText(Vector3(10.0f, (float)(screenHeight - 240), 0.0f), red, "Enter to change shader");
					DrawText(Vector3(10.0f, (float)(screenHeight - 260), 0.0f), red, "WASD to control camera movement");
					DrawText(Vector3(10.0f, (float)(screenHeight - 280), 0.0f), red, "c to draw colliders");
				}
			}
			else {
				DrawText(Vector3(400, (float)(screenHeight - 20), 0.0f), red, "I hope they'll remember you");
			}
		}
	}

	DrawFBO("offscreen");

	glutSwapBuffers();
}

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

void Game::MoveCurrentObject(Vector3 pos, bool relative){
	objects[objectIndex]->Translate(pos, relative);
	GameObject *collider = CheckAllCollisions();
	if(collider){
		if(!collider->isStatic){
			collider->Translate(Vector3(pos), relative);
		}
		else {
			objects[objectIndex]->Translate(Vector3(pos * -1.0f), relative);
		}
	}
}

void Game::RotateCurrentObject(Vector3 rot, bool relative){
	objects[objectIndex]->Rotate(rot, relative);
}

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

void Game::DrawText(Vector3 pos, Vector3 color, std::string text, void* font){
	ResetPosition();
	glColor3f(color.x, color.y, color.z);
	glWindowPos3f(pos.x, pos.y, pos.z);
	const unsigned char* newText = reinterpret_cast<const unsigned char *>(text.c_str());
	glutBitmapString(font, newText);
}


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

void Game::CreateBasicFBO(std::string name){
	glGenFramebuffers(1, &fbos[name].fbo);
	glGenTextures(1, &textures[name]);
	glGenRenderbuffers(1, &fbos[name].fbo);

	glBindFramebuffer(GL_FRAMEBUFFER, fbos[name].fbo);

	glBindTexture(GL_TEXTURE_2D, textures[name]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[name], 0);

	glBindRenderbuffer(GL_RENDERBUFFER, fbos[name].fbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, screenWidth, screenHeight);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbos[name].fbo);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE){
		std::cout << "Created framebuffer with name: " << name << std::endl;
	}
	else {
		std::cout << "Error creating framebuffer with name: " << name << std::endl;
	}
}

void Game::CreateFBOFromTexture(std::string textureName, std::string fboName){
	glGenFramebuffers(1, &fbos[fboName].fbo);
	glGenRenderbuffers(1, &fbos[fboName].fbo);

	glBindFramebuffer(GL_FRAMEBUFFER, fbos[fboName].fbo);

	glBindTexture(GL_TEXTURE_2D, textures[textureName]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[textureName], 0);

	glBindRenderbuffer(GL_RENDERBUFFER, fbos[textureName].fbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, screenWidth, screenHeight);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbos[textureName].fbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Created framebuffer with name: " << textureName << std::endl;
	}
	else {
		std::cout << "Error creating framebuffer with name: " << textureName << std::endl;
	}
}

void Game::CreateProgram(std::string name, bool oneDraw){
	programs[name].id = glCreateProgram();
	programs[name].oneDraw = oneDraw;
	if(programs[name].id == 0){
		std::cout << "ERROR CREATING SHADER PROGRAM: " + name << std::endl;
	}
	else {
		std::cout << "Created program: " + name << std::endl;
	}
}

void Game::CreateShader(std::string name, const char* path, GLenum type){
	shaders[name] = glCreateShader(type);

	std::string contents = ReadFile(path);

	char const* newContents = contents.c_str();
	glShaderSource(shaders[name], 1, &newContents, NULL);

	std::cout << "Created shader: " + name << std::endl;
}

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

void Game::AttachShader(std::string programName, std::string vertexName, std::string fragmentName){
	glAttachShader(programs[programName].id, shaders[vertexName]);
	glAttachShader(programs[programName].id, shaders[fragmentName]);

	programs[programName].fragName = fragmentName;
	programs[programName].vertName = vertexName;
}

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

void Game::DetachShaders(std::string programName){
	int programId = programs[programName].id;

	glDetachShader(programId, shaders[programs[programName].fragName]);
	glDetachShader(programId, shaders[programs[programName].vertName]);
}

void Game::KeyboardInputWrapper(unsigned char key, int x, int y){
	instance->KeyboardInput(key, x, y);
}

void Game::SpecialKeyboardWrapper(int key, int x, int y){
	instance->SpecialKeyboard(key, x, y);
}

std::string Game::ReadFile(const char *path){
	std::ifstream fs(path, std::ios::in);

	if(!fs.is_open()){
		std::cout << "ERROR! COULD NOT READ FILE!" << path << std::endl;
		return nullptr;
	}

	std::stringstream buffer;
	std::string line;
	while(std::getline(fs, line)){
		buffer << line << "\n";
	}

	fs.close();
	return buffer.str();
}

void Game::SpecialKeyboard(int key, int x, int y){
	switch(key){
		//Up/down to change selected object
	case GLUT_KEY_UP:
		objectIndex++;
		if((unsigned int)objectIndex >= objects.size()){
			objectIndex = 0;
		}
		if(objects[objectIndex]->GetType() == GameObject::Type::CONNECTOR) objectIndex++;
		if((unsigned int)objectIndex >= objects.size()){
			objectIndex = 0;
		}
		break;
	case GLUT_KEY_DOWN:
		objectIndex--;
		if(objectIndex < 0){
			objectIndex = (int)objects.size() - 1;
		}
		if(objects[objectIndex]->GetType() == GameObject::Type::CONNECTOR) objectIndex--;
		if(objectIndex < 0){
			objectIndex = (int)objects.size() - 1;
		}
		break;
	case GLUT_KEY_LEFT:
		cameraIndex++;
		if((unsigned int)cameraIndex >= cameras.size()){
			cameraIndex = 0;
		}
		break;
	case GLUT_KEY_RIGHT:
		cameraIndex--;
		if(cameraIndex < 0){
			cameraIndex = (int)cameras.size() - 1;
		}
		break;
	default:
		break;
	}
}

void Game::MoveObjectTime(std::string partName, Vector3 newMove, float time){
	int index = FindIndex(partName);
	objects[index]->Translate(Lerp(objects[index]->GetPos(true), newMove, time));
}

void Game::MoveObjectTime(int index, Vector3 newMove, float time) {
	objects[index]->Translate(Lerp(objects[index]->GetPos(true), newMove, time));
}

void Game::RotateObjectTime(std::string partName, Vector3 newRot, float time){
	int index = FindIndex(partName);
	objects[index]->Rotate(Lerp(objects[index]->GetRot(true), newRot, time));
}

void Game::RotateObjectTime(int index, Vector3 newRot, float time){
	objects[index]->Rotate(Lerp(objects[index]->GetRot(true), newRot, time));
}

void Game::KeyboardInput(unsigned char key, int x, int y){
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
		snap = false;
		it = programs.find(currentProgram);
		it++;
		if(it == programs.end()){
			it = programs.begin();
		}
		currentProgram = it->first;
		if(currentProgram == "SnapProgram"){
			it = programs.find(currentProgram);
			it++;
			if(it == programs.end()){
				it = programs.begin();
			}
		}
		break;

	//c to draw colliders
	case 'c':
		drawColliders = !drawColliders;
		break;
		
	case '/':
		snap = true;
		currentProgram = "SnapProgram";

	default:
		break;
	}

	delete right;
	delete up;
}

void Game::ResetPosition(){
	glLoadIdentity();
	cameras[cameraIndex]->ResetCamera();
}

GameObject* Game::FindObject(std::string name){
	for(unsigned int i = 0; i < objects.size(); i++){
		if(objects[i]->name == name) return objects[i];
	}
	return nullptr;
}

int Game::FindIndex(std::string name){
	for (unsigned int i = 0; i < objects.size(); i++) {
		if (objects[i]->name == name) return i;
	}
	return -1;
}

void Game::MoveCameraWrapper(int x, int y){
	instance->MoveCamera(x, y);
}

void Game::MoveCamera(int x, int y){
	cameras[cameraIndex]->Move(x, y, screenWidth, screenHeight);
}

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

void Game::Reshape(GLsizei width, GLsizei height){

	if (height == 0) height = 1;
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

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

void Game::NewSkeletonPart(Vector3 newPos, Vector3 newRot, std::string name, GameObject* parent){
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