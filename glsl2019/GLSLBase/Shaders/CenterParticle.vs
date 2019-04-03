#version 450

in vec3 a_Position;
in vec4 a_StartLife;
in vec3 a_Vel;
in float a_RandValue;

uniform float u_Time; 
const float PI= 3.141592;
const mat3 c_RP = mat3(0,-1,0,1,0,0,0,0,0);//openGL은 매트리스가 세로로 들어감
// c	s	0	90도		0	1	0
// -s	c	0	->		-1	0	0
// 0	0	0			0	0	0
const vec3 c_Gravity = vec3(0, -2, 0); //중력가속도
void main()
{
	vec3 newPos = a_Position.xyz;
	vec3 newVel = a_Vel.xyz;
	float startTime = a_StartLife.x;
	float lifeTime = a_StartLife.y;
	float ratio = a_StartLife.z;
	float amp = a_StartLife.w;

	float newTime = u_Time - startTime; //반복x

	if(newTime > 0)
	{
		newPos.x += sin(a_RandValue * PI * 2);
		newPos.y += cos(a_RandValue * PI * 2);

		//amp = amp * newTime;
		
		newTime = mod(newTime, lifeTime);
		
		newVel = newVel + c_Gravity * newTime; //새로 중력 반영해서 결정된 속도
		
		newPos = newPos + a_Vel * newTime + c_Gravity * 0.5 * newTime; //위치 계산
		//위에 a_Vel쓴건 초기속도가 적용되어야 하기 때문
		
		
		vec3 vSin = a_Vel * c_RP;
		newPos = newPos + vSin * sin(newTime * PI * 2 * ratio) * amp; 
		
		
		
		//newPos.y  += sin(newTime * PI * 2 * ratio) * amp; //sin(newTime * PI * 2); 정확히 한 주기
		//sin안쪽은 주기를 조절, 바깥쪽은 폭을 조절

		//x,y ---------|------------------------->vx,vy
		//             x = x0 + vx * time
		//			   y축에 가중치를 주어야 함
	}
	else
	{
		newPos = vec3(10000,10000,10000);
	}

	gl_Position = vec4(newPos.xyz , 1);
}

