varying vec2 texcoord;
uniform sampler2D texsampler;
uniform float angle;
uniform float speed;
uniform float alpha; 

void main() {

	float PI = 3.14159265359;
	float a = sin( angle - PI );
	float ax = cos( texcoord.x * PI * 2. + angle );
	float ay = cos( texcoord.y * PI * 2. + angle );
	
	float s = speed ; 
	
	
	vec4 color = vec4(texture2D(texsampler,texcoord + vec2( ay * 0.005 * s * -1., 0)).rgb,1.0);
	
	vec4 color2 = vec4(texture2D(texsampler,texcoord + vec2( ax * 0.005 * s, 0)).rgb, 1.0 - texcoord.x);
	vec4 color3 = vec4(texture2D(texsampler,texcoord + vec2( 0, ay*0.005)).rgb, 1.0 - texcoord.y);
	
	color2 *= vec4(2.0, (0.8 + ax * 0.2), (0.8 + ax * 0.2),1.0); 
	color3 *= vec4((0.8 + ay * 0.2), (0.8 + ay * 0.2), (0.8 + ay * 0.2) * 2.0, 1.0);
	
	gl_FragColor= ( color * (0.6 + ay * 0.2) + color2 * (0.2 * (1.0-ay)) + color3 * (0.2 * (1.0-ay)) ) * alpha;
}