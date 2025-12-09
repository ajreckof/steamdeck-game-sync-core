extends HBoxContainer

const  name_property : StringName = &"shortcut_path"
@onready var file_dialog: FileDialog = $FileDialog
@onready var line_edit: LineEdit = $LineEdit


var steam_shortcut : SteamShortcut :
	set(value) :
		steam_shortcut = value
		steam_shortcut.property_changed.connect(_on_shortcut_property_changed)
		if is_node_ready() :
			line_edit.text = str(steam_shortcut.shortcut_path)


func _on_shortcut_property_changed(property : StringName):
	if property != name_property:
		if String(property) == String(name_property):
			push_error("what the fuck ca n'aurait jamais du arriver")
	if is_node_ready() :
		line_edit.text = str(steam_shortcut.shortcut_path)

func _ready() -> void:
	line_edit.text = str(steam_shortcut.shortcut_path)


func _on_button_pressed() -> void:
	var current_dir = steam_shortcut.shortcut_path
	while not DirAccess.dir_exists_absolute(current_dir):
		current_dir = current_dir.rsplit("/",true,1)[0]
	file_dialog.current_dir = current_dir
	file_dialog.popup_centered_ratio()
	return 
	


func _on_file_dialog_dir_selected(dir: String) -> void:
	steam_shortcut.shortcut_path =  dir
	line_edit.text = dir


func _on_line_edit_text_submitted(new_text: String) -> void:
	steam_shortcut.shortcut_path =  new_text


func _on_line_edit_focus_exited() -> void:
	steam_shortcut.shortcut_path =  line_edit.text
