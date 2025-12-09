extends Label

const name_property : StringName = &"appid"


var steam_shortcut : SteamShortcut :
	set(value) :
		steam_shortcut = value
		steam_shortcut.property_changed.connect(_on_shortcut_property_changed)
		text = str(steam_shortcut.appid)


func _on_shortcut_property_changed(property : StringName):
	if property != name_property:
		if String(property) == String(name_property):
			print("what the fuck ca n'aurait jamais du arriver")
	else :
		text = str(steam_shortcut.appid)
