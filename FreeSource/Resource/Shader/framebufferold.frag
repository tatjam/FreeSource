#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;

uniform int doEffect;

const float offset = 1.0 / 300;  
const float s = 16;




float avgcol(vec3 col)
{
	return max(max(col.x, col.y), max(col.y, col.z));
}

vec3 edgefunc(vec3 col, float min)
{
	float avg = avgcol(col);
	if(avg > min)
	{
		return col;
	}
	else
	{
		return vec3(0.0f);
	}
}


void main()
{ 
	if(doEffect == 1)
	{
		// 0  1  2  3  4
		// 5  6  7  8  9
		// A  B  C  D  E
		// F  G  H  I  J
		// K  L  M  N  O

		vec2 offsets[25] = vec2[](
			vec2(-2 * offset, -2 * offset), //0
			vec2(-1 * offset, -2 * offset), //1
			vec2(-0 * offset, -2 * offset), //2
			vec2(+1 * offset, -2 * offset), //3
			vec2(+2 * offset, -2 * offset), //4
			vec2(-2 * offset, -1 * offset), //5
			vec2(-1 * offset, -1 * offset), //6
			vec2(-0 * offset, -1 * offset), //7
			vec2(+1 * offset, -1 * offset), //8
			vec2(+2 * offset, -1 * offset), //9
			vec2(-2 * offset, -0 * offset), //A
			vec2(-1 * offset, -0 * offset), //B
			vec2(-0 * offset, -0 * offset), //C
			vec2(+1 * offset, -0 * offset), //D
			vec2(+2 * offset, -0 * offset), //E
			vec2(-2 * offset, +1 * offset), //F
			vec2(-1 * offset, +1 * offset), //G 
			vec2(-0 * offset, +1 * offset), //H
			vec2(+1 * offset, +1 * offset), //I
			vec2(+2 * offset, +1 * offset), //J
			vec2(-2 * offset, +2 * offset), //K
			vec2(-1 * offset, +2 * offset), //L 
			vec2(-0 * offset, +2 * offset), //M
			vec2(+1 * offset, +2 * offset), //N
			vec2(+2 * offset, +2 * offset) //O
		);

		// 0  1  2  1  0
		// 1  2  3  2  1
		// 2  3  4  3  2
		// 1  2  3  2  1
		// 0  1  2  1  0

		float kernel[25] = float[](
			 0.5f / s, 1.0f / s, 2.5f / s, 1.0f / s, 0.5f / s,
			 1.0f / s, 2.0f / s, 3.5f / s, 2.0f /s, 1.0f / s,
			 2.5f / s, 3.5f / s, 4.0f / s, 3.5f / s, 2.5f / s,
			 1.0f / s, 2.0f / s, 3.5f / s, 2.0f /s, 1.0f / s,
			 0.5f / s, 1.0f / s, 2.5f / s, 1.0f / s, 0.5f / s
		);
    
		vec3 sampleTex[25];


		for(int i = 0; i < 25; i++)
		{
			sampleTex[i] = 
				edgefunc(
					vec3(
						texture(screenTexture, TexCoords.st + offsets[i])
						), 0.80f
					);
			
		}
		vec3 col = vec3(0.0);
		for(int i = 0; i < 25; i++)
			col += sampleTex[i] * kernel[i];

		// col is the result of the kernel

		//color = vec4(col, 1.0f);

		vec3 result = vec3(texture(screenTexture, TexCoords));

		result += col / 2.0f;

		color = vec4(result, 1.0f);
	}
	else
	{
		color = vec4(vec3(texture(screenTexture, TexCoords)), 1.0f);
	}

    //color = vec4(vec3(resF, resF, resF), 1.0f);
}
