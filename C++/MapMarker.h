#pragma once
#include "skse/GameFormComponents.h"
#include "skse/GameBSExtraData.h"
#include "BSFixedStringEx.h"

#define MARKER_FORMID 0x00000010

namespace MapMarkerUtil
{

	class ExtraMapMarker : public BSExtraData
	{
	public:
		virtual ~ExtraMapMarker();

		class Data : public TESFullName
		{
		public:

			enum
			{
				kFlag2_None = 0,
				kFlag2_Visible = 1 << 0,
				kFlag2_CantFastTravelTo = 1 << 1,
				kFlag2_HiddenWhenShowAll = 1 << 2,
			};

			UInt8	flags1;
			UInt8	flags2;
			UInt16	iconType;

			void Enable(bool abEnabled) {
				if (abEnabled)
					flags1 |= 0x02;
				else
					flags1 &= 0xFD;
			}
			MEMBER_FN_PREFIX(Data);
			DEFINE_MEMBER_FN(ctor, Data*, 0x00429050);
		};

		static ExtraMapMarker* Create();

		Data*			data;

	};

	typedef ExtraMapMarker::Data MarkerData;
}














//BSFixedStringEx 004660d0
/*

typedef void(__fastcall * _UpdateWorldSpace)(PlayerCharacter* player, void* unk);//PlayerCharacter::UpdateWorldSpace()
extern _UpdateWorldSpace UpdateWorldSpace;

typedef void(__fastcall * _PushBackToArray)(void* obj, void* unk, void* param);//tArray::push_back(void*);
extern _PushBackToArray PushBackToArray;



/*
const UInt32 g_TlsIndexPtrPtr = 0x01BBEB54;
const UInt32 UpdateMapMarker_Ret = 0x004C80E8;
__declspec(naked) void HookedUpdateMapMarker()
{
__asm
{
mov eax, [g_TlsIndexPtrPtr]
mov eax, [eax]
pushad
mov eax, [esp + 0x3C]
push eax
push ecx
call CheckObject
popad
jmp	[UpdateMapMarker_Ret]
}

}

_UpdateWorldSpace UpdateWorldSpace = (_UpdateWorldSpace)0x00735D30;

_PushBackToArray PushBackToArray = (_PushBackToArray)0x0043C670;
*/
