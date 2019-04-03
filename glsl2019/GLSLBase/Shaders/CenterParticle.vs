#version 450

in vec3 a_Position;
in vec4 a_StartLife;
in vec3 a_Vel;
in float a_RandValue;

uniform float u_Time; 
const float PI= 3.141592;
const mat3 c_RP = mat3(0,-1,0,1,0,0,0,0,0);//openGL�� ��Ʈ������ ���η� ��
// c	s	0	90��		0	1	0
// -s	c	0	->		-1	0	0
// 0	0	0			0	0	0
const vec3 c_Gravity = vec3(0, -2, 0); //�߷°��ӵ�
void main()
{
	vec3 newPos = a_Position.xyz;
	vec3 newVel = a_Vel.xyz;
	float startTime = a_StartLife.x;
	float lifeTime = a_StartLife.y;
	float ratio = a_StartLife.z;
	float amp = a_StartLife.w;

	float newTime = u_Time - startTime; //�ݺ�x

	if(newTime > 0)
	{
		newPos.x += sin(a_RandValue * PI * 2);
		newPos.y += cos(a_RandValue * PI * 2);

		//amp = amp * newTime;
		
		newTime = mod(newTime, lifeTime);
		
		newVel = newVel + c_Gravity * newTime; //���� �߷� �ݿ��ؼ� ������ �ӵ�
		
		newPos = newPos + a_Vel * newTime + c_Gravity * 0.5 * newTime; //��ġ ���
		//���� a_Vel���� �ʱ�ӵ��� ����Ǿ�� �ϱ� ����
		
		
		vec3 vSin = a_Vel * c_RP;
		newPos = newPos + vSin * sin(newTime * PI * 2 * ratio) * amp; 
		
		
		
		//newPos.y  += sin(newTime * PI * 2 * ratio) * amp; //sin(newTime * PI * 2); ��Ȯ�� �� �ֱ�
		//sin������ �ֱ⸦ ����, �ٱ����� ���� ����

		//x,y ---------|------------------------->vx,vy
		//             x = x0 + vx * time
		//			   y�࿡ ����ġ�� �־�� ��
	}
	else
	{
		newPos = vec3(10000,10000,10000);
	}

	gl_Position = vec4(newPos.xyz , 1);
}

