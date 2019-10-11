#include "Skeleton.h"
#include "GameObject.h"
#include "Game.h"
#include <fstream>
#include <string>
#include <sstream>
#include "Game.h"

void Skeleton::WriteToFile(std::string fileName){
	//FILEFORMAT
	//partName partParent partPos partRot partScale canTranslate canRotate

	std::ofstream file;
	file.open(fileName);
	if(file.is_open()){
		for(int i = 0; i < parts.size(); i++){
			file << parts[i]->name << " ";
			if(parts[i]->GetParent()){
				file << parts[i]->GetParent()->name << " ";
			}
			else {
				file << "null ";
			}
			file << basePose[i].pos.x << "," << basePose[i].pos.y << "," << basePose[i].pos.z << " ";
			file << basePose[i].rot.x << "," << basePose[i].rot.y << "," << basePose[i].rot.z << " ";
			file << basePose[i].scale.x << "," << basePose[i].scale.y << "," << basePose[i].scale.z << " ";
			file << parts[i]->GetCanTranslate() << " ";
			file << parts[i]->GetCanRotate() << std::endl;
		}
		file.close();
	}
	else {
		printf("Failed to load file\n");
	}
}

bool Skeleton::ReadFromFile(std::string fileName){

	std::ifstream file;
	file.open(fileName);

	if(file.is_open()){
		std::string line;
		while(std::getline(file, line)){
			std::string partName = "";
			std::string parentName = "";
			Vector3 pos, rot, scale;
			float numbers[3];
			bool canRotate, canTranslate;
			//This doesn't work because it resets every time

			for(auto letter : line){
				//First read the name
				if(letter = ' '){
					break;
				}
				else {
					partName += letter;
				}
			}

			for(auto letter : line){
				//Next read the parent
				if(letter = ' '){
					break;
				}
				else {
					partName += letter;
				}
			}

			int index = 0;
			for(auto letter : line){
				if(letter = ' '){
					break;
				}
				numbers[index] = std::stoi(std::string(letter, 1));
				index++;
			}

			index = 0;
			pos = Vector3(numbers[0], numbers[1], numbers[2]);

			for(auto letter : line){
				if(letter = ' '){
					break;
				}
				numbers[index] = std::stoi(std::string(letter, 1));
				index++;
			}

			index = 0;
			rot = Vector3(numbers[0], numbers[1], numbers[2]);

			for(auto letter : line){
				if(letter = ' '){
					break;
				}
				numbers[index] = std::stoi(std::string(letter, 1));
				index++;
			}

			index = 0;
			scale = Vector3(numbers[0] * .05f, numbers[1] * .05f, numbers[2] * .05f);

			for(auto letter : line){
				if(letter = ' '){
					break;
				}
				std::istringstream(std::string(letter, 1)) >> canTranslate;
			}

			for(auto letter : line){
				if(letter = ' '){
					break;
				}
				std::istringstream(std::string(letter, 1)) >> canRotate;
			}

			if(parentName == "null"){
				parts.push_back(new GameObject(GameObject::SPHERE, Vector3(1.0, 0.0, 1.0), pos, scale, rot, partName, canTranslate, canRotate));
			}
			else {
				GameObject* parent = nullptr;
				for(int i = 0; i < parts.size(); i++){
					if(parts[i]->name == parentName){
						parent = parts[i];
					}
				}
				if(parent == nullptr){
					return false;
				}
				parts.push_back(new GameObject(GameObject::SPHERE, Vector3(1.0, 0.0, 1.0), pos, scale, rot, partName, parent, canTranslate, canRotate));
			}
		}

		file.close();
	}
}