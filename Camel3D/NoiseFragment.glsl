
varying vec4 pos;
varying vec4 TexCoord;

precision mediump float;

uniform sampler2D terrain;

float rand(in vec2 val){
    return fract(cos(dot(val, vec2(5235.98, 124.3243))) * 43758.5453123);
}

float noise(in vec2 val){
	return 0;
}

void main(){
	float noiseVal = rand(pos.xy);

    gl_FragColor = vec4(vec3(noiseVal), 1.0);
	//gl_FragColor = TexCoord;
}