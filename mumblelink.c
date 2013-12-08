#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <wchar.h>

#include "lua.h"
#include "lauxlib.h"

#include "mumblelink.h"

/*
 * 	A lua module for Mumble Link, positional audio plugin for Mumble
 * 	Source of the C code: http://mumble.sourceforge.net/Link
 */

LinkedMem *lm = NULL;

static int initMumble( ) {

#ifdef WIN32
	HANDLE hMapObject = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, L"MumbleLink");
	if (hMapObject == NULL)
		return 0;

	lm = (LinkedMem *) MapViewOfFile(hMapObject, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(LinkedMem));
	if (lm == NULL) {
		CloseHandle(hMapObject);
		hMapObject = NULL;
		return 0;
	}
#else
	char memname[256];
	snprintf(memname, 256, "/MumbleLink.%d", getuid());

	int shmfd = shm_open(memname, O_RDWR, S_IRUSR | S_IWUSR);

	if (shmfd < 0) {
		return 0;
	}

	lm = (LinkedMem *)(mmap(NULL, sizeof(struct LinkedMem), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd,0));

	if (lm == (void *)(-1)) {
		lm = NULL;
		return 0;

	}
#endif
	return 0;
}

static int updateMumble( lua_State *L ) {
	// updateMumble( string identity, string context, float posx, float posy, float camx, float camy )
	
	char *cidentity = NULL;
	cidentity = (char *)lua_tostring(L, 1);
	wchar_t  identity[256];
	swprintf(identity, 256, L"%hs", cidentity); // copy to wchar_t in a save way (I hope)

	char *context = NULL;
	context = (char *)lua_tostring(L, 2);

	float posx = (float) lua_tonumber(L, 3);
	float posy = (float) lua_tonumber(L, 4);	
	float camx = (float) lua_tonumber(L, 5);
	float camy = (float) lua_tonumber(L, 6);

	if (! lm)
		return 0;

	if(lm->uiVersion != 2) {
		wcsncpy(lm->name, L"Starbound", 256);
		wcsncpy(lm->description, L"Starbound MumbleLink tech", 2048);
		lm->uiVersion = 2;
	}
	lm->uiTick++;

	// Left handed coordinate system.
	// X positive towards "right".
	// Y positive towards "up".
	// Z positive towards "front".
	//
	// 1 unit = 1 meter

	// Unit vector pointing out of the avatars eyes (here Front looks into scene).
	lm->fAvatarFront[0] = 0.0f;
	lm->fAvatarFront[1] = 0.0f;
	lm->fAvatarFront[2] = 1.0f;

	// Unit vector pointing out of the top of the avatars head (here Top looks straight up).
	lm->fAvatarTop[0] = 0.0f;
	lm->fAvatarTop[1] = 1.0f;
	lm->fAvatarTop[2] = 0.0f;

	// Position of the avatar (here standing slightly off the origin)
	lm->fAvatarPosition[0] = posx;
	lm->fAvatarPosition[1] = posy;
	lm->fAvatarPosition[2] = 0.0f;

	// Same as avatar but for the camera.
	lm->fCameraPosition[0] = camx;
	lm->fCameraPosition[1] = camy;
	lm->fCameraPosition[2] = 0.0f;

	lm->fCameraFront[0] = 0.0f;
	lm->fCameraFront[1] = 0.0f;
	lm->fCameraFront[2] = 1.0f;

	lm->fCameraTop[0] = 0.0f;
	lm->fCameraTop[1] = 1.0f;
	lm->fCameraTop[2] = 0.0f;

	// Identifier which uniquely identifies a certain player in a context (e.g. the ingame Name).
	wcsncpy(lm->identity, identity, 256);
	// Context should be equal for players which should be able to hear each other positional and
	// differ for those who shouldn't (e.g. it could contain the server+port and team)
	memcpy(lm->context, context, 16);
	lm->context_len = 16;

	return 0;
}

LUALIB_API int luaopen_mumblelink( lua_State *L ) {
	lua_register(L, "initMumble", initMumble);
	lua_register(L, "updateMumble", updateMumble);
	return 0;
}

