#include <strsafe.h>

static wchar_t buffer[32];

static float GetPrivateProfileFloatW(LPCWSTR lpAppName, LPCWSTR lpKeyName, FLOAT fDefault, LPCWSTR lpFileName) {
	_snwprintf(buffer, _countof(buffer), L"%g", fDefault);
	DWORD strlen = GetPrivateProfileStringW(lpAppName, lpKeyName, buffer, buffer, _countof(buffer), lpFileName);
	wchar_t* end;
	float value = wcstof(buffer, &end);
	while (end[0] != '\0') {
		if (!iswspace(end[0])) {
			return fDefault;
		}
		end++;
	}
	return value;
}

static bool GetPrivateProfileBoolW(LPCWSTR lpAppName, LPCWSTR lpKeyName, BOOLEAN bDefault, LPCWSTR lpFileName) {
	DWORD strlen = GetPrivateProfileStringW(lpAppName, lpKeyName, bDefault ? L"True" : L"False", buffer, _countof(buffer), lpFileName);
	return (CompareStringW(LOCALE_INVARIANT, NORM_IGNORECASE, buffer, strlen, L"True", 4) == CSTR_EQUAL);
}

#define WritePrivateProfileBoolW(lpAppName, lpKeyName, bValue, lpFileName) WritePrivateProfileStringW((lpAppName), (lpKeyName), (bValue) ? L"True" : L"False", (lpFileName))

#define GetSettingInt(keyName) do { SettingManager::keyName = GetPrivateProfileIntW(L"NewVegasRTXLight", L#keyName, SettingManager::keyName, iniPath); } while (0)
#define GetSettingFloat(keyName) do { SettingManager::keyName = GetPrivateProfileFloatW(L"NewVegasRTXLight", L#keyName, SettingManager::keyName, iniPath); } while (0)
#define GetSettingBool(keyName) do { SettingManager::keyName = GetPrivateProfileBoolW(L"NewVegasRTXLight", L#keyName, SettingManager::keyName, iniPath); } while (0)

#define WriteSettingInt(keyName) do { \
	_snwprintf(buffer, _countof(buffer), L"%u", SettingManager::keyName); \
	WritePrivateProfileStringW(L"NewVegasRTXLight", L#keyName, buffer, iniPath); \
} while (0)
#define WriteSettingFloat(keyName) do { \
	_snwprintf(buffer, _countof(buffer), L"%g", SettingManager::keyName); \
	WritePrivateProfileStringW(L"NewVegasRTXLight", L#keyName, buffer, iniPath); \
} while (0)
#define WriteSettingBool(keyName) do { \
	WritePrivateProfileBoolW(L"NewVegasRTXLight", L#keyName, SettingManager::keyName, iniPath); \
} while (0)

void SettingManager::Initialize() {
	Logger::Log("Starting the settings manager...");
	TheSettingManager = new SettingManager();

	TheSettingManager->HaveINIPath = false;

	HMODULE hDllHandle;
	GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCWSTR)&GetPrivateProfileBoolW, &hDllHandle);
	GetModuleFileNameW(hDllHandle, TheSettingManager->iniPath, _countof(iniPath));
	wchar_t* slash = wcsrchr(TheSettingManager->iniPath, L'\\');
	if (slash) {
		slash[0] = 0;
		if (SUCCEEDED(StringCchCatW(TheSettingManager->iniPath, _countof(iniPath), L"\\NewVegasRTXLight.ini"))) {
			TheSettingManager->HaveINIPath = true;
		}
		else {
			Logger::Log("Failed to calculate ini path, path is too long");
		}
	}
	else {
		Logger::Log("Failed to calculate ini path, odd path?");
	}
}

void SettingManager::LoadSettings() {
	if (!TheSettingManager->HaveINIPath) {
		return;
	}

	Logger::Log("Reading settings from %ls", iniPath);
	GetSettingBool(PassLights);
	GetSettingInt(LightRangeMode);
	GetSettingFloat(LightIntensity);
	GetSettingFloat(MoonIntensity);
	GetSettingBool(DisableCulling);
	GetSettingBool(SunLight);
	GetSettingBool(VisualSun);
	GetSettingBool(SkipRenderPass);
	GetSettingBool(UseFixedFunction);

	// Validate settings
	if (SettingManager::LightRangeMode < 0 || SettingManager::LightRangeMode > 2) {
		Logger::Log("Invalid light mode %u, resetting to 1", SettingManager::LightRangeMode);
		SettingManager::LightRangeMode = 1;
	}
}

/*
* Saves the state of the config to the config file
*/
void SettingManager::SaveSettings() {
	if (!TheSettingManager->HaveINIPath) {
		return;
	}

	Logger::Log("Saving settings to %ls", iniPath);
	WriteSettingBool(PassLights);
	WriteSettingInt(LightRangeMode);
	WriteSettingFloat(LightIntensity);
	WriteSettingFloat(MoonIntensity);
	WriteSettingBool(DisableCulling);
	WriteSettingBool(SunLight);
	WriteSettingBool(VisualSun);
	WriteSettingBool(SkipRenderPass);
	WriteSettingBool(UseFixedFunction);
}

bool SettingManager::PassLights = true;
int SettingManager::LightRangeMode = 0;
float SettingManager::LightIntensity = 1.0f;
float SettingManager::MoonIntensity = 0.25f;
bool SettingManager::DisableCulling = true;
bool SettingManager::SunLight = true;
bool SettingManager::VisualSun = true;
bool SettingManager::SkipRenderPass = true;
bool SettingManager::UseFixedFunction = true;
