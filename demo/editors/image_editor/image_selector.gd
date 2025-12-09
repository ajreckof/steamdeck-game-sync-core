extends ConfirmationDialog

signal image_selected(image : Image)

const ImageInfo = preload("uid://7uj3dg1qbfdo")

@onready var item_list: ItemList = $Control/HSplitContainer/ItemList
@onready var image_preview: Control = $Control/HSplitContainer/ImagePreview




var images : Array[ImageInfo] :
	set(value):
		item_list.clear()
		images = value
		for image in images :
			var idx = item_list.add_icon_item(image.thumbnail)
			print("adding image to list at index : ", idx)
			image.thumbnail_changed.connect(func(texture) : item_list.set_item_icon(idx, texture))


func _on_item_list_item_selected(index: int) -> void:
	image_preview.image = images[index]


func _on_item_list_item_activated(index: int) -> void:
	_emit_image_selected(images[index])
	hide()

func _on_confirmed() -> void:
	_emit_image_selected(image_preview.image)



func _emit_image_selected(image_info : ImageInfo) :
	if image_info.full_image :
		image_selected.emit(image_info.full_image)
	else :
		SteamDeckGameSyncCore.download_images_by_url(image_info.url, image_selected.emit)

	
