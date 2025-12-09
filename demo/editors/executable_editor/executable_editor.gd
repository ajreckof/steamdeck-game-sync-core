extends OptionButton

const name_property : StringName = &"exe"


var steam_shortcut : SteamShortcut :
	set(value) :
		steam_shortcut = value
		steam_shortcut.property_changed.connect(_on_shortcut_property_changed)
		generate_options()


func _on_shortcut_property_changed(property : StringName):
	if property != name_property:
		if String(property) == String(name_property):
			print("what the fuck ca n'aurait jamais du arriver")
	else :
		generate_options()

func generate_options() :
	if steam_shortcut.exe_alternatives.is_empty():
		SteamDeckGameSyncCore.find_executables_for_game(steam_shortcut)
	var exe_alts = steam_shortcut.exe_alternatives
	for alt in exe_alts :
		var exe : String = alt.exe
		var size : int = alt.size
		var option : String = exe.get_file() + "(%d)" % size
		add_item(option)



func _generate_readable_size(bytes: int) -> String:
	var byte_quantities = [2^10, 2^20, 2^30]
	if (bytes > byte_quantities[2]):
		return str(snapped(bytes / byte_quantities[2], 0.1)) + " GB"
	elif (bytes > byte_quantities[1]):
		return str(snapped(bytes / byte_quantities[1], 0.01)) + " MB"
	elif (bytes > byte_quantities[0]):
		return str(snapped(bytes / byte_quantities[0], 0.01)) + " KB"
	else:
		return str(bytes) + " B"
