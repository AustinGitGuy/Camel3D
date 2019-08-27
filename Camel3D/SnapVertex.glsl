
varying vec4 pos;

void main(){
	pos = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_Position = pos;
	gl_FrontColor = gl_Color;
}