#include<stdio.h>
int main(void)
{
	char i = 127;
	char a, b, c;

	printf("i  %d\n", i);
	a = i + 1;
	printf("i + 1  %d \n", a);
	b = i + 2;
	printf("i + 2  %d \n", b);
	c = i + 3;
	printf("i + 3  %d \n", c);

	i = -127;

	printf("i  %d\n", i);
	a = i - 1;
	printf("i - 1  %d \n", a);
	b = i - 2;
	printf("i - 2  %d \n", b);
	c = i - 3;
	printf("i - 3  %d \n", c);

	printf("i�� ���� �� �ִ� �ּڰ��� -128�̴�");

	getchar();

    return 0;

}