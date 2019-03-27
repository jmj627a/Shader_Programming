#version 450

in vec3 a_Position;
in vec3 a_Vel;

uniform float u_Time; //1.f

void main()
{
	vec3 newPosition = a_Position.xyz;

	float newTime = fract(u_Time); //0���� 1���̸� �ݺ�

	newPosition += a_Vel * newTime ;

	gl_Position = vec4(newPosition , 1);
}
