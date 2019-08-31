
varying vec4 pos;
varying vec4 texCoord;

varying vec4 TexCoord;

void main(){
	TexCoord = texCoord;
	pos = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_Position = pos;
}