
void main(void)
{
	float seed = gl_Color.r;
	//float phase = sin(seed);
	//gl_Vertex.z += phase * 4.0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}