#version 120

varying float seed;
varying float phase;
varying float alpha, mainAlpha;

void main(void) 
{
	gl_FragColor = vec4(1.0,1.0,1.0,1.0); //texture2D(tex, gl_PointCoord);
	gl_FragColor.a *= alpha;
	gl_FragColor.a *= mainAlpha;
}