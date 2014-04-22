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
	
	float px = texcoord.x * 256.;
	
	float stripAlpha = 1.0; 
	float x = texcoord.x;
	float y = texcoord.y;
	
	//this creates a perfect loop
	vec2 pos = vec2( x, y + angle/(PI*2)*2 - 1 );

	// uses a simple gradient feather the top and bottom. 
	// angle*2, then cap it at 1 to make the gradient 'harsher' 
	float outsideAlpha = sin( texcoord.y * PI ) * 2;
	if(outsideAlpha > 1) outsideAlpha =1;
	
	//this is a big buncha garbage but basically, 
	// it uses mod to separate the image 
	// into 2px vertical strips, then independently animate them. 
	// this should actually be pretty fast.
	
	
	if( mod(px,32) == 0 ){
		// 
		stripAlpha=1.0;
		pos = vec2( x, (y + ay + ax*0.2) );
	}else if( mod(px,16) == 0 ){
		//thin long vertical
		stripAlpha = 0;
		pos = vec2( x, y + angle/PI + ax*0.1 - 1 );
	}else if( mod(px,8) == 0 ){
		// circles
		//stripAlpha=0;
		pos = vec2( x + ay, ax);
	}else if( mod(px,4) == 0 ){
		//fast ripples
		stripAlpha = ax;
		pos.y -= ax;
	}else if( mod(px,2) == 0 ){
		//normal loop - offset
		stripAlpha = 1;
		pos.y -= ax*0.2;
	}else{
		//normal loop + offset
		stripAlpha = ax + a*.2;
		pos.y += ax*0.1;
		pos.y*=2;
	}
	
	if(pos.y >1) pos.y -=1;
	if(pos.y <0) pos.y +=1;
		
	// draw the previously animated 'particle strips' in color2 
	// copy that and offset it a little bit to make them thicker (color3). 
	// copy and undulate the original image underneath in color 4 and 5.  
	
	vec4 color2 = vec4(texture2D(texsampler, pos).rgba) * stripAlpha;
	//vec4 color3 = vec4(texture2D(texsampler, pos + vec2(0.01,0.2)).rgba) * stripAlpha * 0.5;
	vec4 color4 = vec4(texture2D(texsampler,texcoord + vec2( ax * 0.05, 0)).rgb, 1.0 - texcoord.x);
	vec4 color5 = vec4(texture2D(texsampler,texcoord + vec2( 0, ay*0.05 - ax * ax * ay * a * 0.1)).rgb, 1.0 - texcoord.y);
	
	//normal image not used any more. 
	//vec4 color = vec4(texture2D(texsampler, texcoord).rgba) * 0.3;
	
	// outside alpha just uses a sin curve to feather the top and bottom of the image. 
	gl_FragColor= ((color2 + color4*0.2 + color5*0.1) * outsideAlpha) * alpha; 
}