extends TextureRect


func _make_custom_tooltip(for_text: String) -> Object:
	var texture_rect := TextureRect.new()
	texture_rect.expand_mode = TextureRect.EXPAND_KEEP_SIZE
	texture_rect.stretch_mode = TextureRect.STRETCH_KEEP
	texture_rect.texture = texture
	return texture_rect
