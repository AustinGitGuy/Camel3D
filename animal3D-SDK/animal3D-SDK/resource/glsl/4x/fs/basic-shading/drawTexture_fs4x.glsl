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
	
	drawTexture_fs4x.glsl
	Draw texture sample using texture coordinate from prior shader.
*/

#version 410

// **INSTRUCTIONS: 
//	1) declare varying to read texture coordinate
//		-> *test varying by outputting it as color; shows red-green gradient
//	2) declare texture uniform for diffuse map
//	3) sample texture in main
//	4) copy texture sample to output

in vec2 vTexCoord; // (1)

out vec4 rtFragColor;

uniform sampler2D uTex_dm; // (2)

void main(){
	vec4 texData = texture(uTex_dm, vTexCoord); // (3)
	rtFragColor = texData; // (4)
}