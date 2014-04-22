#version 120

varying float seed;
varying float phase;
varying float alpha, mainAlpha;

float map(float value, float start1, float stop1, float start2, float stop2) {
    return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
 }
 
void main(void)
{
	seed = gl_Vertex.w;
	phase = sin(seed);
	
	//wiggle just a lil bit
    float nx = gl_Vertex.x + phase * 2.75;
    float ny = gl_Vertex.y + cos(seed) * 2.34;
    float nz = gl_Vertex.z + sin(gl_Color.g) * 2.32;
//	gl_Vertex.x += phase * 2.75;
//	gl_Vertex.y += cos(seed) * 2.34;
//	gl_Vertex.z += sin(gl_Color.g) * 2.32;
	
	gl_Position = gl_ModelViewProjectionMatrix * vec4(vec3(nx,ny,nz), 1.0);
		
	//set base sizes
	gl_PointSize = 2.5 + (phase*.25) * 2.0;
	alpha = 1.0;
	
	//set extras from GL_COLOR_ARRAY
	mainAlpha = gl_Color.r;
	
	//set z alpha
	float zcutoff = 500.0;
	float zmax = 1100.0;
	float xcutoff = 40.0;
	float xmax = 300.0;
	
	if(gl_Position.z > zmax || gl_Position.x > xmax ) 
	{
		alpha = 0.0;
		return;
	}
	
	if(gl_Position.z >= zcutoff)
		alpha = map(gl_Position.z, zcutoff, zmax, 1.0, 0.0);
			
	if( gl_Position.x > xcutoff && gl_Position.z < zcutoff) 
		alpha = map(gl_Position.x, xcutoff, xmax, 1.0, 0.0);

		
}