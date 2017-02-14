#pragma once
#include "skse/Serialization.h"
#include "MapMarker.h"
#include <map>
#include <string>
using std::string;
using std::map;
class TESObjectREFR;

namespace MapMarkerUtil
{

	struct MapMarkerData
	{
		MapMarkerData(string& displayName, UInt16 iconType) : name(displayName), icon(iconType){}
		string	 name;
		UInt16	 icon;
	};

	class MapMarkerContainer : public map <TESObjectREFR*, MapMarkerData>
	{
	public:
		enum
		{
			kSerializationVersion = 1
		};
		void Save(SKSESerializationInterface * intfc, UInt32 kVersion);
		bool Load(SKSESerializationInterface * intfc, UInt32 kVersion);
		void Revert();
		bool Register(TESObjectREFR* object, string& name, UInt32 iconType);
		bool Unregister(TESObjectREFR* object);
		static UInt64 GetHandleByObject(void * src, UInt32 typeID);
		static void* GetObjectByHandle(UInt64 handle, UInt32 typeID);
		static MapMarkerContainer* GetSingleton();
	};
}
