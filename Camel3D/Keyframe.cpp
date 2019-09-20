#include "Keyframe.h"
#include "Vector3.h"
#include "Game.h"

template<class T>
Keyframe<T>::Keyframe(T* toPass, float time, int sizeOfArray, bool array){
	data = toPass;
	isArray = array;
	loadupTime = time;
	arraySize = sizeOfArray;
}

template<class T>
Keyframe<T>::Keyframe(int arrayPoints, float time){
	data = new T[arrayPoints];
	isArray = true;
	loadupTime = time;
	arraySize = arrayPoints;
}

template<class T>
Keyframe<T>::Keyframe(float time){
	data = new T;
	isArray = false;
	loadupTime = time;
	arraySize = 0;
}

template<class T>
void Keyframe<T>::DeleteData(){
	delete data;
}

template<class T>
void Keyframe<T>::SetDatapoint(T point, int index){
	data[index] = point;
}
template<class T>
void Keyframe<T>::SetDatapoint(T* point){
	data = point;
}

template class Keyframe<Vector3>;
template class Keyframe<Transform>;