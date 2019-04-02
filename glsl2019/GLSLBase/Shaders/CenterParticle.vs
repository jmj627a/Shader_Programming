#version 450

in vec3 a_Position;
in vec2 a_Temp;
in vec4 a_StartLife;

uniform float u_Time; //1.f
uniform bool u_Repeat = true; 
const float PI= 3.141592;

void main()
{
	vec3 newPos = a_Position.xyz;
	float startTime = a_StartLife.x;
	float lifeTime = a_StartLife.y;
	float ratio = a_StartLife.z;
	float amp = a_StartLife.w;

	//float newTime = fract(u_Time);
	float newTime = u_Time - startTime; //반복x

		if(newTime > 0)
	{
		newTime = mod(newTime, lifeTime);
		float life = newTime;
		float remainingLife = lifeTime - life;
		
		newPos.x  += newTime ;
		newPos.y  += sin(newTime * PI * 2); //sin(newTime * PI * 2); 정확히 한 주기

		if(u_Repeat == true)
		{
			remainingLife = 1.f;
			newTime = mod(newTime, lifeTime);
		}
		
		if(remainingLife < 0)
		{
			newPos = vec3(10000,10000,10000);
		}
	}
	else
	{
		newPos = vec3(10000,10000,10000);
	}

	gl_Position = vec4(newPos , 1);
}

