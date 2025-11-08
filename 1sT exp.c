#include<reg51.h>
void main(void)
{
	unsigned char source [10]={0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x20};
	unsigned char destination[10];
	unsigned char i ;
	
	for( i = 0;i<10;i++)
	{
		destination[i]=source[i];
	}
	while(1);
}