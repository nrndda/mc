#include "stdafx.h"
#include <stdio.h>
#include <conio.h>

int main()
{
	int Data=0;
	while(Data!=-1)
	{
		printf(">");
		scanf("%i",&Data);
		if(Data>=0 &&  Data<=255)
		{
			_outp(0x378,Data);
		}
	}

	

	return 0;
}
