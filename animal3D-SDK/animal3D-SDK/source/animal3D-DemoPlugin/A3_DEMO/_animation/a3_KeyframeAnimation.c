/*
	Copyright 2011-2019 Daniel S. Buckstein

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Daniel S. Buckstein
	
	a3_KeyframeAnimation.c
	Implementation of keyframe animation interfaces.
*/

#include "a3_KeyframeAnimation.h"

#include <stdlib.h>
#include <string.h>


// macros to help with names
#define A3_CLIP_DEFAULTNAME		("unnamed clip")
#define A3_CLIP_SEARCHNAME		((clipName && *clipName) ? clipName : A3_CLIP_DEFAULTNAME)


//-----------------------------------------------------------------------------

// allocate keyframe pool
a3i32 a3keyframePoolCreate(a3_KeyframePool* keyframePool_out, const a3ui32 count)
{
	if (keyframePool_out && !keyframePool_out->keyframe && count)
	{
		const a3size sz = sizeof(a3_Keyframe) * count;
		a3ui32 i;

		// allocate and set pointer and count
		keyframePool_out->keyframe = (a3_Keyframe*)malloc(sz);
		if (keyframePool_out->keyframe)
		{
			// reset all keyframes in pool
			memset(keyframePool_out->keyframe, 0, sz);
			for (i = 0; i < count; ++i)
				keyframePool_out->keyframe[i].index = i;

			// done
			return (keyframePool_out->count = count);
		}
	}
	return -1;
}

// release keyframe pool
a3i32 a3keyframePoolRelease(a3_KeyframePool* keyframePool)
{
	if (keyframePool && keyframePool->keyframe)
	{
		// release and reset keyframe pointer in pool
		free(keyframePool->keyframe);
		keyframePool->keyframe = 0;
		return keyframePool->count;
	}
	return -1;
}

//Part 2 of 2: DONE
// initialize keyframe
a3i32 a3keyframeInit(a3_Keyframe* keyframe, const a3real dur, const a3ui32 value_x)
{
	if (keyframe)
	{
		// set keyframe data
		keyframe->dur = a3maximum(dur, 0);
		keyframe->durInv = a3recipsafe(keyframe->dur);
		keyframe->value = value_x;

		// done
		return keyframe->index;
	}
	return -1;
}


// allocate clip pool
a3i32 a3clipPoolCreate(a3_ClipPool* clipPool_out, const a3ui32 count)
{
	if (clipPool_out && !clipPool_out->clip && count)
	{
		const a3size sz = sizeof(a3_Clip) * count;
		a3ui32 i;

		// allocate and save
		clipPool_out->clip = (a3_Clip*)malloc(sz);
		if (clipPool_out->clip)
		{
			// reset all clips
			memset(clipPool_out->clip, 0, sz);
			for (i = 0; i < count; ++i)
				clipPool_out->clip[i].index = i;

			// done
			return (clipPool_out->count = count);
		}
	}
	return -1;
}

// release clip pool
a3i32 a3clipPoolRelease(a3_ClipPool* clipPool)
{
	if (clipPool && clipPool->clip)
	{
		// release and reset
		free(clipPool->clip);
		clipPool->clip = 0;
		return clipPool->count;
	}
	return -1;
}

// initialize clip with first and last indices
// duration is calculated as the total of keyframe durations
a3i32 a3clipInit(a3_Clip* clip, const a3byte clipName[a3keyframeAnimation_nameLenMax], const a3_KeyframePool* keyframePool, const a3ui32 firstKeyframeIndex, const a3ui32 finalKeyframeIndex)
{
	if (clip && keyframePool && keyframePool->keyframe && firstKeyframeIndex < keyframePool->count && finalKeyframeIndex >= firstKeyframeIndex)
	{
		// copy name
		strncpy(clip->name, A3_CLIP_SEARCHNAME, a3keyframeAnimation_nameLenMax);
		clip->name[a3keyframeAnimation_nameLenMax - 1] = 0;

		// ****TO-DO
		// calculate total frames
		clip->first = firstKeyframeIndex;
		clip->final = finalKeyframeIndex;
		clip->numKeys = 1 + finalKeyframeIndex - firstKeyframeIndex;

		// set keyframe list from pool
		clip->keyframeListBasePtr_pool = keyframePool->keyframe;

		// calculate duration
		a3clipCalculateDuration(clip);

		// done
		return clip->numKeys;	// something meaningful wow
	}
	return -1;
}

// get clip index from pool
a3i32 a3clipGetIndexInPool(const a3_ClipPool* clipPool, const a3byte clipName[a3keyframeAnimation_nameLenMax])
{
	if (clipPool && clipPool->clip)
	{
		const a3byte* searchName = A3_CLIP_SEARCHNAME;
		a3ui32 i;

		// look for first clip with name, return index
		// if not found, function exits with -1
		for (i = 0; i < clipPool->count; ++i)
			if (strcmp(searchName, clipPool->clip[i].name) == 0)
				return i;
	}
	return -1;
}


//-----------------------------------------------------------------------------
