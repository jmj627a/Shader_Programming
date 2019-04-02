#version 450

in vec3 a_Position;
in vec3 a_Vel;
in vec2 a_StartLife;

uniform float u_Time; //1.f
uniform bool u_Repeat = true; //1.f

const vec3 c_Gravity = vec3(0, -5, 0); //�߷°��ӵ�
void main()
{
	vec3 newPos = a_Position.xyz;
	float startTime = a_StartLife.x;
	float lifeTime = a_StartLife.y;

	//float newTime = fract(u_Time); //0���� 1���̸� �ݺ�
	float newTime = u_Time - startTime; //�ݺ�x

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

��ġ = �ʱ���ġ + �ʱ�ӵ� * �ð�
�ӵ� = �ʱ�ӵ� + ���ӵ� * �ð�

�ӵ��� ���� --> ���⼺ ũ��
���ӵ��� ���� --> ����ӵ�

��ġ = �ʱ���ġ + �ʱ�ӵ� * �ð� + 1/2*���ӵ�*�ð�^2


*/