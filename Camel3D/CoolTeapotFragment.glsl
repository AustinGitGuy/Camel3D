
varying vec4 texCoord;

void main(){
	if(texCoord.x == 0.0 && texCoord.y == 0.0 && texCoord.z == 0.0){
		gl_FragColor = vec4(0.0, 1.0, 1.0, 1.0);
	}
	else {
		gl_FragColor = vec4(texCoord.xyz, 1.0);
	}
}