#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/wrapped.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/classes/http_request.hpp>
#include <godot_cpp/classes/image.hpp>

#include "steam_shortcuts.h"
#include "parallel_downloader.h"

using namespace godot;

enum ImageType {
	GRID, // Couverture
	HERO, // Arrière plan
	LOGO, // Logo
	LARGE_GRID, // Couverture Large
	MAX
};
class SteamDeckGameSyncCore : public Node {
	GDCLASS(SteamDeckGameSyncCore, Node)

	
protected:
	static SteamDeckGameSyncCore* steam_deck_game_sync_core_singleton;
	static void _bind_methods();
	String steamgriddb_api_key; // You may want to set this via a method or property
	ParallelDownloader *parallel_downloader;
	ParallelDownloader *image_downloader;
	PackedStringArray headers;
	void _on_db_url_response(Dictionary response_dict, ImageType image_type, Ref<SteamShortcut> p_shortcut);
	void fetch_steamgriddb_image(Ref<SteamShortcut> p_shortcut, ImageType image_type);
	void _on_db_url_all_images_response(Dictionary response_dict, Callable callback);
	void _on_search_response(Dictionary response_dict, Callable callback);
	void _on_search_best_matching_response(Array responses, Ref<SteamShortcut> shortcut);
	void save_image_to_local_path(Ref<Image> image, String local_path);
	
public:
	SteamDeckGameSyncCore();
	void add_singleton_to_scene_tree();
	~SteamDeckGameSyncCore() override = default;
	String get_steamgriddb_api_key() const;
	void set_steamgriddb_api_key(const String &p_api_key);
	void _on_request_completed(int result, int response_code, PackedStringArray headers, PackedByteArray body);
	void fetch_all_steamgriddb_images(Ref<SteamShortcut> p_shortcut, ImageType image_type, Callable callback);
	void download_images_by_url(String image_url, Callable callback);
	void cancel_image_download(String image_url);
	
	void find_executables_for_game(Ref<SteamShortcut> p_shortcut);
	void add_games_as_shortcut_to_steam(String steam_user_data_path, TypedArray<SteamShortcut> shortcuts);
	void download_images_for_game(Ref<SteamShortcut> p_shortcut);
	void find_possible_app_name_from_folder_name(String folder_name, Callable callback);
	Ref<SteamShortcut> create_steam_shortcut_for_game_folder(String steam_user_data_path, String game_folder_path);
	TypedArray<SteamShortcut> get_steam_shortcuts(String steam_user_data_path);

	static SteamDeckGameSyncCore *get_singleton() { return steam_deck_game_sync_core_singleton; }
	static void initialize_singleton() { steam_deck_game_sync_core_singleton = memnew(SteamDeckGameSyncCore);};
	static void cleanup_singleton() { steam_deck_game_sync_core_singleton = nullptr;};
};

VARIANT_ENUM_CAST(ImageType)
