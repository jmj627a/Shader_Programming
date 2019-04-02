#version 450

in vec3 a_Position;
in vec3 a_Vel;

uniform float u_Time; //1.f

const vec3 c_Gravity = vec3(0, -5, 0); //중력가속도
void main()
{
	vec3 newPosition = a_Position.xyz;

	//float newTime = fract(u_Time); //0에서 1사이를 반복
	float newTime = u_Time; //반복x

	newPosition += a_Vel * newTime + 0.5f * c_Gravity * newTime * newTime ;

	gl_Position = vec4(newPosition , 1);
}


/*

위치 = 초기위치 + 초기속도 * 시간
속도 = 초기속도 + 가속도 * 시간

속도를 적분 --> 방향성 크기
가속도를 적분 --> 현재속도

위치 = 초기위치 + 초기속도 * 시간 + 1/2*가속도*시간^2


*/