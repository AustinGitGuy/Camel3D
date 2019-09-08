#include "Keyframe.h"
#include "Vector3.h"
#include "Game.h"

template<class T>
Keyframe<T>::Keyframe(T* toPass, float time, bool array){
	data = toPass;
	isArray = array;
	loadupTime = time;
}

template<class T>
Keyframe<T>::Keyframe(int arrayPoints, float time){
	data = new T[arrayPoints];
	isArray = true;
	loadupTime = time;
}

template<class T>
Keyframe<T>::Keyframe(float time){
	data = new T;
	isArray = false;
	loadupTime = time;
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