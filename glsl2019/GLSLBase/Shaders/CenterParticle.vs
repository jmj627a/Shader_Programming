#version 450

in vec3 a_Position;
in vec2 a_Temp;
in vec4 a_StartLife;

uniform float u_Time; //1.f
//uniform bool u_Repeat = true; 
const float PI= 3.141592;

void main()
{
	vec3 newPos = a_Position.xyz;
	float startTime = a_StartLife.x;
	float lifeTime = a_StartLife.y;
	float ratio = a_StartLife.z;
	float amp = a_StartLife.w;

	float newTime = u_Time - startTime; //반복x

		if(newTime > 0)
	{
		newTime = mod(newTime, lifeTime);
		
		amp = amp * newTime  ;
		newPos.x  += newTime ;
		newPos.y  += sin(newTime * PI * 2 * ratio) * amp; //sin(newTime * PI * 2); 정확히 한 주기
		//sin안쪽은 주기를 조절, 바깥쪽은 폭을 조절
	}
	else
	{
		newPos = vec3(10000,10000,10000);
	}

	gl_Position = vec4(newPos.xyz , 1);
}

