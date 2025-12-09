extends HBoxContainer

const ImageInfo = preload("uid://7uj3dg1qbfdo")

@onready var texture_rect: TextureRect = $TextureRect
@onready var label: Label = $ImageSelector/Control/Label
@onready var image_selector: ConfirmationDialog = $ImageSelector




var image_type : SteamDeckGameSyncCore.ImageType
var name_property_for_image_type := {
	SteamDeckGameSyncCore.HERO : "hero_image",
	SteamDeckGameSyncCore.GRID : "grid_image",
	SteamDeckGameSyncCore.LARGE_GRID : "large_grid_image",
	SteamDeckGameSyncCore.LOGO : "logo_image"
}
const steamgriddb_id_name_property := &"steamgriddb_id"


var steam_shortcut : SteamShortcut :
	set(value) :
		steam_shortcut = value
		steam_shortcut.property_changed.connect(_on_shortcut_property_changed)
		if is_node_ready() :
			retrieve_image()
			label.visible = steam_shortcut.steamgriddb_id == 0


func _on_shortcut_property_changed(property : StringName):
	print("reached here 2")
	if not is_node_ready() : 
		return
	if property == name_property_for_image_type[image_type]:
		retrieve_image()
	if property == steamgriddb_id_name_property :
		label.visible = steam_shortcut.steamgriddb_id == 0

func _ready() -> void:
	if steam_shortcut :
		retrieve_image()
		label.visible = steam_shortcut.steamgriddb_id == 0


func retrieve_image():
	var image : Image = steam_shortcut.get(name_property_for_image_type[image_type])
	if image :
		texture_rect.texture = ImageTexture.create_from_image(image)
	else :
		print("shortcut for {0} didn't have an image for {1}".format([steam_shortcut.appname, name_property_for_image_type[image_type]]))


func _on_search_image_button_pressed() -> void:
	image_selector.popup_centered_ratio()
	if steam_shortcut.steamgriddb_id != 0 :
		SteamDeckGameSyncCore.fetch_all_steamgriddb_images(steam_shortcut, image_type, _on_fetch_all_images)
	
func _on_fetch_all_images(response_array : Array) :
	var images : Array[ImageInfo]
	
	for response in response_array :
		for k in response :
			print(k, " : ", response[k])
		var image := ImageInfo.new()
		image.url = response.url
		image.style = response.style
		image.size = Vector2i(response.width, response.height)
		image.notes = response.notes if response.notes else ""
		SteamDeckGameSyncCore.download_images_by_url(response.thumb, func(thumb) : image.thumbnail = ImageTexture.create_from_image(thumb))
		images.append(image)
	image_selector.images = images
	


func _on_image_selector_image_selected(image: Image) -> void:
	print("reached here : ", name_property_for_image_type[image_type], name_property_for_image_type[image_type] in steam_shortcut)
	steam_shortcut.set(name_property_for_image_type[image_type], image)
