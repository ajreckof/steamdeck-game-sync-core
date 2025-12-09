extends SuggestionLineEdit

const too_small_text_for_suggestion_message := "Rentrez au moins trois charactère pour commencer à avoir des suggestions."

@export var time_before_generating_suggestions := 0.25
@export var minimum_length_for_suggestion := 3

var time_since_last_edited : float = -1
var possible_games : Array

func _generate_suggestions() -> void:
	if text.length() < minimum_length_for_suggestion :
		time_since_last_edited = -1
		set_placeholder_suggestion(too_small_text_for_suggestion_message)
	else :
		time_since_last_edited = 0

func _process(delta: float) -> void:
	if time_since_last_edited < 0 : 
		return
	time_since_last_edited += delta
	if time_since_last_edited > time_before_generating_suggestions :
		print("started looking for : ", text)
		SteamDeckGameSyncCore.find_possible_app_name_from_folder_name(text, _on_possible_appname_found.bind(text))
		time_since_last_edited = -1

func _on_possible_appname_found(p_possible_games : Array , request_text : String) :
	print(p_possible_games, request_text)
	if request_text != text :
		return
	possible_games = p_possible_games
	var possible_app_names : PackedStringArray
	for game in possible_games :
		possible_app_names.append(game.name)
	set_suggestions(possible_app_names)
