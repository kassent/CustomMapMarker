Scriptname _CMM_MapMarkerEffectsScript extends ActiveMagicEffect 

Sound Property MAGIllusionNightEyeOn Auto
_CMM_MainQuestScript property ControlMapMarker auto

Event OnEffectStart(Actor akTarget, Actor akCaster)

	MAGIllusionNightEyeOn.Play(Game.GetPlayer())
	ControlMapMarker.DetectEnvironment()
  
EndEvent

