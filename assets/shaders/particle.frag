#version 120

uniform sampler2D tex;
varying vec2 pos;
varying float distance;

int b(int times)
{
	int baseDistance = 525;
	int inc = 4;
	return baseDistance + (inc * times);
}

void main(void) 
{
	int max = 1920;
	int bounds = 1250;

	//get basics for measurement
	//float texWidth = gl_PointCoord.x / 4.0;
	//float texHeight = gl_PointCoord.y / 2.0;

	//new sprite sheet is 4x4 so size is .25 for each
	vec2 textureSize = vec2( .25, .25 );

	//hard code row and columns
	float col = 0.25;
	float row = 0.5;
		
	//set default at top left
	vec2 texPos = vec2(0,0);
	
	//if outside the bounds, fade in the alpha
	if(pos.x >= bounds)
	{
		vec2 realTexCoord = texPos + ( gl_PointCoord * textureSize );
		vec4 texColor = texture2D(tex, realTexCoord);
		
		float dif = max - pos.x;
		float alpha = ( dif / (max-bounds) );
		
		gl_FragColor = vec4(texColor.rgb, texColor.a * alpha);
	}
	else
	{
		float d = distance;
		//float d = pos.x;
		if(d < b(1) )		texPos = vec2(.75, .75);
		else if(d < b(2) )	texPos = vec2(.5, .75);
		else if(d < b(3) )	texPos = vec2(.25, .75);
		else if(d < b(4) )	texPos = vec2(0, .75);
		else if(d < b(5) )	texPos = vec2(.75, .5);
		else if(d < b(6) )	texPos = vec2(.5, .5);
		else if(d < b(7) )	texPos = vec2(.25, .5);
		else if(d < b(8) )	texPos = vec2(0, .5);
		else if(d < b(9) )	texPos = vec2(.75, .25);
		else if(d < b(10) )	texPos = vec2(.5, .25);
		else if(d < b(11) )	texPos = vec2(.25, .25);
		else if(d < b(12) )	texPos = vec2(0, .25);
		else if(d < b(13) )	texPos = vec2(.75, 0);
		else if(d < b(14) )	texPos = vec2(.5, 0);
		else if(d < b(15) )	texPos = vec2(.25, 0);
		else
			texPos = vec2(0,0);

		vec2 realTexCoord = texPos + ( gl_PointCoord * textureSize );
		vec4 texColor = texture2D(tex, realTexCoord);
		
		gl_FragColor = texColor;
	}
}

