#define NULL 0 
#include "serial_api.h"

extern serial_t stdio_uart;

int strlen(char *str) 
{  
	int i;  
	if(str==NULL)  
		return -1;  
	for(i = 0;str[i]!='\0';i++);  
	return i;  
}  



void str_printf(char *msg)  
{  
	int x = strlen(msg);  
	int i = 0;  
	for(;i<x;i++)  
		serial_putc(&stdio_uart,msg[i]);  
	if(msg[x-1]=='\n')  
		serial_putc(&stdio_uart,'\r');
}  

char *itoa(int value, char *string, int radix)  
{  
	char tmp[33];  
	char *tp = tmp;  
	int i;  
	unsigned v;  
	int sign;  
	char *sp;  

	if (radix > 36 || radix <= 1)  
	{  
		return 0;  
	}  

	sign = (radix == 10 && value < 0);  
	if (sign)  
		v = -value;  
	else  
		v = (unsigned)value;  
	while (v || tp == tmp)  
	{  
		i = v % radix;  
		v = v / radix;  
		if (i < 10)  
			*tp++ = i+'0';  
		else  
			*tp++ = i + 'a' - 10;  
	}  

	sp = string;  

	if (sign)  
		*sp++ = '-';  
	while (tp > tmp)  
		*sp++ = *--tp;  
	*sp = 0;  
	return string;  
}  

#define HAVE_PRINTF 1  
#ifdef HAVE_PRINTF  
int hs_printf(const char *fmt,...)
{  
	char *str = fmt;   
	int x = strlen(str);  
	char* pArg = NULL;  
	char c;  

	pArg = (char*) &fmt;  
	pArg += sizeof(fmt);  

	char tmp[100];  

	for(;*fmt!='\0';fmt++)  
	{  
		c =*fmt;  
		if (c != '%')  
		{  
			serial_putc(&stdio_uart,*fmt);  
			if(*fmt=='\n')  
				serial_putc(&stdio_uart,'\r');  
		}  
		else  
		{  
			fmt++;  
			switch(*fmt)  
			{  
				case 'd':  
					itoa(*((int*)pArg),tmp,10);  
					str_printf(tmp);  
					break;  
				case 'i':  
					itoa(*((int*)pArg),tmp,10);  
					str_printf(tmp);  
					break;  
				case 'u':  
					if(*((int*)pArg)<0)  
					{  
						printf("printf format \%u use err\n");  
						return 0;  
					}  
					itoa(*((int*)pArg),tmp,10);  
					str_printf(tmp);  
					break;    
				case 'x':  
					itoa(*((int*)pArg),tmp,16);  
					str_printf(tmp);  
					break;  
				case 's':  
					str_printf(*((int*)pArg));  
					break;  
				case '0':  
					if((*(fmt+1)=='8')&&(*(fmt+2)=='x'))  
					{  
						itoa(*((int*)pArg),tmp,16);  
						if((strlen(tmp)<8)&&(strlen(tmp)>0))  
						{  
							int j = 8-strlen(tmp);  
							int i = 0;   
							for(;i<j;i++)  
								serial_putc(&stdio_uart,'0');  
							str_printf(tmp);  
						}  
						else  
							str_printf(tmp);  
						fmt=fmt+2;  
						break;  
					}  
					else if((*(fmt+1)=='2')&&(*(fmt+2)=='x'))  
					{  
						itoa(*((int*)pArg),tmp,16);  
						if((strlen(tmp)<2)&&(strlen(tmp)>0))//fixme  
						{  
							int j = 2-strlen(tmp);  
							int i = 0;   
							for(;i<j;i++)  
								serial_putc(&stdio_uart,'0');  
							str_printf(tmp);  
						}  
						else                                  
							str_printf(tmp);  
						fmt=fmt+2;  
						break;  
					}  
					else  
					{  
						str_printf("this \"printf\" only support %d%i%u%x%s%08x%02x\n");  
						break;    
					}  
				default:  
					str_printf("this \"printf\" only support %d%i%u%x%s%08x%02x\n");  
					break;  
			}  
			pArg += sizeof(int);//point to next arg  
		}  
	}
}  
#endif
