#include <windows.h>

#pragma pack(push)
#pragma pack(1)

// 种地的数据包
// 只需要截取一次种地数据包，之后
typedef struct _ZD
{
	WORD code;
	WORD length;
	WORD syn;
	DWORD param1;
	DWORD param2;
	DWORD param3[2];
} ZD, *PZD;

ZD g_zd;

// 收地的数据包
typedef struct _SD
{
	WORD code;
	WORD length;
	WORD syn;
	DWORD reserved0[3];
	DWORD id;
	BYTE unused[10];
}SD, *PSD;

// 放技能的数据包，大小是23 00
#define SKILL_CODE 0xcd
// 取消技能的数据包，大小是08 00
#define UN_SKILL_CODE 0xc1
typedef struct _SKILL
{
	WORD code;
	WORD length;
	WORD syn;
	DWORD reserved1;
	DWORD type;
	DWORD param0;
	DWORD param1;
	DWORD param2;
	DWORD param3;
	DWORD reserved2;
	DWORD reserved3;	
	WORD reserved4;
	BYTE reserved5;
} SKILL, * PSKILL;

// 选怪的数据包，大小是08 00，最后8个字节是两个ID
#define SELECT_CODE 0x15
typedef struct _SELECT
{
	WORD code;
	WORD length;
	WORD syn;
	DWORD param1;		
	DWORD reserved;
} SELECT, * PSELECT;

#pragma pack(pop)