extends Control

@onready var settings: ConfirmationDialog = $Settings
@onready var grid_container: GridContainer = $VBoxContainer/ScrollContainer/GridContainer

const CELL_TITLE = preload("uid://puhkmbpb5cqn")

const columns : Array[Dictionary] =[
	{
		title = "Game Id",
		scene = preload("uid://bun4h4nd8bx57"),
	},
	{
		title = "Game Name",
		scene = preload("uid://dxf4eg5fl6g08"),
	},
	{
		title = "Game Start Dir",
		scene = preload("uid://chrgdgq5b6mdv"),
	},
	{
		title = "Shortcut Path",
		scene = preload("uid://c2n3gq8riaevj"),
	},
	{
		title = "Game Executable",
		scene = preload("uid://g2rs7alws55g"),
	},
	{
		title = "Game Logo",
		scene = preload("uid://bauhm3f2i3klf"),
		width = 150,
		parameters = {
			image_type = SteamDeckGameSyncCore.LOGO
		},
	},
	{
		title = "Game Grid",
		scene = preload("uid://bauhm3f2i3klf"),
		width = 125,
		parameters = {
			image_type = SteamDeckGameSyncCore.GRID
		},
	},
	{
		title = "Game Large Grid",
		scene = preload("uid://bauhm3f2i3klf"),
		width = 300,
		parameters = {
			image_type = SteamDeckGameSyncCore.LARGE_GRID
		},
	},
	{
		title = "Game Hero",
		scene = preload("uid://bauhm3f2i3klf"),
		width = 400,
		parameters = {
			image_type = SteamDeckGameSyncCore.HERO
		},
	}
]


@onready var file_dialog: FileDialog = $FileDialog
const steam_user_path = "/Users/tbonhoure/Library/Application Support/Steam/userdata/399577801/config"
var shortcuts : Array[SteamShortcut]

func _ready() -> void:
	grid_container.columns = columns.size()
	for column in columns :
		var cell_title : Control = CELL_TITLE.instantiate()
		cell_title.title = column.title
		if "width" in column :
			cell_title.custom_minimum_size.x = column.width
		grid_container.add_child(cell_title)
	shortcuts = SteamDeckGameSyncCore.get_steam_shortcuts(steam_user_path)
	for shortcut in shortcuts:
		generate_editor_for_shortcut(shortcut)

func generate_editor_for_shortcut(shortcut : SteamShortcut):
	print("generating editor for game : ", shortcut.appname)
	
	for column in columns :
		var editor = column.scene.instantiate()
		editor.steam_shortcut = shortcut
		if "parameters" in column :
			for parameter in column.parameters :
				editor.set(parameter, column.parameters[parameter])
		grid_container.add_child(editor)


func _on_add_by_folder_pressed() -> void:
	file_dialog.file_mode = FileDialog.FILE_MODE_OPEN_DIR
	file_dialog.popup_centered_ratio()
	file_dialog.dir_selected.connect(add_by_folder)
	
func add_by_folder(path : String):
	var shortcut := SteamDeckGameSyncCore.create_steam_shortcut_for_game_folder(steam_user_path, path)
	generate_editor_for_shortcut(shortcut)
	shortcuts.append(shortcut)
	pass


func _on_save_shortcuts_pressed() -> void:
	SteamDeckGameSyncCore.add_games_as_shortcut_to_steam(steam_user_path, shortcuts)


func _on_settings_pressed() -> void:
	settings.popup_centered_ratio()
