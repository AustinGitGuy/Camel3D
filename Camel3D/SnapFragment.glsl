
uniform float uTime;

varying vec4 pos;

float rand(in vec2 val){
    return fract(cos(dot(val, vec2(5235.98, 124.3243))) * 43758.5453123);
}

void main(){
	float noiseVal = rand(pos.xy);
	gl_FragColor = gl_Color;

	if(uTime > noiseVal * 10){
		gl_FragColor.w = 0;
	}
}