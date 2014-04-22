#version 120
uniform sampler2D tex;

varying float seed;
varying float phase;
varying float alpha;

void main(void) 
{
	gl_FragColor = texture2D(tex, gl_PointCoord);
	gl_FragColor.a *= .65;// // texture2D(tex, gl_PointCoord);
	//gl_FragColor = vec4(0.51, 0.98, 1.0, .5);
	//gl_FragColor.a *= alpha;
}