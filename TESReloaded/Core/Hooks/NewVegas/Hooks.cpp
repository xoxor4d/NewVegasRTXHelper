#include "../lib/minhook/include/MinHook.h"
#include "Hooks.h"
#include "../GameCommon.h"
#include "Culling.h"
#include "Game.h"
#include "Lights.h"
#include "Render.h"

bool MH_CreateHookSimple(LPVOID *ppOriginal, LPVOID pDetour) {
	if (ppOriginal == nullptr) {
		Logger::Log("Attempted to create null hook for %p", pDetour);
	}
	void* pTarget = *ppOriginal;
	if (MH_CreateHook(pTarget, pDetour, ppOriginal) != MH_OK) {
		Logger::Log("Failed to create hook for %p", pTarget);
	}
	else if (MH_EnableHook(pTarget)) {
		Logger::Log("Failed to enable hook for %p", pTarget);
	}
	else {
		Logger::Debug("Created hook for %p to %p", pTarget, pDetour);
		return true;
	}
	return false;
}

void AttachHooks() {
	if (MH_Initialize() != MH_OK) {
		Logger::Log("Failed to initialize MinHook");
		return;
	}
	MH_CreateHookSimple((LPVOID*)&InitializeRenderer, InitializeRendererHook);
	MH_CreateHookSimple((LPVOID*)&NewTES, NewTESHook);
	MH_CreateHookSimple((LPVOID*)&NewMenuInterfaceManager, NewMenuInterfaceManagerHook);

	AttachRenderHooks();

	if (SettingManager::PassLights) {
		AttachLightHooks();
	}

	if (SettingManager::DisableCulling) {
		AttachCullingHooks();
	}
}
