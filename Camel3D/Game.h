#pragma once
#include <vector>
#include <map>
#include "GameObject.h"
#include "Vector3.h"
#include "GL/glut.h"
#include "Clip.h"

struct ObjLoader;
class Vector3;
class Camera;
class Skeleton;

struct Program {
	GLuint id;
	std::string fragName;
	std::string vertName;
	std::string geoname;
};

struct FBO {
	GLuint fbo;
	GLuint depth;
	GLuint texture;
};

struct Transform {
	Vector3 pos;
	Vector3 rot;
	Vector3 scale;
	std::string objName;
};

struct Texture {
	void* data;
	int width;
	int height;
	GLuint id;
};

class Game {

	public:
		void Init(int width, int height);
		void ResetPosition();
		void Cleanup();
		void GameLoop();
		static void InitInstance();
		static void CleanupInstance();
		static void KeyboardInputWrapper(unsigned char key, int x, int y);
		static void MouseInputWrapper(int button, int state, int x, int y);
		static void SpecialKeyboardWrapper(int key, int x, int y);
		static void ReshapeWrapper(GLsizei width, GLsizei height);
		static void MoveCameraWrapper(int x, int y);
		static void DisplayWrapper();
		static void UpdateWrapper();
		static void MenuWrapper(int id);
		static void ColorMenuWrapper(int id);
		static Game* GetInstance();
		void CrossProduct(Vector3 one, Vector3 two, Vector3* product);
		void NewObject(GameObject::Type newType, Vector3 newColor, Vector3 newPos, Vector3 newScale, Vector3 newRot, std::string name, bool isStatic, bool hasCollider = true);
		void NewObject(GameObject::Type newType, char* filename, Vector3 newColor, Vector3 newPos, Vector3 newScale, Vector3 newRot, std::string name, bool isStatic, bool hasCollider = true);
		void NewObject(GameObject::Type newType, Vector3 newColor, Vector3 newPos, Vector3 newScale, Vector3 newRot, std::string name, GameObject* parent, bool isStatic, bool hasCollider = true);
		void NewObject(Vector3 newPos, Vector3 newScale, Vector3 newRot, std::string newName);
		void NewSkeletonPart(Vector3 newPos, Vector3 newRot, Vector3 newScale, std::string name, std::string skeleName, GameObject* parent = nullptr);
		void NewSkeletonPart(Vector3 newPos, Vector3 newRot, Vector3 newScale, std::string name, std::string skeleName, GameObject* parent, bool canTranslate, bool canRotate);
		void NewObject(GameObject* one, GameObject* two);
		void NewCamera(Vector3 pos, Vector3 direction);
		void CreateProgram(std::string name);	
		void CreateShader(std::string name, const char* filename, GLenum type);
		void DrawText(Vector3 pos, Vector3 color, std::string text, void* font = GLUT_BITMAP_HELVETICA_18);
		void AttachShader(std::string programName, std::string vertexName, std::string fragmentName);
		std::string ReadFile(std::string path);
		void MoveCurrentObject(Vector3 pos, bool relative = false);
		void RotateCurrentObject(Vector3 rot, bool relative = false);
		void CompileShader(std::string name);
		void LinkProgram(std::string name);
		void DetachShaders(std::string name);
		void CreateBasicFBO(std::string name);
		void CreateTexture(std::string fileName, std::string texName);
		void DrawFBO(std::string name);
		GameObject* FindObject(std::string name);
		int FindIndex(std::string name);
		int FindIndexSkeleton(std::string partName, std::string skeleName);
		void MoveObjectTime(int index, Vector3 newMove, float time, bool relative = true);
		void MoveObjectTime(int index, Vector3 newMove, float time, std::string skeleName, bool relative = true);
		void RotateObjectTime(int index, Vector3 newRot, float time);
		void RotateObjectTime(int index, Vector3 newRot, std::string skeleName, float time);
		void ScaleObjectTime(int index, Vector3 newScale, float time);
		void ScaleObjectTime(int index, Vector3 newScale, std::string skeleName, float time);
		void Animate(Clip<Transform>* clip, std::string skeleName);
		void Animate(Clip<Transform>* clip);
		template<class T>
		void StopAnimation(Clip<T>* clip);
		template<class T>
		void StartAnimation(Clip<T>* clip);
		template<class T>
		void SwitchForward(Clip<T>* clip);
		template<class T>
		void SwitchReverse(Clip<T>* clip);

	private:
		Game();
		~Game();
		void KeyboardInput(unsigned char key, int x, int y);
		void SpecialKeyboard(int key, int x, int y);
		void MouseInput(int button, int state, int x, int y);
		void Reshape(GLsizei width, GLsizei height);
		void MoveCamera(int x, int y);
		void ClearGL();
		void Display();
		void Update();
		void Menu(int id);
		void ColorMenu(int id);
		GameObject* CheckAllCollisions();
		GameObject* CheckAllCollisions(int objIndex);
		int screenWidth;
		int screenHeight;

		std::vector<GameObject*> objects;
		std::vector<GameObject*> connectors;
		std::vector<Camera*> cameras;

		std::map<std::string, Skeleton> skeletons;

		int cameraIndex = 0;
		int objectIndex = 0;

		Vector3 bgColor;

		Clip<Transform>* starting;

		std::map<std::string, Program> programs;
		std::map<std::string, GLuint> shaders;
		std::map<std::string, FBO> fbos;
		std::map<std::string, Texture> textures;

		std::string currentProgram;

		static Game* instance;
		static int msID;
		int mID;

		int spawnNumber = 0;
		int textureIndex = 0;

		bool inited;
		bool stopped;
		bool drawColliders;
		bool showInfo;

		float timeElapsed = 0;

		time_t newTime;
		time_t deltaTime;
		time_t oldTime;
};