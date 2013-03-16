#include <windows.h>

#pragma pack(push)
#pragma pack(1)

// 放技能的数据包，大小是23 00
#define SKILL_CODE 0x1E
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
#define SELECT_CODE 0x7E
typedef struct _SELECT
{
	WORD code;
	WORD length;
	WORD syn;
	DWORD param1;		
	DWORD reserved;
} SELECT, * PSELECT;

#pragma pack(pop)