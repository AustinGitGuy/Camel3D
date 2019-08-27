
uniform vec4 newColor;

void main(){
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = newColor;
}