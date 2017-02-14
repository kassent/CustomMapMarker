#include "MapMarker.h"
#include "MapMarkerUtility.h"
#include "MapMarkerContainer.h"
#include "skse\GameReferences.h"
namespace MapMarkerUtil
{
	BSFixedString GetMapMarkerName(StaticFunctionTag* base, TESObjectREFR* object)
	{
		if (object)
		{
			ExtraMapMarker* marker = static_cast<ExtraMapMarker*>(object->extraData.GetByType(kExtraData_MapMarker));
			if (marker)
			{
				MarkerData* data = marker->data;
				if (data)
					return data->name;
			}
		}
		return NULL;
	}

	UInt32 GetMapMarkerIcon(StaticFunctionTag* base, TESObjectREFR* object)
	{
		if (object)
		{
			ExtraMapMarker* marker = static_cast<ExtraMapMarker*>(object->extraData.GetByType(kExtraData_MapMarker));
			if (marker)
			{
				MarkerData* data = marker->data;
				if (data)
					return data->iconType;
			}
		}
		return NULL;
	}

	bool CreateMapMarker(StaticFunctionTag* base, TESObjectREFR* object, BSFixedString name, UInt32 iconType)
	{
		string temp(name.data);
		return MapMarkerContainer::GetSingleton()->Register(object, temp, iconType);
	}

	bool RemoveMapMarker(StaticFunctionTag* base, TESObjectREFR* object)
	{
		return MapMarkerContainer::GetSingleton()->Unregister(object);
	}

	bool IsCustomMapMarker(StaticFunctionTag* base, TESObjectREFR* object)
	{
		if (object)
		{
			const UInt32 mapMakerFormID = MARKER_FORMID;
			TESForm* baseForm = object->baseForm;
			if (baseForm->formID == mapMakerFormID)
			{
				UInt32 objFormID = object->formID;
				objFormID >>= 24;
				if (objFormID == 0xFF)
					return true;
			}
		}
		return false;
	}

	TESWorldSpace* GetRootWorldSpace(StaticFunctionTag* base, TESObjectREFR* object)
	{
		TESWorldSpace* result = nullptr;
		if (object)
		{
			result = CALL_MEMBER_FN(object, GetWorldspace)();
		}
		return result;
	}

	bool RegisterFuncs(VMClassRegistry* registry) {

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, BSFixedString, TESObjectREFR*>("GetMapMarkerName", "MapMarkerUtil", MapMarkerUtil::GetMapMarkerName, registry));
		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, UInt32, TESObjectREFR*>("GetMapMarkerIcon", "MapMarkerUtil", MapMarkerUtil::GetMapMarkerIcon, registry));
		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, bool, TESObjectREFR*>("RemoveMapMarker", "MapMarkerUtil", MapMarkerUtil::RemoveMapMarker, registry));
		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, bool, TESObjectREFR*>("IsCustomMapMarker", "MapMarkerUtil", MapMarkerUtil::IsCustomMapMarker, registry));
		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, TESWorldSpace*, TESObjectREFR*>("GetRootWorldSpace", "MapMarkerUtil", MapMarkerUtil::GetRootWorldSpace, registry));
		registry->RegisterFunction(
			new NativeFunction3 <StaticFunctionTag, bool, TESObjectREFR*, BSFixedString, UInt32>("CreateMapMarker", "MapMarkerUtil", MapMarkerUtil::CreateMapMarker, registry));
		return true;
	}

};
