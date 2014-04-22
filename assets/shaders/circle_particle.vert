uniform float frame;

varying float life;
varying float seed;

void main(void)
{
	life = gl_Vertex.z;
	seed = gl_Vertex.w;
	gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_Vertex.xy, 0, 1);
	gl_PointSize =  1.1;
}