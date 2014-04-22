
varying vec2 texture_coordinate;

void main()
{
	//BLOOOOOOM
	/*
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
	//*/
}