#pragma once
#include <string>
#include <vector>

struct ObjLoader;
class Vector3;
class Collider;

//CREDIT TO https://www.dreamincode.net/forums/topic/342023-loading-obj-in-opengl-texture-issue/ for the obj loader

class GameObject {
	public:
		enum Type {CUBE, PYRAMID, OBJ, TEAPOT, DODECAHEDRON, SPHERE, CONNECTOR, EMPTY};
		GameObject(Type newType, Vector3 newColor, Vector3 newPos, Vector3 newScale, Vector3 newRot, std::string newName, bool newStatic, bool hasCollider = true);
		GameObject(Type newType, Vector3 newColor, Vector3 newPos, Vector3 newScale, Vector3 newRot, std::string newName, GameObject* newParent, bool newStatic, bool hasCollider = true);
		GameObject(Type newType, char* filename, Vector3 newColor, Vector3 newPos, Vector3 newScale, Vector3 newRot, std::string newName, bool newStatic, bool hasCollider = true);
		GameObject(Vector3 newPos, Vector3 newScale, Vector3 newRot, std::string newName);
		GameObject(GameObject* one, GameObject* two);
		~GameObject();
		void Draw();
		void Translate(Vector3 pos, bool relative = false);
		void Translate(float x, float y, float z, bool relative = false);
		void Rotate(Vector3 rot, bool relative = false);
		void Rotate(float x, float y, float z, bool relative = false);
		void Scale(Vector3 scale, bool relative = false);
		void Scale(float x, float y, float z, bool relative = false);
		void SetChild(GameObject* obj);
		void TranslateFromParent();
		std::vector<GameObject*> GetChildren(){return children;}
		Vector3 GetPos(bool relative = false);
		Vector3 GetRot(bool relative = false);
		Vector3 GetScale(bool relative = false);
		void SetColor(Vector3 color);
		GameObject* GetParent(){return parent;};
		ObjLoader* OBJLoad(char* filename, ObjLoader* obj);
		Collider* GetCollider(){return col;};
		Type GetType(){return type;};
		bool AttachBoxCollider();

		std::string name;
		bool isStatic;

	private:
		Vector3* color;
		Vector3* pos;
		Vector3* scale;
		Vector3* size;
		Vector3* rot;

		ObjLoader* obj;
		GameObject* parent;
		Type type;

		Collider* col;

		std::vector<GameObject*> children;

		float* vertexData;
		unsigned int mode;
		int vertexSize;

		GameObject* connectionOne;
		GameObject* connectionTwo;
};