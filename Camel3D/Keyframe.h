#pragma once
class Vector3;

#ifndef KEYFRAMEH
#define KEYFRAMEH

template<class T>
class Keyframe {
	public:
		//Ownership will be passed to the Keyframe class
		Keyframe(T* toPass, float time, int sizeOfArray, bool array = true);
		//If you want an array of points
		Keyframe(int arrayPoints, float time);
		//If you want a single datapoint
		Keyframe(float time);
		
		void DeleteData();
		void SetDatapoint(T point, int index);
		void SetDatapoint(T* point);

		T* data;
		float loadupTime;
		int arraySize;

	private:
		//Datapoints to pass
		bool isArray;
};
#endif