#pragma once
#include "skse/PapyrusNativeFunctions.h"
#include "BSFixedStringEx.h"

class TESObjectREFR;
class TESWorldSpace;

namespace MapMarkerUtil
{
	BSFixedString GetMapMarkerName(StaticFunctionTag* base, TESObjectREFR* object);

	UInt32 GetMapMarkerIcon(StaticFunctionTag* base, TESObjectREFR* object);

	bool CreateMapMarker(StaticFunctionTag* base, TESObjectREFR* object, BSFixedStringEx name, UInt32 iconType);

	bool RemoveMapMarker(StaticFunctionTag* base, TESObjectREFR* object);

	bool IsCustomMapMarker(StaticFunctionTag* base, TESObjectREFR* object);

	TESWorldSpace* GetRootWorldSpace(StaticFunctionTag* base, TESObjectREFR* object);

	bool RegisterFuncs(VMClassRegistry* registry);

};
