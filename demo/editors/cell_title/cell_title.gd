@tool
extends PanelContainer


@onready var label: Label = $Label

@export var title : String :
	set(value) :
		if not is_node_ready():
			await ready
		label.text = value
	get:
		if not is_node_ready():
			return ""
		return label.text
		
