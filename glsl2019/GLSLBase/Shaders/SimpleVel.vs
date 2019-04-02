#version 450

in vec3 a_Position;
in vec3 a_Vel;

uniform float u_Time; //1.f

const vec3 c_Gravity = vec3(0, -5, 0); //�߷°��ӵ�
void main()
{
	vec3 newPosition = a_Position.xyz;

	//float newTime = fract(u_Time); //0���� 1���̸� �ݺ�
	float newTime = u_Time; //�ݺ�x

	newPosition += a_Vel * newTime + 0.5f * c_Gravity * newTime * newTime ;

	gl_Position = vec4(newPosition , 1);
}


/*

��ġ = �ʱ���ġ + �ʱ�ӵ� * �ð�
�ӵ� = �ʱ�ӵ� + ���ӵ� * �ð�

�ӵ��� ���� --> ���⼺ ũ��
���ӵ��� ���� --> ����ӵ�

��ġ = �ʱ���ġ + �ʱ�ӵ� * �ð� + 1/2*���ӵ�*�ð�^2


*/