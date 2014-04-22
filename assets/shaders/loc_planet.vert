#version 120

varying vec2 planet_tex_coord;
varying vec2 noise_tex_coord;
varying vec4 vertex;

void main() 
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	planet_tex_coord = vec2(gl_MultiTexCoord0);
	noise_tex_coord = vec2(gl_MultiTexCoord0);
}