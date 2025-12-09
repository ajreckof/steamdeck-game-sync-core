extends ConfirmationDialog

@onready var line_edit: LineEdit = $ScrollContainer/VBoxContainer/HBoxContainer/LineEdit
const user_settings_file = "user://settings.cfg"

func _ready() -> void:
	load_settings_from_file()

func _on_line_edit_text_submitted() -> void:
	SteamDeckGameSyncCore.set_steamgriddb_api_key(line_edit.text)


func _on_about_to_popup() -> void:
	line_edit.text = SteamDeckGameSyncCore.get_steamgriddb_api_key()


func load_settings_from_file():
	print("loaded settings from files")
	var config_file = ConfigFile.new()
	var err = config_file.load(user_settings_file)
	if (err != OK) :
		return 
	SteamDeckGameSyncCore.set_steamgriddb_api_key(config_file.get_value("","steamgriddb_api_key"))

func save_settings_to_file():
	var config_file = ConfigFile.new()
	config_file.set_value("", "steamgriddb_api_key", SteamDeckGameSyncCore.get_steamgriddb_api_key())
	var err = config_file.save(user_settings_file)
	if err != OK :
		push_error("There was a probleme while saving settings. quitting will result in settings lost.")


func _on_confirmed() -> void:
	save_settings_to_file()

func _on_canceled() -> void:
	load_settings_from_file()
