uniform int direction;
uniform float inBlurAmount;
uniform float inAlphaAmount;
uniform sampler2D texture;
varying vec2 texture_coordinate; 
varying vec4 vertex;

vec4 baseColor;

void main()
{
	//set color immediately just for testing
	gl_FragColor = texture2D(texture, texture_coordinate);
	
	//store base texture and alpha/blur textures
	baseColor = texture2D(texture, texture_coordinate);

	if(inBlurAmount == 0.0)
	{
		return;
	}
	
	//BOX BLURRRR
	//*
	const float MAX_BLUR_RADIUS = 25.0;	
	float blurAmount = 0.0;	
	
	//cheating with blur vignette alpha channel
	blurAmount = inBlurAmount * MAX_BLUR_RADIUS;
	
	float count = 0.0;
	vec4 colour = vec4(0.0);
	vec2 texelOffset;
	
	//get direction uniform for horizontal and vertical blurring
	if ( direction == 0 )
		texelOffset = vec2(1.0/1920.0, 0.0);
	else
		texelOffset = vec2(0.0, 1.0/1080.0);
		
	if( blurAmount >= 1.0 )
	{
		float halfBlur = blurAmount * 0.5;
		for (float i = 0.0; i < MAX_BLUR_RADIUS; ++i)
		{
			if ( i >= blurAmount )
				break;
			
			float offset = i - halfBlur;
			vec2 vOffset = texture_coordinate + (texelOffset * offset);

			colour += texture2D(texture, vOffset);
			++count;
		}
	}
	
	// Apply colour	
	if ( count > 0.0 )
		gl_FragColor = colour / count;
	else
		gl_FragColor = baseColor;	
	//*/
	
	gl_FragColor *= inAlphaAmount;
	
	//apply alpha vec on the second pass
	// if(direction==1)
	// {
		// if(gl_FragColor.a > 0.0)
			// gl_FragColor.a *= abs(alphaVec.a - 1.0);
	// }
	
	
	
	//scanlines
	/*
	int py = texture_coordinate.y * 1080;
	
	if(mod((py / 3), 2) == 0)
	{
		float lessThan = 0.84, alpha = 0.88;
	
		if(gl_FragColor.r < lessThan && gl_FragColor.g < lessThan && gl_FragColor.b < lessThan)
			gl_FragColor.rgb *= vec3(alpha,alpha,alpha);
	}
	//*/
	
	//gl_FragColor = alphaVec;
}
