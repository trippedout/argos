#version 120

varying vec2 texcoord;
uniform sampler2D texsampler;
uniform float angle;
uniform float alpha;

void main() {

	float PI = 3.14159265359;
	float a = sin( angle );

	float ax = cos( texcoord.x * PI * 2 + angle );
	float ay = sin( texcoord.y * PI * 2 + angle );
	
	//warp the base image a little based on the sin curve. 
	vec4 color1 = vec4(texture2D(texsampler,texcoord + vec2( ax * 0.002, 0)).rgba) * 0.9;
	
	// add a 0.1 alpha layer ontop warping it a lot, creates the 'breathing' effect
	vec4 color2 = vec4(texture2D(texsampler,texcoord + vec2( 0, ay*0.02 - ax * ax * ay * a * 0.01)).rgba) * 0.1;
	
	gl_FragColor= (color1 + color2) * alpha; 
}