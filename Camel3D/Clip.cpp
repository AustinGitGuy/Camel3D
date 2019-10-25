#include "Clip.h"
#include "Game.h"
#include "Keyframe.h"

template<class T>
Clip<T>::Clip(Keyframe<T> firstFrame, BlendType type){
	keys.push_back(firstFrame);
	dir = ClipDirection::Forward;
	elapsedTime = 0;
	index = 0;
	blendType = type;
}

template<class T>
Clip<T>::Clip(std::list<Keyframe<T>> keyList, BlendType type) {
	keys = keyList;
	dir = ClipDirection::Forward;
	elapsedTime = 0;
	index = 0;
	blendType = type;
}

template<class T>
void Clip<T>::AddKeyframe(Keyframe<T> nextFrame){
	keys.push_back(nextFrame);
}

template<class T>
Clip<T>::~Clip(){}

template class Clip<Vector3>;
template class Clip<Transform>;