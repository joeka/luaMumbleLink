#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "lua.h"
#include "lauxlib.h"

#define PROJECT_TABLENAME "mumblelink"

/*
 * 	A lua module for Mumble Link, positional audio plugin for Mumble
 * 	Source of the C code: http://mumble.sourceforge.net/Link
 */

struct LinkedMem {
#ifdef WIN32
	UINT32	uiVersion;
	DWORD	uiTick;
#else
	uint32_t uiVersion;
	uint32_t uiTick;
#endif
	float	fAvatarPosition[3];
	float	fAvatarFront[3];
	float	fAvatarTop[3];
	wchar_t	name[256];
	float	fCameraPosition[3];
	float	fCameraFront[3];
	float	fCameraTop[3];
	wchar_t	identity[256];
#ifdef WIN32
	UINT32	context_len;
#else
	uint32_t context_len;
#endif
	unsigned char context[256];
	wchar_t description[2048];
};

typedef struct LinkedMem LinkedMem;

LUALIB_API int luaopen_mumblelink( lua_State *L );
static int initMumble(  );

// updateMumble( string identity, string context, float posx, float posy, float camx, float camy )
static int updateMumble( lua_State *L );

