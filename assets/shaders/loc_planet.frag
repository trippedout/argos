#version 120

uniform sampler2D planetTex;
uniform sampler2D noiseText;
uniform float frame;

varying vec2 planet_tex_coord;
varying vec3 noise_tex_coord;
varying vec4 noiseColor;

void main() 
{
	gl_FragColor = texture2D(planetTex, planet_tex_coord);
    vec4 noiseColorNew = texture2D(noiseText, planet_tex_coord);
//	noiseColor = texture2D(noiseText, planet_tex_coord);
	
	//vec3 noise = noiseColor.xyz;
	//float noiseLum = (0.2126*noiseColor.r) + (0.7152*noiseColor.g) + (0.0722*noiseColor.b);
	
	//float max_reach = 10.0;
	float phase = (cos(frame * 0.025f) + 1.0) / 2.0;;
	
	// BASIC SHADER
	//*
	float alpha = .75;
	
	float PI = 3.14159265359;
	//float a = sin( angle - PI );
	float ax = cos( planet_tex_coord.x * PI * 2 + (frame * 0.05) );
	float ay = cos( planet_tex_coord.y * PI * 2 + (frame * 0.05) );
	
	float s = 0.0 ; 
	
	
	vec4 color = vec4(texture2D(planetTex,planet_tex_coord ).rgb, 1.0);	
	vec4 color2 = vec4(texture2D(planetTex,planet_tex_coord + vec2( phase * 0.025, 0)).rgb, 1.0 - planet_tex_coord.x);
	vec4 color3 = vec4(texture2D(planetTex,planet_tex_coord + vec2( phase * 0.002, phase*0.004)).rgb, 1.0 - planet_tex_coord.y);
	
	color2 *= vec4(2.0, (0.8 + ax * 0.2), (0.8 + ax * 0.2),1.0); 
	color3 *= vec4((0.8 + ay * 0.2), (0.8 + ay * 0.2) * 2, (0.8 + ay * 0.2),1.0); 
	
	gl_FragColor = (color * (0.8 + ay * 0.2) + color2 * (0.25 * (1-ay)) + color3 * (0.25 * (1-ay))) * 0.85 * alpha; 
	gl_FragColor.xyz *= (noiseColorNew.xyz * phase);
	//gl_FragColor.xyz *= noiseLum;
	gl_FragColor.a = noiseColorNew.a;
	
	//*/
}