#pragma once
#include "Keyframe.h"
#include <list>

#ifndef CLIPH
#define CLIPH

enum ClipDirection { Reverse = -1, Stop, Forward };
enum BlendType { BlendLerp, Add, Scale, Average };

template<class T>
class Clip {
	public:
		Clip(Keyframe<T> firstFrame, BlendType type = BlendType::BlendLerp);
		Clip(std::list<Keyframe<T>> keyList, BlendType type = BlendType::BlendLerp);
		void AddKeyframe(Keyframe<T> nextFrame);
		~Clip();

		float elapsedTime;
		ClipDirection dir;
		BlendType blendType;
		float index;
		std::list<Keyframe<T>> keys;

		bool posRelative = true;
	private:
		
};

#endif