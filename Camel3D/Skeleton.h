#pragma once
#include <vector>

class GameObject;
struct Transform;

class Skeleton {
	public:
		void WriteToFile(std::string fileName);
		bool ReadFromFile(std::string fileName);
		std::vector<GameObject*> parts;
		std::vector<GameObject*> connectors;
		std::vector<Transform> basePose;
};