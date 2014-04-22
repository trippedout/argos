#version 110

varying vec2 texture_coordinate;
varying vec4 vertex;

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	vertex = gl_Vertex;
	
	texture_coordinate = vec2(gl_MultiTexCoord0);
}
