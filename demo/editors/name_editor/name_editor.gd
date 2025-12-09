extends HBoxContainer

const name_property : StringName = &"appname"
@onready var label: Label = $Label
@onready var line_edit: SuggestionLineEdit = $AppNameEdit

var editing_app_name : bool :
	set(value):
		editing_app_name = value
		if not is_node_ready() :
			return
		label.visible = not editing_app_name
		line_edit.visible = editing_app_name
		label.text = steam_shortcut.appname
		line_edit.text = steam_shortcut.appname

var steam_shortcut : SteamShortcut :
	set(value) :
		steam_shortcut = value
		steam_shortcut.property_changed.connect(_on_shortcut_property_changed)
		editing_app_name = false

func _ready() -> void:
	editing_app_name = editing_app_name

func _on_shortcut_property_changed(property : StringName):
	if property != name_property:
		if String(property) == String(name_property):
			print("what the fuck ca n'aurait jamais du arriver")
	editing_app_name = false


func _on_app_name_edit_text_submitted(new_text: String) -> void:
	steam_shortcut.appname = new_text
	
	for game in line_edit.possible_games :
		if game.name == new_text :
			steam_shortcut.steamgriddb_id = game.id
			return
	push_error("Could not find the steamgriddb_id")


func _on_edit_button_pressed() -> void:
	editing_app_name = !editing_app_name
