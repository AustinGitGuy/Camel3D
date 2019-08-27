
varying vec3 normalEyeSpace;
varying vec3 vEyeSpace;

uniform vec3 position;

void main(void){
	 vec4 diffuse = vec4(vec3(max(dot(normalEyeSpace, normalize(position - vEyeSpace)), 0.0)), 1.0);

	 gl_FragColor = diffuse;
}