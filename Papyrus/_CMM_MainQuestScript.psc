Scriptname _CMM_MainQuestScript extends Quest

import MapMarkerUtil

Spell Property _CMM_MapMarkerSpell Auto
Message Property _CMM_InstallMessage Auto
Message Property _CMM_RemoveMapMarker Auto
Message Property _CMM_CreateMapMarker Auto
Static Property _CMM_BaseMapMarker Auto

Event OnInit()
	RegisterForSingleUpdate(2.0)
EndEvent

Event OnUpdate()
	If (Game.GetPlayer().AddSpell(_CMM_MapMarkerSpell))
		_CMM_InstallMessage.ShowAsHelpMessage("_CMM_InstallMessage", 10, 10, 1)
	EndIf
EndEvent

Auto State readyState
	Function DetectEnvironment()
		GotoState("lockState")
		ObjectReference marker = Game.FindClosestReferenceOfTypeFromRef(_CMM_BaseMapMarker, Game.GetPlayer(), 1000.0)
		If (marker && marker.IsEnabled())
			If MapMarkerUtil.IsCustomMapMarker(marker)
				MapMarkerUtil.RemoveMapMarker(marker)
				marker.Disable()
				marker.Delete()
				_CMM_RemoveMapMarker.Show()
			EndIf
		ElseIf (!Game.GetPlayer().IsInInterior()) && (Game.GetPlayer().GetWorldSpace() == MapMarkerUtil.GetRootWorldSpace(Game.GetPlayer()))
			String markerName = ((Self as Form) as _CMM_UILIBScript).ShowTextInput("Set map marker's name:", "Custom Marker")
			Utility.Wait(0.5)
			Int markerIcon = (((Self as Form) as _CMM_UILIBScript).ShowTextInput("Set map marker's icon:", "25")) as Int
			marker = Game.GetPlayer().PlaceAtMe(_CMM_BaseMapMarker, 1, true, false)
			MapMarkerUtil.CreateMapMarker(marker, markerName, markerIcon)
			_CMM_CreateMapMarker.Show()
		EndIf
		marker = None
		GotoState("readyState")
	EndFunction
EndState
  
Function DetectEnvironment()

EndFunction

State lockState
	
EndState