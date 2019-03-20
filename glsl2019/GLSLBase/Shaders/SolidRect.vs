#version 450

in vec4 a_Position;
in vec4 a_Color;

uniform float u_Time; //1.f

void main()
{
	//Vec3 temp;
	//temp = a_Position.xyz * u_Time;

	float newX, newY;
	newX= sin(u_Time*3.141562*2.0);
	newY= cos(u_Time*3.141562*2.0);

	newX += a_Position.x;
	newY += a_Position.y;

	gl_Position = vec4(newX, newY, 0 , 1);
	//gl_Position = vec4(a_Position.xyz , 1);
}
