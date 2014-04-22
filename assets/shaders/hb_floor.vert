#version 110

uniform int direct;
varying vec2 texture_coordinate;
varying vec3 normal;
varying vec4 vertex;

void main()
{
	normal = gl_NormalMatrix * gl_Normal;	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	vertex = gl_Vertex;
	texture_coordinate = vec2(gl_MultiTexCoord0);
}
