#ifndef _PRINT_H_
#define _PRINT_H_ (1)

#include <crtdbg.h>

extern void LCDPrint(int row,int col, const char*text);

class Print{

	public:

		int print(char ch)
		{
			char string[3];

			_RPT1(_CRT_WARN,"%c",ch);

			string[0]=ch;string[1]=0;
			LCDPrint(0,0,string);
			return 1;
		}

		int print(const char*str)
		{
			
			LCDPrint(0,0,str);

			return 0;
		}
};

#endif
