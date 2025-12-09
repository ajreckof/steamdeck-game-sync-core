#include "steam_shortcuts.h"
#include "steamdeck_game_sync_core.h"


#define MAKE_RESOURCE_TYPE_HINT(m_type) vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, m_type)

void SteamShortcut::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_appid", "appid"), &SteamShortcut::set_appid);
	ClassDB::bind_method(D_METHOD("get_appid"), &SteamShortcut::get_appid);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "appid"), "set_appid", "get_appid");

	ClassDB::bind_method(D_METHOD("set_steamgriddb_id", "steamgriddb_id"), &SteamShortcut::set_steamgriddb_id);
	ClassDB::bind_method(D_METHOD("get_steamgriddb_id"), &SteamShortcut::get_steamgriddb_id);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "steamgriddb_id"), "set_steamgriddb_id", "get_steamgriddb_id");

	ClassDB::bind_method(D_METHOD("set_appname", "appname"), &SteamShortcut::set_appname);
	ClassDB::bind_method(D_METHOD("get_appname"), &SteamShortcut::get_appname);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "appname"), "set_appname", "get_appname");

	ClassDB::bind_method(D_METHOD("set_exe", "exe"), &SteamShortcut::set_exe);
	ClassDB::bind_method(D_METHOD("get_exe"), &SteamShortcut::get_exe);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "exe"), "set_exe", "get_exe");

	ClassDB::bind_method(D_METHOD("set_exe_alternatives", "exe_alternatives"), &SteamShortcut::set_exe_alternatives);
	ClassDB::bind_method(D_METHOD("get_exe_alternatives"), &SteamShortcut::get_exe_alternatives);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "exe_alternatives", PROPERTY_HINT_ARRAY_TYPE, "Dictionary"), "set_exe_alternatives", "get_exe_alternatives");

	ClassDB::bind_method(D_METHOD("set_start_dir", "start_dir"), &SteamShortcut::set_start_dir);
	ClassDB::bind_method(D_METHOD("get_start_dir"), &SteamShortcut::get_start_dir);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "start_dir"), "set_start_dir", "get_start_dir");

	ClassDB::bind_method(D_METHOD("set_icon", "icon"), &SteamShortcut::set_icon);
	ClassDB::bind_method(D_METHOD("get_icon"), &SteamShortcut::get_icon);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "icon"), "set_icon", "get_icon");

	ClassDB::bind_method(D_METHOD("set_shortcut_path", "shortcut_path"), &SteamShortcut::set_shortcut_path);
	ClassDB::bind_method(D_METHOD("get_shortcut_path"), &SteamShortcut::get_shortcut_path);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "shortcut_path"), "set_shortcut_path", "get_shortcut_path");

	ClassDB::bind_method(D_METHOD("set_launch_options", "launch_options"), &SteamShortcut::set_launch_options);
	ClassDB::bind_method(D_METHOD("get_launch_options"), &SteamShortcut::get_launch_options);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "launch_options"), "set_launch_options", "get_launch_options");

	ClassDB::bind_method(D_METHOD("set_is_hidden", "is_hidden"), &SteamShortcut::set_is_hidden);
	ClassDB::bind_method(D_METHOD("get_is_hidden"), &SteamShortcut::get_is_hidden);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_hidden"), "set_is_hidden", "get_is_hidden");

	ClassDB::bind_method(D_METHOD("set_allow_desktop_config", "allow_desktop_config"), &SteamShortcut::set_allow_desktop_config);
	ClassDB::bind_method(D_METHOD("get_allow_desktop_config"), &SteamShortcut::get_allow_desktop_config);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "allow_desktop_config"), "set_allow_desktop_config", "get_allow_desktop_config");

	ClassDB::bind_method(D_METHOD("set_allow_overlay", "allow_overlay"), &SteamShortcut::set_allow_overlay);
	ClassDB::bind_method(D_METHOD("get_allow_overlay"), &SteamShortcut::get_allow_overlay);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "allow_overlay"), "set_allow_overlay", "get_allow_overlay");

	ClassDB::bind_method(D_METHOD("set_open_vr", "open_vr"), &SteamShortcut::set_open_vr);
	ClassDB::bind_method(D_METHOD("get_open_vr"), &SteamShortcut::get_open_vr);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "open_vr"), "set_open_vr", "get_open_vr");

	ClassDB::bind_method(D_METHOD("set_devkit", "devkit"), &SteamShortcut::set_devkit);
	ClassDB::bind_method(D_METHOD("get_devkit"), &SteamShortcut::get_devkit);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "devkit"), "set_devkit", "get_devkit");

	ClassDB::bind_method(D_METHOD("set_devkit_game_id", "devkit_game_id"), &SteamShortcut::set_devkit_game_id);
	ClassDB::bind_method(D_METHOD("get_devkit_game_id"), &SteamShortcut::get_devkit_game_id);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "devkit_game_id"), "set_devkit_game_id", "get_devkit_game_id");

	ClassDB::bind_method(D_METHOD("set_devkit_override_app_id", "devkit_override_app_id"), &SteamShortcut::set_devkit_override_app_id);
	ClassDB::bind_method(D_METHOD("get_devkit_override_app_id"), &SteamShortcut::get_devkit_override_app_id);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "devkit_override_app_id"), "set_devkit_override_app_id", "get_devkit_override_app_id");

	ClassDB::bind_method(D_METHOD("set_last_play_time", "last_play_time"), &SteamShortcut::set_last_play_time);
	ClassDB::bind_method(D_METHOD("get_last_play_time"), &SteamShortcut::get_last_play_time);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "last_play_time"), "set_last_play_time", "get_last_play_time");

	ClassDB::bind_method(D_METHOD("set_flatpak_app_id", "flatpak_app_id"), &SteamShortcut::set_flatpak_app_id);
	ClassDB::bind_method(D_METHOD("get_flatpak_app_id"), &SteamShortcut::get_flatpak_app_id);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "flatpak_app_id"), "set_flatpak_app_id", "get_flatpak_app_id");

	ClassDB::bind_method(D_METHOD("set_sort_as", "sort_as"), &SteamShortcut::set_sort_as);
	ClassDB::bind_method(D_METHOD("get_sort_as"), &SteamShortcut::get_sort_as);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "sort_as"), "set_sort_as", "get_sort_as");

	ClassDB::bind_method(D_METHOD("set_tags", "tags"), &SteamShortcut::set_tags);
	ClassDB::bind_method(D_METHOD("get_tags"), &SteamShortcut::get_tags);
	ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "tags"), "set_tags", "get_tags");

	ClassDB::bind_method(D_METHOD("set_hero_image", "hero_image"), &SteamShortcut::set_hero_image);
	ClassDB::bind_method(D_METHOD("get_hero_image"), &SteamShortcut::get_hero_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "hero_image"), "set_hero_image", "get_hero_image");

	ClassDB::bind_method(D_METHOD("set_logo_image", "logo_image"), &SteamShortcut::set_logo_image);
	ClassDB::bind_method(D_METHOD("get_logo_image"), &SteamShortcut::get_logo_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "logo_image"), "set_logo_image", "get_logo_image");

	ClassDB::bind_method(D_METHOD("set_grid_image", "grid_image"), &SteamShortcut::set_grid_image);
	ClassDB::bind_method(D_METHOD("get_grid_image"), &SteamShortcut::get_grid_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "grid_image"), "set_grid_image", "get_grid_image");

	ClassDB::bind_method(D_METHOD("set_large_grid_image", "large_grid_image"), &SteamShortcut::set_large_grid_image);
	ClassDB::bind_method(D_METHOD("get_large_grid_image"), &SteamShortcut::get_large_grid_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "large_grid_image"), "set_large_grid_image", "get_large_grid_image");

	ADD_SIGNAL(MethodInfo("property_changed", PropertyInfo(Variant::STRING_NAME,"property_name")));

}


void SteamShortcut::emit_changed(StringName p_property) {
	emit_signal("property_changed", p_property);
}


HashSet<int> SteamShortcut::app_ids_used = HashSet<int>();


void SteamShortcut::set_appid(uint32_t p_appid) {
	if (appid == p_appid) return;
	ERR_FAIL_COND_EDMSG(app_ids_used.has(p_appid), "appid(" + itos(p_appid) + ") is already in use please use generated appid from generate_appid.");
	app_ids_used.insert(p_appid);
	appid = p_appid;
	app_ids_used.erase(appid);
	emit_changed(StringName("appid"));
}
uint32_t SteamShortcut::get_appid() const {
	return appid;
}

void SteamShortcut::set_steamgriddb_id(int p_steamgriddb_id) {
	steamgriddb_id = p_steamgriddb_id;
	if (steamgriddb_id != 0) {
		SteamDeckGameSyncCore::get_singleton()->download_images_for_game(Ref<SteamShortcut>(this));
	}
	emit_changed(StringName("steamgriddb_id"));
}
int SteamShortcut::get_steamgriddb_id() const {
	return steamgriddb_id;
}

void SteamShortcut::set_appname(const String &p_appname) {
	appname = p_appname;
	emit_changed(StringName("appname"));
}
String SteamShortcut::get_appname() const {
	return appname;
}

void SteamShortcut::set_exe(const String &p_exe) {
	exe = p_exe;
	emit_changed(StringName("exe"));
}
String SteamShortcut::get_exe() const {
	return exe;
}

void SteamShortcut::set_exe_alternatives(const TypedArray<Dictionary> &p_exe_alternatives) {
	exe_alternatives.clear();
	for (int i = 0; i < p_exe_alternatives.size(); i++) {
		Dictionary dict = p_exe_alternatives[i];
		ExeAlternative alt;
		alt.exe = dict.get("exe", "");
		alt.size = uint32_t(dict.get("size", 0));
		exe_alternatives.append(alt);
	}
	emit_changed(StringName("exe_alternatives"));
}

void SteamShortcut::set_direct_exe_alternatives(const Vector<ExeAlternative> &p_exe_alternatives) {
	exe_alternatives = p_exe_alternatives;
	
	emit_changed(StringName("exe_alternatives"));
}

TypedArray<Dictionary> SteamShortcut::get_exe_alternatives() const {
	TypedArray<Dictionary> arr;
	for (int i = 0; i < exe_alternatives.size(); i++) {
		Dictionary dict;
		dict["exe"] = exe_alternatives[i].exe;
		dict["size"] = int(exe_alternatives[i].size);
		arr.append(dict);
	}
	return arr;
}

Vector<SteamShortcut::ExeAlternative> SteamShortcut::get_direct_exe_alternatives() const {
	return exe_alternatives;
}

void SteamShortcut::set_start_dir(const String &p_start_dir) {
	start_dir = p_start_dir;
	emit_changed(StringName("start_dir"));
}
String SteamShortcut::get_start_dir() const {
	return start_dir;
}

void SteamShortcut::set_icon(const String &p_icon) {
	icon = p_icon;
	emit_changed(StringName("icon"));
}
String SteamShortcut::get_icon() const {
	return icon;
}

void SteamShortcut::set_shortcut_path(const String &p_shortcut_path) {
	shortcut_path = p_shortcut_path;
	emit_changed(StringName("shortcut_path"));
}
String SteamShortcut::get_shortcut_path() const {
	return shortcut_path;
}

void SteamShortcut::set_launch_options(const String &p_launch_options) {
	launch_options = p_launch_options;
	emit_changed(StringName("launch_options"));
}
String SteamShortcut::get_launch_options() const {
	return launch_options;
}

void SteamShortcut::set_is_hidden(bool p_is_hidden) {
	is_hidden = p_is_hidden;
	emit_changed(StringName("is_hidden"));
}
bool SteamShortcut::get_is_hidden() const {
	return is_hidden;
}

void SteamShortcut::set_allow_desktop_config(bool p_allow_desktop_config) {
	allow_desktop_config = p_allow_desktop_config;
	emit_changed(StringName("allow_desktop_config"));
}
bool SteamShortcut::get_allow_desktop_config() const {
	return allow_desktop_config;
}

void SteamShortcut::set_allow_overlay(bool p_allow_overlay) {
	allow_overlay = p_allow_overlay;
	emit_changed(StringName("allow_overlay"));
}
bool SteamShortcut::get_allow_overlay() const {
	return allow_overlay;
}

void SteamShortcut::set_open_vr(bool p_open_vr) {
	open_vr = p_open_vr;
	emit_changed(StringName("open_vr"));
}
bool SteamShortcut::get_open_vr() const {
	return open_vr;
}

void SteamShortcut::set_devkit(bool p_devkit) {
	devkit = p_devkit;
	emit_changed(StringName("devkit"));
}
bool SteamShortcut::get_devkit() const {
	return devkit;
}

void SteamShortcut::set_devkit_game_id(const String &p_devkit_game_id) {
	devkit_game_id = p_devkit_game_id;
	emit_changed(StringName("devkit_game_id"));
}
String SteamShortcut::get_devkit_game_id() const {
	return devkit_game_id;
}

void SteamShortcut::set_devkit_override_app_id(int p_devkit_override_app_id) {
	devkit_override_app_id = p_devkit_override_app_id;
	emit_changed(StringName("devkit_override_app_id"));
}
int SteamShortcut::get_devkit_override_app_id() const {
	return devkit_override_app_id;
}

void SteamShortcut::set_last_play_time(bool p_last_play_time) {
	last_play_time = p_last_play_time;
	emit_changed(StringName("last_play_time"));
}
bool SteamShortcut::get_last_play_time() const {
	return last_play_time;
}

void SteamShortcut::set_flatpak_app_id(const String &p_flatpak_app_id) {
	flatpak_app_id = p_flatpak_app_id;
	emit_changed(StringName("flatpak_app_id"));
}
String SteamShortcut::get_flatpak_app_id() const {
	return flatpak_app_id;
}

void SteamShortcut::set_sort_as(const String &p_sort_as) {
	sort_as = p_sort_as;
	emit_changed(StringName("sort_as"));
}
String SteamShortcut::get_sort_as() const {
	return sort_as;
}

void SteamShortcut::set_tags(const PackedStringArray &p_tags) {
	tags = p_tags;
	emit_changed(StringName("tags"));
}
PackedStringArray SteamShortcut::get_tags() const {
	return tags;
}

void SteamShortcut::set_hero_image(Ref<Image> p_hero_image) {
	hero_image = p_hero_image;
	emit_changed(StringName("hero_image"));
}
Ref<Image> SteamShortcut::get_hero_image() {
	return hero_image;
}

void SteamShortcut::set_logo_image(Ref<Image> p_logo_image) {
	logo_image = p_logo_image;
	emit_changed(StringName("logo_image"));
}
Ref<Image> SteamShortcut::get_logo_image() {
	return logo_image;
}

void SteamShortcut::set_grid_image(Ref<Image> p_grid_image) {
	grid_image = p_grid_image;
	emit_changed(StringName("grid_image"));
}
Ref<Image> SteamShortcut::get_grid_image() {
	return grid_image;
}

void SteamShortcut::set_large_grid_image(Ref<Image> p_large_grid_image) {
	large_grid_image = p_large_grid_image;
	emit_changed(StringName("large_grid_image"));
}
Ref<Image> SteamShortcut::get_large_grid_image() {
	return large_grid_image;
}



