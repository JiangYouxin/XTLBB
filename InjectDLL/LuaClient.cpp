#include "stdafx.h"
#include "LuaClient.h"

#include "../lua/lua.h"
#include "../lua/lualib.h"
#include "../lua/lauxlib.h"

namespace 
{
	BYTE selectCode = 0x7E;
	BYTE skillCode = 0x45;
}

namespace LuaClient 
{
	/*
	BOOL Init(const char *fname)
	{
		lua_State *L = luaL_newstate();
		if (luaL_loadfile(L, fname) || lua_pcall(L, 0, 0, 0))
			return FALSE;
		lua_getglobal(L, "selectCode");
		lua_getglobal(L, "skillCode");
		if (!lua_isnumber(L, -2)) 
			return FALSE;
		if (!lua_isnumber(L, -1))
			return FALSE;
		selectCode = lua_tointeger(L, -2);
		skillCode = lua_tointeger(L, -1);
		lua_close(L);
		return TRUE;
	}
	*/
	BYTE GetSelectCode()
	{
		return selectCode;
	}
	BYTE GetSkillCode()
	{
		return skillCode;
	}
}
