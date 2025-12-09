extends Control

const ImageInfo = preload("uid://7uj3dg1qbfdo")

const additional_info_text := "style : {style}
size : {size_x}x{size_y}
notes : {notes}"

@onready var texture_rect: TextureRect = $TextureRect
@onready var label: Label = $Label


var image : ImageInfo :
	set(value):
		image = value
		if image.full_image :
			texture_rect.texture = ImageTexture.create_from_image(image.full_image)
		else :
			SteamDeckGameSyncCore.download_images_by_url(image.url, set_full_image)
		label.text = additional_info_text.format({size_x = image.size.x, size_y = image.size.y, notes = image.notes, style = image.style})
	

func set_full_image(new_image : Image) :
	image.full_image = new_image
	texture_rect.texture = ImageTexture.create_from_image(image.full_image)
