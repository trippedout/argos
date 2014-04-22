varying vec2 texcoord;
uniform sampler2D texsampler;
uniform float alpha;

void main() {

	float PI = 3.14159265359;
	
	// use sin and cos curves based on x and y to warp the image in a circle. 
	// this creates an offset for each pixel based on where it is on the x/y plane. 
	// remember that the entire span of the texture for x and y 0-1
	
	float offsetx = sin((1. - texcoord.x) * 2. * PI) * cos(texcoord.y * 2. * PI) * 0.005;
	float offsety = sin((1. - texcoord.y) * 2. * PI) * cos(texcoord.x * 2. * PI) * 0.003;
	
	//this uncomment to re-enable 'bending' effect. 
	//vec2 offset = vec2( offsetx, offsety );
	vec2 offset = vec2( 0, 0 );
	
	vec4 color;
	
	//the background quads have their 'alpha' set lower.
	// i.e. the front quad, draws normally, 
	// the back 2 quads have a simple 'blur' applied to them. 
	// by grabbing the 4 neighbouring pixles and adding their color to the current pixel. 
	if(alpha < 1.0){
		color = vec4(texture2D(texsampler, texcoord + offset).rgba);
		color += vec4(texture2D(texsampler, texcoord + offset + vec2(0.001,0)).rgba);
		color += vec4(texture2D(texsampler, texcoord + offset + vec2(-0.001,0)).rgba);
		color += vec4(texture2D(texsampler, texcoord + offset + vec2(0,0.001)).rgba);
		color += vec4(texture2D(texsampler, texcoord + offset + vec2(0,-0.001)).rgba);
		color /= 4.;
		
		// skew the color a bit, tints the color of the background images more blue. 
		color *= vec4(0.3 + alpha, 0.4 + alpha, 1.0, alpha); 
		
	}else{
		//normal foreground plane, only offset the color based on the sine/cos offset above.
		color = vec4(texture2D(texsampler, texcoord + offset).rgba) * vec4(1,1,1,alpha);
	}
	
	//set the final output color. 
	gl_FragColor= color; 
}