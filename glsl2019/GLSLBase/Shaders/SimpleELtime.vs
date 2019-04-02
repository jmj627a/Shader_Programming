#version 450

in vec3 a_Position;
in vec3 a_Vel;
in vec2 a_StartLife;

uniform float u_Time; //1.f
uniform bool u_Repeat = true; //1.f

const vec3 c_Gravity = vec3(0, -5, 0); //중력가속도
void main()
{
	vec3 newPos = a_Position.xyz;
	float startTime = a_StartLife.x;
	float lifeTime = a_StartLife.y;

	//float newTime = fract(u_Time); //0에서 1사이를 반복
	float newTime = u_Time - startTime; //반복x

	if(newTime > 0)
	{
		float life = newTime;
		float remainingLife = lifeTime - life;
		
		if(u_Repeat == true)
		{
			remainingLife = 1.f;
			newTime = mod(newTime, lifeTime);
		}
		
		if(remainingLife < 0)
		{
			newPos = vec3(10000,10000,10000);
		}
		newPos += a_Vel * newTime + 0.5f * c_Gravity * newTime * newTime ;
	}
	else
	{
		newPos = vec3(10000,10000,10000);
	}
	

	gl_Position = vec4(newPos , 1);
}


/*

위치 = 초기위치 + 초기속도 * 시간
속도 = 초기속도 + 가속도 * 시간

속도를 적분 --> 방향성 크기
가속도를 적분 --> 현재속도

위치 = 초기위치 + 초기속도 * 시간 + 1/2*가속도*시간^2


*/