#pragma once
#include "Keyframe.h"
#include <list>

#ifndef CLIPH
#define CLIPH

template<class T>
class Clip {
	public:
		Clip(Keyframe<T> firstFrame);
		Clip(std::list<Keyframe<T>> keyList);
		void AddKeyframe(Keyframe<T> nextFrame);
		~Clip();

		float elapsedTime;
		enum direction{Reverse=-1, Stop, Forward};
		direction dir;
		float index;
		std::list<Keyframe<T>> keys;
	private:
		
};

#endif