#version 120

uniform float frame;

varying float life;
varying float seed;

void main(void) 
{
	float max_alpha = 0.5;
	float alpha = max_alpha;
	float fade_in = 150.0;
	float max_life = 200.0;
	
	if(life > fade_in) alpha = max_alpha - ( max_alpha * (life-fade_in)/(max_life-fade_in) );

	if(life < 100.0) alpha = max_alpha * life/100.0;

	//additive (literally derp)
	gl_FragColor = vec4(.25,.55,.85,alpha);// + vec4(gl_FrontColor.xyz, 0);
}

