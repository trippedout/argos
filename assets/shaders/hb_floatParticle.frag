#version 120
uniform sampler2D tex;

varying float seed;
varying float phase;
varying float alpha;

void main(void) 
{
	vec4 color = texture2D(tex, gl_PointCoord);
	gl_FragColor = vec4( 0.51f, 0.98f, 1.0f, color.a );
	gl_FragColor.a *= alpha * .25;
}