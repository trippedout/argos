#version 120

uniform float frame;

varying float life;
varying float seed;

void main(void) 
{
	float max_alpha = 1.0;
	float alpha = max_alpha;

	if(life < 60) alpha = max_alpha * life/60;

	//additive (literally derp)
	gl_FragColor = vec4(.25,.55,.85,alpha);// + vec4(gl_FrontColor.xyz, 0);
}

