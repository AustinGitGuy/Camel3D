#include "GameObject.h"
#include "Vector3.h"
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/glut.h"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "OBJStructs.h"
#include "BoxCollider.h"
#include "DrawShapes.h"

float pyramidData[]{
	0.0f, 1.0f, 0.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,

	0.0f, 1.0f, 0.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, -1.0f,

	0.0f, 1.0f, 0.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0,

	0.0f, 1.0f, 0.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f, 1.0f,

	1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
};

GameObject::GameObject(Type newType, Vector3 newColor, Vector3 newPos, Vector3 newScale, Vector3 newRot, std::string newName, bool newStatic, bool hasCollider){
	pos = new Vector3(newPos.x, newPos.y, newPos.z);
	rot = new Vector3(newRot.x, newRot.y, newRot.z);
	scale = new Vector3(newScale.x, newScale.y, newScale.z);

	type = newType;
	isStatic = newStatic;
	name = newName;

	if(newRot != VECTOR3_ZERO){
		glRotatef(rot->x, 1, 0, 0);
		glRotatef(rot->y, 0, 1, 0);
		glRotatef(rot->z, 0, 0, 1);
	}

	if(type == Type::DODECAHEDRON || type == Type::PYRAMID){
		size = new Vector3(scale->x * 2, scale->y * 2, scale->z * 2);
	}
	else if(type == Type::SPHERE){
		size = new Vector3(scale->x * 1.5f, scale->y * 1.5f, scale->z * 1.5f);
	}
	else {
		size = new Vector3(scale->x, scale->y, scale->z);
	}
	
	if(hasCollider){
		col = new BoxCollider(pos, size);
	}

	if(type == Type::PYRAMID){
		vertexSize = 45;
		vertexData = new float[45];
		color = new Vector3[24];
		for(int i = 0; i < 24; i++){
			color[i] = newColor;
		}
		vertexData = pyramidData;
		mode = GL_TRIANGLES;
	}
	else {
		color = new Vector3[1];
		color[0] = newColor;
	}
	parent = nullptr;
}

GameObject::GameObject(Vector3 newPos, Vector3 newScale, Vector3 newRot, std::string newName){
	pos = new Vector3(newPos.x, newPos.y, newPos.z);
	rot = new Vector3(newRot.x, newRot.y, newRot.z);
	scale = new Vector3(newScale.x, newScale.y, newScale.z);

	type = Type::EMPTY;
	name = newName;
	isStatic = false;
}

GameObject::GameObject(Type newType, Vector3 newColor, Vector3 newPos, Vector3 newScale, Vector3 newRot, std::string newName, GameObject* newParent, bool newStatic, bool hasCollider){
	pos = new Vector3(newPos.x, newPos.y, newPos.z);
	rot = new Vector3(newRot.x, newRot.y, newRot.z);
	scale = new Vector3(newScale.x, newScale.y, newScale.z);

	type = newType;
	isStatic = newStatic;
	name = newName;

	if(type == Type::DODECAHEDRON || type == Type::PYRAMID){
		size = new Vector3(scale->x * 2, scale->y * 2, scale->z * 2);
	}
	else if(type == Type::SPHERE){
		size = new Vector3(scale->x * 1.5f, scale->y * 1.5f, scale->z * 1.5f);
	}
	else {
		size = new Vector3(scale->x, scale->y, scale->z);
	}

	if(hasCollider){
		col = new BoxCollider(pos, size);
	}

	if(type == Type::PYRAMID){
		vertexSize = 45;
		vertexData = new float[45];
		color = new Vector3[24];
		for(int i = 0; i < 24; i++){
			color[i] = newColor;
		}
		vertexData = pyramidData;
		mode = GL_TRIANGLES;
	}
	else {
		color = new Vector3[1];
		color[0] = newColor;
	}

	parent = newParent;
	//NOTE: This does not set the child for the parent. Do so in wrapper for now
}

bool GameObject::AttachBoxCollider(){
	if(col == nullptr){
		col = new BoxCollider(pos, size);
		return true;
	}
	else return false;
}

GameObject::GameObject(Type newType, char* filename, Vector3 newColor, Vector3 newPos, Vector3 newScale, Vector3 newRot, std::string newName, bool newStatic, bool hasCollider){
	pos = new Vector3(newPos.x, newPos.y, newPos.z);
	obj = new ObjLoader();
	obj = OBJLoad(filename, obj);
	color = new Vector3(newColor.x, newColor.y, newColor.z);
	scale = new Vector3(newScale.x, newScale.y, newScale.z);
	rot = new Vector3(newRot.x, newRot.y, newRot.z);

	type = newType;
	isStatic = newStatic;
	name = newName;
}

GameObject::GameObject(GameObject* one, GameObject* two){
	type = Type::CONNECTOR;
	isStatic = true;
	name = "Connector";

	connectionOne = one;
	connectionTwo = two;
}

GameObject::~GameObject(){
	delete pos;
	delete scale;
	delete size;
	delete col;
	delete rot;
	delete[] color;
	if(type == Type::PYRAMID){
		delete vertexData;
	}
}

void GameObject::TranslateFromParent(){
	if(parent != nullptr){
		parent->TranslateFromParent();
	}
	glTranslatef(pos->x, pos->y, pos->z);
	glRotatef(rot->x, 1, 0, 0);
	glRotatef(rot->y, 0, 1, 0);
	glRotatef(rot->z, 0, 0, 1);
}

void GameObject::Draw(){
	if(type == Type::CONNECTOR){
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);
		glColor3f(0.537f, 0.164f, 0.529f);
		connectionOne->TranslateFromParent();
		DrawCube(VECTOR3_ZERO, connectionTwo->GetPos(true));
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);
		return;
	}

	if(parent != nullptr){
		parent->TranslateFromParent();
	}

	if(type == Type::EMPTY) return;

	glTranslatef(pos->x, pos->y, pos->z);

	glRotatef(rot->x, 1, 0, 0);
	glRotatef(rot->y, 0, 1, 0);
	glRotatef(rot->z, 0, 0, 1);

	glScalef(scale->x, scale->y, scale->z);
	
	if(type == Type::TEAPOT){
		glColor3f(color[0].x, color[0].y, color[0].z);
		glutSolidTeapot(1);
	}
	if(type == Type::OBJ){
		glBegin(GL_TRIANGLES);
		for(int o = 0; o < obj->objCount; o++){
			for(int f = 0; f < obj->obj[o].TotalFaceCount; f++){
				for(int i = 0; i < 3; i++){
					glColor3f(color->x, color->y, color->z);
					glVertex3f((GLfloat)-obj->vert[obj->obj[o].fmap[f].vI[i]].x, (GLfloat)obj->vert[obj->obj[o].fmap[f].vI[i]].y, (GLfloat)obj->vert[obj->obj[o].fmap[f].vI[i]].z);
				}
			}
		}
		glEnd();
	}
	if(type == Type::CUBE){
		glColor3f(color[0].x, color[0].y, color[0].z);
		glutSolidCube(1);
	}
	if(type == Type::DODECAHEDRON){
		glColor3f(color[0].x, color[0].y, color[0].z);
		glutSolidDodecahedron();
	}
	if(type == Type::SPHERE){
		glColor3f(color[0].x, color[0].y, color[0].z);
		glutSolidSphere(1, 50, 50);
	}
	else {
		glBegin(mode);
		for(int i = 0; i < vertexSize / 3; i++){
			glColor3f(color[i].x, color[i].y, color[i].z);
			glVertex3f(vertexData[i * 3], vertexData[i * 3 + 1], vertexData[i * 3 + 2]);
		}
		glEnd();
	}
}

Vector3 GameObject::GetPos(bool relative){
	if(relative) return *pos;
	if(parent != nullptr){
		return parent->GetPos().operator+(*pos);
	}
	else return *pos;
}

Vector3 GameObject::GetRot(bool relative){
	if(relative) return *rot;
	if(parent != nullptr){
		return parent->GetRot().operator+(*rot);
	}
	else return *rot;
}

Vector3 GameObject::GetScale(bool relative){
	if(relative) return *scale;
	if(parent != nullptr){
		return parent->GetScale().operator+(*scale);
	}
	else return *scale;
}

void GameObject::Translate(Vector3 newPos, bool relative, bool tOverride){
	if(!canTranslate && !tOverride) return;
	if(relative){
		Vector3* posTwo = new Vector3(newPos.x + pos->x, newPos.y + pos->y, newPos.z + pos->z);
		delete pos;
		pos = posTwo;
	}
	else {
		delete pos;
		pos = new Vector3(newPos.x, newPos.y, newPos.z);
	}

	if(col) col->RegenerateCollider(pos, size);
}

void GameObject::Translate(float x, float y, float z, bool relative, bool tOverride){
	if(!canTranslate && !tOverride) return;
	if(relative){
		Vector3* posTwo = new Vector3(x + pos->x, y + pos->y, z + pos->z);
		delete pos;
		pos = posTwo;
	}
	else {
		delete pos;
		pos = new Vector3(x, y, z);
	}

	if(col) col->RegenerateCollider(pos, size);
}

void GameObject::TranslateChildren(float x, float y, float z, bool relative){
	for(int i = 0; i < children.size(); i++){
		children[i]->Translate(x, y, z, relative, true);
	}
}

void GameObject::TranslateChildren(Vector3 pos, bool relative){
	for(int i = 0; i < children.size(); i++) {
		children[i]->Translate(pos, relative, true);
	}
}

void GameObject::Rotate(float x, float y, float z, bool relative){
	if(!canRotate) return;
	if(relative){
		Vector3* rotTwo = new Vector3(x + pos->x, y + pos->y, z + pos->z);
		delete rot;
		rot = rotTwo;
	}
	else {
		delete rot;
		rot = new Vector3(x, y, z);
	}
}

void GameObject::Rotate(Vector3 newRot, bool relative){
	if(!canRotate) return;
	if(relative){
		Vector3* rotTwo = new Vector3(newRot.x + rot->x, newRot.y + rot->y, newRot.z + rot->z);
		delete rot;
		rot = rotTwo;
	}
	else {
		delete rot;
		rot = new Vector3(newRot.x, newRot.y, newRot.z);
	}
}


void GameObject::Scale(Vector3 newScale, bool relative){
	if(relative){
		Vector3* scaleTwo = new Vector3(newScale.x + scale->x, newScale.y + scale->y, newScale.z + scale->z);
		delete scale;
		scale = scaleTwo;
	}
	else {
		delete scale;
		scale = new Vector3(newScale.x, newScale.y, newScale.z);
	}
}
void GameObject::Scale(float x, float y, float z, bool relative){
	if (relative) {
		Vector3* scaleTwo = new Vector3(x + scale->x, y + scale->y, z + scale->z);
		delete scale;
		scale = scaleTwo;
	}
	else {
		delete scale;
		scale = new Vector3(x, y, z);
	}
}

void GameObject::SetChild(GameObject* obj){
	children.push_back(obj);
}

void GameObject::SetColor(Vector3 newColor){
	delete[] color;

	color = new Vector3(newColor.x, newColor.y, newColor.z);
}

void GameObject::SetCanRotate(bool newRotate){
	canRotate = newRotate;
}

void GameObject::SetCanTranslate(bool newTranslate){
	canTranslate = newTranslate;
}

bool GameObject::GetCanRotate(){
	return canRotate;
}

bool GameObject::GetCanTranslate(){
	return canTranslate;
}

ObjLoader* GameObject::OBJLoad(char* filename, ObjLoader* obj){
	std::ifstream fin(filename);

	if(!fin){
		obj->objCount = -1;
		return obj;
	}

	std::string line;
	while(getline(fin, line)){
		std::stringstream ss(line);
		std::string lhead;
		std::getline(ss, lhead, ' ');
		if(lhead.compare("o") == 0){
			std::string name;
			std::getline(ss, name, '\n');
			Obj object;
			object.name = name;
			object.TotalFaceCount = 0;

			obj->obj.push_back(object);
			obj->objCount += 1;
			std::cout << "Loading " << name << std::endl;
		}
		else if(lhead.compare("v") == 0){
			Vertex tmpVert;

			tmpVert.used = true;

			ss >> tmpVert.z;
			ss >> tmpVert.y;

			ss >> tmpVert.x;

			obj->vert.push_back(tmpVert);
		}
		else if(lhead.compare("vt") == 0){
			TVertex tmpVert;
			ss >> tmpVert.x;
			ss >> tmpVert.y;
			obj->uv_vert.push_back(tmpVert);
		}
		else if(lhead.compare("vn") == 0){
			Vertex tmpVert;
			std::string tmp;
			tmpVert.used = true;
			ss >> tmpVert.x;
			ss >> tmpVert.y;
			ss >> tmpVert.z;
			obj->vn_vert.push_back(tmpVert);
		}
		else if(lhead.compare("f") == 0){
			Map mtmp;
			obj->obj[obj->objCount-1].TotalFaceCount += 1;
			for(int i = 0; i < 3; i++){
				std::string group;
				std::getline(ss, group, ' ');
				std::stringstream iss(group);

				std::string tmp;
				std::getline(iss, tmp, '/');
				mtmp.vI[i] = atoi(tmp.c_str()) -1;
				tmp.clear();
				std::getline(iss, tmp, '/');
				mtmp.uvI[i] = atoi(tmp.c_str()) -1;
				tmp.clear();
				std::getline(iss, tmp, ' ');
				mtmp.vnI[i] = atoi(tmp.c_str()) -1;
				tmp.clear();
			}
			obj->obj[obj->objCount-1].fmap.push_back(mtmp);
		}
		else if(lhead.compare("mtllib") == 0){
			getline(ss,obj->mtllib);
			std::ifstream mtlTest(obj->mtllib.c_str());
			if(!fin){
				std::cout << "ERROR: Unable able to open mtllib file " << obj->mtllib << std::endl;
			}
		}
		else if(lhead.compare("usemtl") == 0){
			getline(ss, obj->obj[obj->objCount -1].mtl);
		}
	}

	return obj;
}