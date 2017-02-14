ScriptName MapMarkerUtil Hidden

String Function GetMapMarkerName(ObjectReference akRef) native global 

Int Function GetMapMarkerIcon(ObjectReference akRef) native global 

Bool Function CreateMapMarker(ObjectReference akRef, String name = "", Int iconType = 0) native global 

Bool Function RemoveMapMarker(ObjectReference akRef) native global

Bool Function IsCustomMapMarker(ObjectReference akRef) native global

WorldSpace Function GetRootWorldSpace(ObjectReference akRef) native global
