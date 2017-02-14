#include "skse/PluginAPI.h"
#include "skse/skse_version.h"
#include "skse/PluginManager.h"

#include "MapMarkerContainer.h"
#include "MapMarkerUtility.h"

#include <shlobj.h>	// CSIDL_MYCODUMENTS

using namespace MapMarkerUtil;

IDebugLog		g_Log;
const char*		kLogPath = "\\My Games\\Skyrim\\Logs\\MapMarkerUtil.log";

const char*		PLUGIN_NAME = "MapMarkerUtil";
const UInt32	VERSION_MAJOR = 1;
const UInt32	VERSION_MINOR = 0;
const UInt32	VERSION_PATCH = 1;

PluginHandle	g_pluginHandle = kPluginHandle_Invalid;

SKSEPapyrusInterface*			g_papyrus = NULL;
SKSESerializationInterface*		g_serialization = NULL;


void Serialization_Save(SKSESerializationInterface * intfc)
{
	_MESSAGE("Saving...");

	MapMarkerContainer::GetSingleton()->Save(intfc, MapMarkerContainer::kSerializationVersion);

	_MESSAGE("%s - Serialized string table", __FUNCTION__);
}

void Serialization_Load(SKSESerializationInterface * intfc)
{
	_MESSAGE("Loading...");

	UInt32 type, length, version;
	bool error = false;

	while (intfc->GetNextRecordInfo(&type, &version, &length))
	{
		switch (type)
		{
		case 'MMCT':
			MapMarkerContainer::GetSingleton()->Load(intfc, version);
			_MESSAGE("Loading map marker map...");
			break;
		default:
			_MESSAGE("unhandled type %08X (%.4s)", type, &type);
			error = true;
			break;
		}
	}
}

void Serialization_Revert(SKSESerializationInterface * intfc)
{
	_MESSAGE("Reverting...");
	MapMarkerContainer::GetSingleton()->Revert();
}

extern "C"
{

	bool SKSEPlugin_Query(const SKSEInterface * skse, PluginInfo * info)
	{
		g_Log.OpenRelative(CSIDL_MYDOCUMENTS, kLogPath);
		g_Log.SetPrintLevel(IDebugLog::kLevel_Error);

#ifdef _DEBUG
		g_Log.SetLogLevel(IDebugLog::kLevel_DebugMessage);
#else
		g_Log.SetLogLevel(IDebugLog::kLevel_Message);
#endif
		_MESSAGE("%s %i.%i.%i", PLUGIN_NAME, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);

		// populate info structure
		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = PLUGIN_NAME;
		info->version = VERSION_MAJOR;

		// store plugin handle so we can identify ourselves later
		g_pluginHandle = skse->GetPluginHandle();

		if (skse->isEditor)
		{
			_ERROR("loaded in editor, marking as incompatible");

			return false;
		}
		else if (skse->runtimeVersion != RUNTIME_VERSION_1_9_32_0)
		{
			_ERROR("unsupported runtime version %08X", skse->runtimeVersion);

			return false;
		}
		g_serialization = (SKSESerializationInterface *)skse->QueryInterface(kInterface_Serialization);
		if (!g_serialization)
		{
			_FATALERROR("couldn't get serialization interface");
			return false;
		}
		if (g_serialization->version < SKSESerializationInterface::kVersion)
		{
			_FATALERROR("serialization interface too old (%d expected %d)", g_serialization->version, SKSESerializationInterface::kVersion);
			return false;
		}
		return true;
	}


	bool SKSEPlugin_Load(const SKSEInterface * skse)
	{
		// register callback for SKSE messaging interface
		g_papyrus = (SKSEPapyrusInterface*)skse->QueryInterface(kInterface_Papyrus);
		// register custom papyrus functions
		g_papyrus->Register(MapMarkerUtil::RegisterFuncs);

		if (g_serialization) {
			g_serialization->SetUniqueID(g_pluginHandle, 'MMUT');
			g_serialization->SetRevertCallback(g_pluginHandle, Serialization_Revert);
			g_serialization->SetSaveCallback(g_pluginHandle, Serialization_Save);
			g_serialization->SetLoadCallback(g_pluginHandle, Serialization_Load);
			//g_serialization->SetFormDeleteCallback(g_pluginHandle, );
		}

		return true;
	}
};



















