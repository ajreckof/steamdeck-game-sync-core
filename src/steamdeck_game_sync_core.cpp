#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/json.hpp>
#include <godot_cpp/classes/image_texture.hpp>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>


#include "steamdeck_game_sync_core.h"
#include "steam_shortcut_read_write.h"

#ifdef UNIX_ENABLED
#include <unistd.h>
#endif

SteamDeckGameSyncCore *SteamDeckGameSyncCore::steam_deck_game_sync_core_singleton = nullptr;

SteamDeckGameSyncCore::SteamDeckGameSyncCore() {
	callable_mp(this, &SteamDeckGameSyncCore::add_singleton_to_scene_tree).call_deferred();

	if (Engine::get_singleton()->is_editor_hint()) return;

	parallel_downloader = memnew(ParallelDownloader);
	parallel_downloader->set_debug_enabled(false);
	parallel_downloader->set_host("https://www.steamgriddb.com");
	parallel_downloader->set_max_parallel_request(2);
	add_child(parallel_downloader);

	image_downloader = memnew(ParallelDownloader);
	image_downloader->set_debug_enabled(false);
	image_downloader->set_host("https://cdn2.steamgriddb.com");
	image_downloader->set_max_parallel_request(6);
	add_child(image_downloader);

	
}

void SteamDeckGameSyncCore::add_singleton_to_scene_tree() {
	MainLoop *main_loop = Engine::get_singleton()->get_main_loop();
	if(!main_loop) {
		ERR_PRINT_ED("Failed to get MainLoop from Engine singleton.");
		return;
	}
	SceneTree *scene_tree = Object::cast_to<SceneTree>(main_loop);
	if (!scene_tree) {
		ERR_PRINT_ED("Failed to get SceneTree from MainLoop." + main_loop->to_string() + " is not a SceneTree.");
		return;
	}
	scene_tree->get_root()->add_child(this);
}

String SteamDeckGameSyncCore::get_steamgriddb_api_key() const {
	return steamgriddb_api_key;
}
void SteamDeckGameSyncCore::set_steamgriddb_api_key(const String &p_api_key) {
	steamgriddb_api_key = p_api_key;
	headers.clear();
	headers.append("Authorization: Bearer " + steamgriddb_api_key);
}


void SteamDeckGameSyncCore::_bind_methods() {
	ClassDB::bind_method(D_METHOD("find_executables_for_game", "p_shortcut"), &SteamDeckGameSyncCore::find_executables_for_game);
	ClassDB::bind_method(D_METHOD("add_games_as_shortcut_to_steam", "steam_user_data_path", "shortcuts"), &SteamDeckGameSyncCore::add_games_as_shortcut_to_steam);
	ClassDB::bind_method(D_METHOD("get_steam_shortcuts", "steam_user_data_path"), &SteamDeckGameSyncCore::get_steam_shortcuts);
	ClassDB::bind_method(D_METHOD("download_images_for_game", "shortcut"), &SteamDeckGameSyncCore::download_images_for_game);
	ClassDB::bind_method(D_METHOD("find_possible_app_name_from_folder_name", "folder_name", "callback"), &SteamDeckGameSyncCore::find_possible_app_name_from_folder_name);
	ClassDB::bind_method(D_METHOD("create_steam_shortcut_for_game_folder", "steam_user_data_path", "game_folder_path"), &SteamDeckGameSyncCore::create_steam_shortcut_for_game_folder);
	ClassDB::bind_method(D_METHOD("fetch_all_steamgriddb_images", "p_shortcut", "image_type", "callback"), &SteamDeckGameSyncCore::fetch_all_steamgriddb_images);
	ClassDB::bind_method(D_METHOD("download_images_by_url", "image_url", "callback"), &SteamDeckGameSyncCore::download_images_by_url);
	ClassDB::bind_method(D_METHOD("cancel_image_download", "image_url"), &SteamDeckGameSyncCore::cancel_image_download);
	ClassDB::bind_method(D_METHOD("get_steamgriddb_api_key"), &SteamDeckGameSyncCore::get_steamgriddb_api_key);
	ClassDB::bind_method(D_METHOD("set_steamgriddb_api_key", "api_key"), &SteamDeckGameSyncCore::set_steamgriddb_api_key);


	BIND_ENUM_CONSTANT(GRID);
	BIND_ENUM_CONSTANT(HERO);
	BIND_ENUM_CONSTANT(LOGO);
	BIND_ENUM_CONSTANT(LARGE_GRID);
}

TypedArray<SteamShortcut> SteamDeckGameSyncCore::get_steam_shortcuts(String steam_user_data_path) {

	
	// this is the file to be modified 
	String steam_shortcuts_vdf_path = steam_user_data_path.path_join("shortcuts.vdf");
	// create a vdf object to store everything
	return SteamShortcutRead::read_shortcuts(steam_shortcuts_vdf_path);

}




void SteamDeckGameSyncCore::add_games_as_shortcut_to_steam(String steam_user_data_path, TypedArray<SteamShortcut> shortcuts) {
	

	// this is the file to be modified 
	String steam_shortcuts_vdf_path = steam_user_data_path.path_join("shortcuts.vdf");

	SteamShortcutWrite::write_shortcuts(steam_shortcuts_vdf_path, shortcuts);

}



void SteamDeckGameSyncCore::find_executables_for_game(Ref<SteamShortcut> p_shortcut) {
	// Implementation to find executables for the given game path
	// This is a placeholder for the actual logic
	String game_path = p_shortcut->get_start_dir();

	Vector<SteamShortcut::ExeAlternative> executables;
	
	Ref<DirAccess> dir = DirAccess::open(game_path);
	ERR_FAIL_COND_EDMSG(dir.is_null(), "Error: Could not open directory: " + game_path);

	
	dir->list_dir_begin();
	String file_name = dir->get_next();
	
	while (file_name != "") {
		if (!dir->current_is_dir()) {
			String full_path = game_path.path_join(file_name);
			
			// Check if file has execute permissions (Unix-like systems)
			#ifdef UNIX_ENABLED
			if (!access(full_path.utf8().get_data(), X_OK)) {
				SteamShortcut::ExeAlternative exe_alt;
				exe_alt.exe = full_path;
				exe_alt.size = FileAccess::open(full_path, FileAccess::READ)->get_length();
				executables.append(exe_alt);
			}
			#endif
			// Windows: check for .exe files
			if (file_name.ends_with(".exe") || file_name.ends_with(".bat") || file_name.ends_with(".cmd")) {
				
				SteamShortcut::ExeAlternative exe_alt;
				exe_alt.exe = full_path;
				exe_alt.size = FileAccess::open(full_path, FileAccess::READ)->get_length();
				executables.append(exe_alt);
			}
		}
		file_name = dir->get_next();
	}

	p_shortcut->set_direct_exe_alternatives(executables);
}

void SteamDeckGameSyncCore::fetch_steamgriddb_image(Ref<SteamShortcut> p_shortcut, ImageType image_type){
	ERR_FAIL_COND_EDMSG(steamgriddb_api_key == "", "SteamGridDB API key is not set. Please set it before fetching images.");
	String base_url;
	String image_name;
	switch (image_type){
		case HERO:
			base_url = "https://www.steamgriddb.com/api/v2/heroes/game/" + itos(p_shortcut->steamgriddb_id);
			break;
		case LARGE_GRID:
			base_url = "https://www.steamgriddb.com/api/v2/grids/game/" + itos(p_shortcut->steamgriddb_id) + "?dimensions=920x430,460x215";
			break;
		case GRID:
			base_url = "https://www.steamgriddb.com/api/v2/grids/game/" + itos(p_shortcut->steamgriddb_id) + "?dimensions=600x900,342x482,660x930";
			break;
		case LOGO:
			base_url = "https://www.steamgriddb.com/api/v2/logos/game/" + itos(p_shortcut->steamgriddb_id);
			break;
		
		default:
			break;
	}
	parallel_downloader->download_json(base_url, callable_mp(this, &SteamDeckGameSyncCore::_on_db_url_response).bind(image_type, p_shortcut), headers);
}




void SteamDeckGameSyncCore::_on_db_url_response(Dictionary response_dict, ImageType image_type, Ref<SteamShortcut> p_shortcut) {
	// Parse the JSON to extract image URLs
	Array data_array = response_dict.get("data", Array());

	ERR_FAIL_COND_EDMSG(data_array.size() == 0, "No images found in SteamGridDB response.");

	Dictionary first_image = data_array[0];
	String image_url = first_image.get("url", "");

	ERR_FAIL_COND_EDMSG(image_url == "", "Image URL not found in SteamGridDB response.");
	Callable callback;
	switch (image_type){
		case HERO:
			callback = callable_mp(p_shortcut.ptr(), &SteamShortcut::set_hero_image);
			break;
		case GRID:
			callback = callable_mp(p_shortcut.ptr(), &SteamShortcut::set_grid_image);
			break;
		case LARGE_GRID:
			callback = callable_mp(p_shortcut.ptr(), &SteamShortcut::set_large_grid_image);
			break;
		case LOGO:
			callback = callable_mp(p_shortcut.ptr(), &SteamShortcut::set_logo_image);
			break;
		default :
			break;
	}
	image_downloader->download_image(image_url, callback);
}


void SteamDeckGameSyncCore::fetch_all_steamgriddb_images(Ref<SteamShortcut> p_shortcut, ImageType image_type, Callable callback) {
	ERR_FAIL_COND_EDMSG(steamgriddb_api_key == "", "SteamGridDB API key is not set. Please set it before fetching images.");
	String base_url;
	String image_name;
	switch (image_type){
		case HERO:
			base_url = "https://www.steamgriddb.com/api/v2/heroes/game/" + itos(p_shortcut->steamgriddb_id);
			break;
		case LARGE_GRID:	
			base_url = "https://www.steamgriddb.com/api/v2/grids/game/" + itos(p_shortcut->steamgriddb_id) + "?dimensions=920x430,460x215";
			break;
		case GRID:	
			base_url = "https://www.steamgriddb.com/api/v2/grids/game/" + itos(p_shortcut->steamgriddb_id) + "?dimensions=600x900,342x482,660x930";
			break;
		case LOGO:	
			base_url = "https://www.steamgriddb.com/api/v2/logos/game/" + itos(p_shortcut->steamgriddb_id);
			break;
		
		default:	
			break;
	}		
	parallel_downloader->download_json(base_url, callable_mp(this, &SteamDeckGameSyncCore::_on_db_url_all_images_response).bind(callback), headers);
}
void SteamDeckGameSyncCore::download_images_by_url(String image_url, Callable callback) {
	image_downloader->download_image(image_url, callback);
}

void SteamDeckGameSyncCore::cancel_image_download(String image_url) {
	image_downloader->cancel_download(image_url);
}

void SteamDeckGameSyncCore::_on_db_url_all_images_response(Dictionary response_dict, Callable callback) {
	// Parse the JSON to extract image URLs
	callback.call(response_dict.get("data", Array()));
}	


void SteamDeckGameSyncCore::download_images_for_game(Ref<SteamShortcut> p_shortcut) {
	ERR_FAIL_COND_EDMSG(p_shortcut->steamgriddb_id == 0, "SteamGridDB ID is not set for this shortcut. Please use ");
	for(int i = 0; i < ImageType::MAX; i++) {
		fetch_steamgriddb_image(p_shortcut, static_cast<ImageType>(i));
	}
}

void SteamDeckGameSyncCore::find_possible_app_name_from_folder_name(String folder_name, Callable callback) {
	ERR_FAIL_COND_EDMSG(steamgriddb_api_key == "", "SteamGridDB API key is not set. Please set it before fetching images.");
	// Simple heuristic: replace spaces with underscores and convert to lowercase
	String app_name = folder_name.replace(" ", "_").to_lower();

	String search_url = "https://www.steamgriddb.com/api/v2/search/autocomplete/" + folder_name;
	parallel_downloader->download_json(search_url, callable_mp(this, &SteamDeckGameSyncCore::_on_search_response).bind(callback), headers);
}



Ref<SteamShortcut> SteamDeckGameSyncCore::create_steam_shortcut_for_game_folder(String steam_user_data_path, String game_folder_path) {
	Ref<SteamShortcut> shortcut = memnew(SteamShortcut());
	shortcut->set_shortcut_path(game_folder_path);
	find_possible_app_name_from_folder_name(game_folder_path.get_file(), callable_mp(this, &SteamDeckGameSyncCore::_on_search_best_matching_response).bind(shortcut));
	
	return shortcut;
}

void SteamDeckGameSyncCore::_on_search_best_matching_response(Array responses, Ref<SteamShortcut> shortcut){
	ERR_FAIL_COND_EDMSG(responses.size() == 0, "SteamGridDB search did not return any results.");

	Dictionary best_match = responses[0];
	shortcut->set_steamgriddb_id(best_match.get("id", 0));
	shortcut->set_appname(best_match.get("name", ""));
}

void SteamDeckGameSyncCore::_on_search_response(Dictionary response_dict, Callable callback) {
	ERR_FAIL_COND_EDMSG((bool)response_dict.get("success", false) != true, "SteamGridDB search unsuccessful.");

	Array responses = response_dict.get("data", Array());
	ERR_FAIL_COND_EDMSG(responses.size() == 0, "SteamGridDB search did not return any results.");
	
	callback.call(responses);
}

