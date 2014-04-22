uniform sampler2D mapTexture;
uniform sampler2D alphaTexture;
varying vec2 texture_coordinate; 
varying vec4 vertex;

vec4 baseColor;
vec4 alphaVec;

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
	alphaVec = texture2D(alphaTexture, texture_coordinate);


	//alpha out pixels that dont matter and gtfo so we save cycles
	if(alphaVec.a == 1.0)
	{
		gl_FragColor.a = 0.0;
		return;
	}
	
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

	if(gl_FragColor.a > 0.0)
		gl_FragColor.a *= abs(alphaVec.a - 1.0);
		
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
