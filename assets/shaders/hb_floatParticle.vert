#version 120

varying float seed;
varying float phase;
varying float alpha;

void main(void)
{
	seed = gl_Vertex.w;
	phase = sin(seed);
	
	//float seed2 = gl_Color.r;
	
    float nx = gl_Vertex.x + phase * 15.0;
//	gl_Vertex.x += phase * 15.0;
	//float yoff = cos(seed2) * 1.15;
	
	//gl_Vertex.x += xoff;
	//gl_Vertex.y += yoff;
	
	gl_Position = gl_ModelViewProjectionMatrix * vec4(vec3(nx, gl_Vertex.yz), 1.0);
	
	//set alpha based on mExtraData vec4.a
	alpha = gl_Color.r / 60.0;
	
	//set point size based on Particle mSeed stored in mExtraData vec4.r
	//float pointPhase = sin(seed2);
	gl_PointSize = 13.0 + (phase * 2.0 - 1.0);
}