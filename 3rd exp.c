#include<reg51.h>
void delay(unsigned int);

sbit sw  = P3^1;

void main (void)
{
	P2= 0X00; 
	while(1)
	{
		if (sw==0)
		{
			P2=0X03; 
			delay(100); 
			P2=0X06; 
			delay(100); 
			P2=0X0C; 
			delay(100); 
			P2=0X09; 
			delay(100);
		}
		else 
		{ 
			P2=0X09; 
			delay(100); 
			P2=0X0C; 
			delay(100); 
			P2=0X06; 
			delay(100); 
			P2=0X03; 
			delay(100);
		}
	}
}

void delay (unsigned int m)
{
	unsigned int i , j ;
	for(i = 0 ; i < m ; i++)
	{
		for(j = 0 ; j < 1275 ; j++);
	}
}
