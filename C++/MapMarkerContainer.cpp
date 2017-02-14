#include "MapMarkerContainer.h"
#include "skse/GameReferences.h"
#include "skse/GameObjects.h"
#include "skse/PapyrusVM.h"
#include <memory>

const UInt32 kPlayerPointer = 0x01B2E8E4;
const UInt32 kGetWorldSpaceFunc = 0x004D5EB0;
const UInt32 kUnknownFunc = 0x00735CE0;
const UInt32 kUpdateMapMarkerFunc = 0x004F23E0;

__declspec(naked) void UpdateMapMarkers()
{
	__asm
	{
			pushad
			mov eax, [kPlayerPointer]
			mov esi, [eax]
			mov ecx, esi
			call [kGetWorldSpaceFunc]
			mov edi, eax
			mov ecx, esi
			call [kUnknownFunc]
			test edi, edi
			je ZeroProc
			mov	[esi + 0x39C], edi
			mov ecx, [esi + 0x39C]
			push 0x0
			lea eax, [esi + 0x2F4]
			push eax
			call [kUpdateMapMarkerFunc]
			popad
			retn
		ZeroProc :
			mov ecx, [esi + 0x39C]
			push 0x0
			lea eax, [esi + 0x2F4]
			push eax
			call [kUpdateMapMarkerFunc]
			popad
			retn
	}
}

namespace MapMarkerUtil
{

	MapMarkerContainer* MapMarkerContainer::GetSingleton()
	{
		static MapMarkerContainer instance;
		return &instance;
	}

	bool MapMarkerContainer::Register(TESObjectREFR* object, string& name, UInt32 iconType)
	{
		if (object)
		{
			TESForm* baseForm = object->baseForm;
			if (baseForm->formID == MARKER_FORMID)
			{
				const char* temp = name.c_str();
				BaseExtraList* extraData = &(object->extraData);
				ExtraMapMarker* extraMapMarker = static_cast<ExtraMapMarker*>(extraData->GetByType(kExtraData_MapMarker));
				if (extraMapMarker)
				{
					MarkerData* data = extraMapMarker->data;
					if (data)
					{
						if (strlen(temp))
							CALL_MEMBER_FN(&(data->name), Set)(temp);
						if (iconType > NULL)
							data->iconType = static_cast<UInt16>(iconType);
					}
				}
				else
				{
					ExtraMapMarker* marker = ExtraMapMarker::Create();
					MarkerData* data = marker->data;
					if (data)
					{
						CALL_MEMBER_FN(&(data->name), Set)(temp);
						if (iconType > NULL)
							data->iconType = static_cast<UInt16>(iconType);
					}
					extraData->Add(kExtraData_MapMarker, marker);
					UpdateMapMarkers();
				}
				MapMarkerData data(name, static_cast<UInt16>(iconType));
				auto it = this->find(object);
				if (it != this->end())
					it->second = data;
				else
					this->insert({ object, data });
				return true;
			}
		}
		return false;
	}

	bool MapMarkerContainer::Unregister(TESObjectREFR* object)
	{
		auto it = this->find(object);
		if (it != this->end())
		{
			ExtraMapMarker* marker = static_cast<ExtraMapMarker*>(object->extraData.GetByType(kExtraData_MapMarker));
			UInt32 objFormID = object->formID;
			objFormID >>= 24;
			if (objFormID == 0xFF && marker)
			{
				object->extraData.Remove(kExtraData_MapMarker, marker);
				CALL_MEMBER_FN(&(marker->data->name), Release)();
				FormHeap_Free(marker->data);
				FormHeap_Free(marker);
			}
			this->erase(object);
			return true;
		}
		return false;
	}

	void MapMarkerContainer::Save(SKSESerializationInterface * intfc, UInt32 kVersion)
	{
		intfc->OpenRecord('MMCT', kVersion);

		UInt32 totalRecords = this->size();
		intfc->WriteRecordData(&totalRecords, sizeof(totalRecords));

		for (auto& pair : *this)
		{
			UInt64 markerHandle = GetHandleByObject(pair.first, FormType::kFormType_Reference);
			_MESSAGE("Saving map marker record:%016X", markerHandle);
			const char* markerName = pair.second.name.c_str();
			UInt16 length = strlen(markerName);
			intfc->WriteRecordData(&markerHandle, sizeof(markerHandle));
			intfc->WriteRecordData(&length, sizeof(length));
			intfc->WriteRecordData(markerName, length);
			intfc->WriteRecordData(&(pair.second.icon), sizeof(pair.second.icon));
		}
	}

	bool MapMarkerContainer::Load(SKSESerializationInterface* intfc, UInt32 kVersion)
	{
		bool error = false;
		UInt32 totalRecord = 0;

		if (!intfc->ReadRecordData(&totalRecord, sizeof(totalRecord)))
		{
			_ERROR("%s - Error loading total records from table", __FUNCTION__);
			error = true;
			return error;
		}
		for (UInt32 i = 0; i < totalRecord; i++)
		{
			UInt64 markerHandle;
			if (!intfc->ReadRecordData(&markerHandle, sizeof(markerHandle)))
			{
				_ERROR("%s - Error loading map marker object ptr", __FUNCTION__);
				error = true;
				return error;
			}
			//GetObject(UInt64 handle, UInt32 typeID)
			TESObjectREFR* mapMarkerRef = (TESObjectREFR*)GetObjectByHandle(markerHandle, FormType::kFormType_Reference);
			if (!mapMarkerRef)
				continue;
			//char* name = nullptr;
			UInt16 length = 0;
			if (!intfc->ReadRecordData(&length, sizeof(length)))
			{
				_ERROR("%s - Error loading marker name length", __FUNCTION__);
				error = true;
				return error;
			}
			//name = new char[length + 1];
			std::unique_ptr<char[]> name(new char[length + 1]);
			if (length > 0 && !intfc->ReadRecordData(name.get(), length)) {
				_ERROR("%s - Error loading marker name string of length %d", __FUNCTION__, length);
				error = true;
				return error;
			}
			name[length] = '\0';
			string markerName(name.get());

			UInt16 icon;
			if (!intfc->ReadRecordData(&icon, sizeof(icon)))
			{
				_ERROR("%s - Error loading marker icon", __FUNCTION__);
				error = true;
				return error;
			}
			Register(mapMarkerRef, markerName, icon);
			_MESSAGE("Load map record:%016X", markerHandle);
		}
		return error;
	}

	void MapMarkerContainer::Revert()
	{
		for (auto& element : *this)
		{
			TESObjectREFR* ref = element.first;
			ExtraMapMarker* marker = static_cast<ExtraMapMarker*>(ref->extraData.GetByType(kExtraData_MapMarker));
			UInt32 objFormID = ref->formID;
			objFormID >>= 24;
			if (objFormID == 0xFF)
			{
				ref->extraData.Remove(kExtraData_MapMarker, marker);
				CALL_MEMBER_FN(&(marker->data->name), Release)();
				FormHeap_Free(marker->data);
				FormHeap_Free(marker);
			}
		}
		this->clear();
	}

	UInt64 MapMarkerContainer::GetHandleByObject(void * src, UInt32 typeID)
	{
		VMClassRegistry		* registry = (*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy = registry->GetHandlePolicy();

		return policy->Create(typeID, (void*)src);
	}

	void * MapMarkerContainer::GetObjectByHandle(UInt64 handle, UInt32 typeID)
	{
		VMClassRegistry		* registry = (*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy = registry->GetHandlePolicy();

		if (handle == policy->GetInvalidHandle()) {
			return nullptr;
		}
		return policy->Resolve(typeID, handle);
	}
}
