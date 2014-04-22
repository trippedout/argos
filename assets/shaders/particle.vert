#version 120

varying vec2 pos;
varying float seed;
varying float distance;

void main(void)
{
	//set position
	vec4 vert = vec4(gl_Vertex.xyz, 1);	
	vec4 eyecoord = gl_ModelViewMatrix * vert;
	gl_Position = gl_ProjectionMatrix * eyecoord;
	pos = gl_Vertex.xy;
	seed = gl_Vertex.w;
		
	//set size
	distance = sqrt( eyecoord.x * eyecoord.x + eyecoord.y * eyecoord.y + eyecoord.z * eyecoord.z );
	float attenuation = 4000.0 / distance;
	
	gl_PointSize =  3 * attenuation * seed;
	gl_FrontColor = gl_Color;
}