#include "skse/GameAPI.h"
#include "MapMarker.h"

namespace MapMarkerUtil
{
	ExtraMapMarker* ExtraMapMarker::Create()
	{
		MarkerData* markerData = (MarkerData*)FormHeap_Allocate(0xC);  //0C
		markerData = CALL_MEMBER_FN(markerData, ctor)();
		if (markerData)
		{
			markerData->flags1 = 0x03;
			markerData->flags2 = 0x03;
			markerData->iconType = 25;
			ExtraMapMarker* instance = (ExtraMapMarker*)FormHeap_Allocate(0xC);
			if (instance)
			{
				((UInt32*)instance)[0] = 0x01079D2C;
				instance->next = NULL;
				instance->data = markerData;
				return instance;
			}
		}
		return nullptr;
		//0040F960    A1 54EBBB01     mov eax,dword ptr ds:[0x1BBEB54]  BSExtraList::MapMarker(Data*);
	}
}
