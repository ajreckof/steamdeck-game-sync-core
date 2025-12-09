extends RefCounted


signal thumbnail_changed(new_thumbnail : Texture2D)

var thumbnail : Texture2D :
	set(value):
		thumbnail = value
		thumbnail_changed.emit(value)
var size : Vector2i
var url : String
var style : String
var notes : String
var full_image : Image 
