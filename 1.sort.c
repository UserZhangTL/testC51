#include <stdio.h>
//≤Â»Î≈≈–Ú 

void func()
{
    
}


int main(void)
{
	int num[10] = {10,13,2,42,35,4,43,64,42,65};
	int i = 0;
	int t_i = 0;
	int temp = 0;
	
	for(i = 1;i < 10;i ++)
	{
	    t_i = i;
		do
		{
		    if(num[t_i - 1] > num[t_i])
		    {
			    temp = num[t_i - 1];
				num[t_i - 1] = num[t_i];
				num[t_i] = temp;	
		    }
			t_i --;	
		}while((t_i >= 1));
	} 
	
	for(i = 0;i < 10;i ++)
	{
	    printf("%d  ",num[i]);
	} 
	
	return 0;
} 
