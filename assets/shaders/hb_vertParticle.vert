varying float seed;
varying float phase;
varying float alpha;

void main(void)
{
	seed = gl_Vertex.w;
	phase = sin(seed);
	
	//float seed2 = gl_Color.r;
	
	//float xoff = phase * 1.15;
	//float yoff = cos(seed2) * 1.15;
	
	//gl_Vertex.x += xoff;
	//gl_Vertex.y += yoff;
	gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_Vertex.xyz, 1.0);
	
	//set alpha based on mExtraData vec4.a
	//alpha = gl_Color.a;
	
	//set point size based on Particle mSeed stored in mExtraData vec4.r
	//float pointPhase = sin(seed2);
	gl_PointSize =  12.0 + (phase * 4.0 - 2.5);
}