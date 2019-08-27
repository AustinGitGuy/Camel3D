
varying vec3 normalEyeSpace;
varying vec3 vEyeSpace;

void main(void){
   vEyeSpace = vec3(gl_ModelViewMatrix * gl_Vertex);
   normalEyeSpace = normalize(gl_NormalMatrix * gl_Normal);
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;  
}