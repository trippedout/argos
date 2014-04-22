#version 110

varying vec2 texture_coordinate;
varying vec4 vertex;

void main()
{
	vertex = gl_Vertex;
	gl_Position = gl_ModelViewProjectionMatrix * vertex;
	
	texture_coordinate = vec2(gl_MultiTexCoord0);
}
