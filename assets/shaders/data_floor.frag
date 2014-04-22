#version 120

uniform sampler2D tex;
uniform sampler2D overlay;
varying vec2 texture_coordinate;
varying vec4 vertex;

void main(void) 
{	
	vec4 color = texture2D(tex, texture_coordinate);
	vec4 overlay = texture2D(overlay, texture_coordinate + vec2(0.0, -.05));
	gl_FragColor = (color * .7f) * (overlay * 1.2);
}