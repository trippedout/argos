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
	//worldPosition = gl_ModelViewMatrix * gl_Vertex;
	//worldN = normalize( vec4( gl_NormalMatrix * gl_Normal, 0 ) );
	//vec4 viewDir4 = normalize( camLoc - worldPosition );
	//viewDir = vec3( viewDir4.xyz );
	texture_coordinate = vec2(gl_MultiTexCoord0);
}
