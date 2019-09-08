#include "Clip.h"
#include "Keyframe.h"

template<class T>
Clip<T>::Clip(Keyframe<T> firstFrame){
	keys.push_back(firstFrame);
	dir = Clip::Forward;
	elapsedTime = 0;
	index = 0;
}

template<class T>
Clip<T>::Clip(std::list<Keyframe<T>> keyList){
	keys = keyList;
	dir = Clip::Forward;
	elapsedTime = 0;
	index = 0;
}

template<class T>
void Clip<T>::AddKeyframe(Keyframe<T> nextFrame){
	keys.push_back(nextFrame);
}

template<class T>
Clip<T>::~Clip(){}

template class Clip<Vector3>;