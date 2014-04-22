uniform int direction;
uniform sampler2D mapTexture;
uniform sampler2D vignetteAlpha;
uniform sampler2D vignetteBlur;
varying vec2 texture_coordinate; 
varying vec3 normal;
varying vec4 vertex;

vec4 baseColor;
vec4 blurVec;
vec4 alphaVec;

float weights[21];

float map(float value, float start1, float stop1, float start2, float stop2) 
{
    return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
}

void main()
{
	//set color immediately just for testing
	gl_FragColor = texture2D(mapTexture, texture_coordinate);
	
	//store base texture and alpha/blur textures
	baseColor = texture2D(mapTexture, texture_coordinate);
	blurVec = texture2D(vignetteBlur, texture_coordinate);
	alphaVec = texture2D(vignetteAlpha, texture_coordinate);


	//alpha out pixels that dont matter and gtfo so we save cycles
	if(alphaVec.a == 1.0)
	{
		gl_FragColor.a = 0.0;
		return;
	}
	
	
	//BOX BLURRRR
	//*
	const float MAX_BLUR_RADIUS = 25.0;	
	float blurAmount = 0.0;	
	
	//cheating with blur vignette alpha channel
	blurAmount = blurVec.a * MAX_BLUR_RADIUS;
	
	float count = 0.0;
	vec4 colour = vec4(0.0);
	vec2 texelOffset;
	
	//get direction uniform for horizontal and vertical blurring
	if ( direction == 0 )
		texelOffset = vec2(1.0/1500.0, 0.0);
	else
		texelOffset = vec2(0.0, 1.0/1000.0);
		
	if( blurAmount >= 1.0 )
	{
		float halfBlur = blurAmount * 0.5;
		for (float i = 0.0; i < MAX_BLUR_RADIUS; ++i)
		{
			if ( i >= blurAmount )
				break;
			
			float offset = i - halfBlur;
			vec2 vOffset = texture_coordinate + (texelOffset * offset);

			colour += texture2D(mapTexture, vOffset);
			++count;
		}
	}
	
	// Apply colour	
	if ( count > 0.0 )
		gl_FragColor = colour / count;
	else
		gl_FragColor = baseColor;	
	//*/
	
	
	
	//GAUSSIAN BLUR
	/*
	const float MAX_BLUR_RADIUS = 5.0;	
	float yVal = vertex.y;
	float blurAmount = 0.0;	

	blurAmount = min(floor(blurAmount), MAX_BLUR_RADIUS);
	
	vec2 sampleOffset = vec2( blurAmount * .00008,blurAmount * .00008);

	weights[0] = 0.0091679276560113852;
	weights[1] = 0.014053461291849008;
	weights[2] = 0.020595286319257878;
	weights[3] = 0.028855245532226279;
	weights[4] = 0.038650411513543079;
	weights[5] = 0.049494378859311142;
	weights[6] = 0.060594058578763078;
	weights[7] = 0.070921288047096992;
	weights[8] = 0.079358891804948081;
	weights[9] = 0.084895951965930902;
	weights[10] = 0.086826196862124602;
	weights[11] = 0.084895951965930902;
	weights[12] = 0.079358891804948081;
	weights[13] = 0.070921288047096992;
	weights[14] = 0.060594058578763092;
	weights[15] = 0.049494378859311121;
	weights[16] = 0.0386504115135431;
	weights[17] = 0.028855245532226279;
	weights[18] = 0.020595286319257885;
	weights[19] = 0.014053461291849008;
	weights[20] = 0.00916792765601138;


	vec3 sum = vec3( 0.0, 0.0, 0.0 );
	vec2 baseOffset = -10.0 * sampleOffset;
	vec2 offset = vec2( 0.0, 0.0 );
	for( int s = 0; s < 21; ++s ) {
		sum += texture2D( mapTexture, texture_coordinate.st + baseOffset + offset ).rgb * weights[s];
		offset += sampleOffset;
	}
	gl_FragColor.rgb = sum;
	gl_FragColor.a = 1.0;
	
	// if( vertex.y < low )
	// {	
		// gl_FragColor.a = 0.0;
	// }
	// else if( vertex.y < max )
		// gl_FragColor.a = (vertex.y - low) / total;	
		*/
		
	//BLOOOOOOM
	/*
	if( direction == 1 )
	{
		vec4 sum = vec4(0);
		int i,j;

		for( i= -2 ;i < 4; i++)
		{
			for (j = -2; j < 3; j++)
			{
				sum += baseColor; //texture2D(mapTexture, texture_coordinate + vec2(j, i)*0.002) * 0.2;
			}
		}
		
		if (texture2D(mapTexture, texture_coordinate).r < 0.1)
		{
			gl_FragColor += sum*sum*0.002;// + texture2D(mapTexture, texture_coordinate);
		}
		else
		{
			if(texture2D(mapTexture, texture_coordinate).r < 0.2)
			{
				gl_FragColor += sum*sum*0.0015;// + texture2D(mapTexture, texture_coordinate);
			}
			else
			{
				gl_FragColor += sum*sum*0.00085;// + texture2D(mapTexture, texture_coordinate);
			}
		}
	}	
	//*/

	
	//apply alpha vec on the second pass
	if(direction==1)
	{
		if(gl_FragColor.a > 0.0)
			gl_FragColor.a *= abs(alphaVec.a - 1.0);
	}
	
	int py = texture_coordinate.y * 1080;
	
	//scanlines
	//*
	if(mod((py / 3), 2) == 0)
	{
		float lessThan = 0.84, alpha = 0.88;
	
		if(gl_FragColor.r < lessThan && gl_FragColor.g < lessThan && gl_FragColor.b < lessThan)
			gl_FragColor.rgb *= vec3(alpha,alpha,alpha);
	}
	//*/
	
	//gl_FragColor = alphaVec;
}

//old method of finding which pixels to blur
//float entry = 350.0, cutoff = 625.0, high = 750.0;
// if( yVal < entry )
	// blurAmount = MAX_BLUR_RADIUS;
// else if( yVal < cutoff )
	// blurAmount = map(yVal, entry, cutoff, MAX_BLUR_RADIUS, 0.0); 
// else if( yVal > high )
	// blurAmount = (yVal-high) / (1080.0-high) * 15.0;
