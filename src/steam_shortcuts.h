#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/wrapped.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/templates/hash_set.hpp>

using namespace godot;



// public class Shortcut
// {
// 	public byte[]? AppId { get; set; }
// 	public string? AppName { get; set; }
// 	public string? Exe { get; set; }
// 	public string? StartDir { get; set; }
// 	public string? Icon { get; set; }
// 	public string? ShortcutPath { get; set; }
// 	public string? LaunchOptions { get; set; }
// 	public bool IsHidden { get; set; }
// 	public bool AllowDesktopConfig { get; set; }
// 	public bool AllowOverlay { get; set; }
// 	public bool OpenVR { get; set; }
// 	public bool Devkit { get; set; }
// 	public string? DevkitGameID { get; set; }
// 	public byte[]? DevkitOverrideAppID { get; set; }
// 	public byte[]? LastPlayTime { get; set; }
// 	public string? FlatpakAppID { get; set; }
// 	public string? SortAs { get; set; }
// 	public List<string> Tags { get; set; } = new List<string>();
// }

class SteamShortcut : public RefCounted {
	GDCLASS(SteamShortcut, RefCounted)
protected:
	static void _bind_methods();
	void emit_changed(StringName p_property);
	static HashSet<int> app_ids_used;
    
public:
	void set_appid(uint32_t p_appid);
	uint32_t get_appid() const;
	uint32_t appid;
    
    void set_steamgriddb_id(int p_steamgriddb_id);
    int get_steamgriddb_id() const;
    int steamgriddb_id;
    
    void set_appname(const String &p_appname);
    String get_appname() const;
    String appname;
    
    void set_exe(const String &p_exe);
    String get_exe() const;
    String exe;

    struct ExeAlternative {
        String exe;
        uint32_t size;
    };
    void set_exe_alternatives(const TypedArray<Dictionary> &p_exe_alternatives);
    void set_direct_exe_alternatives(const Vector<ExeAlternative> &p_exe_alternatives);
    TypedArray<Dictionary> get_exe_alternatives() const;
    Vector<ExeAlternative> get_direct_exe_alternatives() const;
    Vector<ExeAlternative> exe_alternatives;
    
    void set_start_dir(const String &p_start_dir);
    String get_start_dir() const;
    String start_dir;
    
    void set_icon(const String &p_icon);
    String get_icon() const;
    String icon;
    
    void set_shortcut_path(const String &p_shortcut_path);
    String get_shortcut_path() const;
    String shortcut_path;
    
    void set_launch_options(const String &p_launch_options);
    String get_launch_options() const;
    String launch_options;
    
    void set_is_hidden(bool p_is_hidden);
    bool get_is_hidden() const;
    bool is_hidden;
    
    void set_allow_desktop_config(bool p_allow_desktop_config);
    bool get_allow_desktop_config() const;
    bool allow_desktop_config;
    
    void set_allow_overlay(bool p_allow_overlay);
    bool get_allow_overlay() const;
    bool allow_overlay;
    
    void set_open_vr(bool p_open_vr);
    bool get_open_vr() const;
    bool open_vr;
    
    void set_devkit(bool p_devkit);
    bool get_devkit() const;
    bool devkit;
    
    void set_devkit_game_id(const String &p_devkit_game_id);
    String get_devkit_game_id() const;
    String devkit_game_id;
    
    void set_devkit_override_app_id(int p_devkit_override_app_id);
    int get_devkit_override_app_id() const;
    int devkit_override_app_id;
    
    void set_last_play_time(bool p_last_play_time);
    bool get_last_play_time() const;
    bool last_play_time;
    
    void set_flatpak_app_id(const String &p_flatpak_app_id);
    String get_flatpak_app_id() const;
    String flatpak_app_id;
    
    void set_sort_as(const String &p_sort_as);
    String get_sort_as() const;
    String sort_as;
    
    void set_tags(const PackedStringArray &p_tags);
    PackedStringArray get_tags() const;
    PackedStringArray tags;
    
    void set_hero_image(Ref<Image> p_hero_image);
    Ref<Image> get_hero_image();
    Ref<Image> hero_image;
    
    void set_grid_image(Ref<Image> p_grid_image);
    Ref<Image> get_grid_image();
    Ref<Image> grid_image;
    
    void set_large_grid_image(Ref<Image> p_large_grid_image);
    Ref<Image> get_large_grid_image();
    Ref<Image> large_grid_image;
    
    void set_logo_image(Ref<Image> p_logo_image);
    Ref<Image> get_logo_image();
    Ref<Image> logo_image;

    const String steamgriddb_id_string_indicator = "steamgriddb_id:";
    PackedStringArray get_steamgriddb_id_and_shortcut_tags() {
        PackedStringArray result;
        result.append(steamgriddb_id_string_indicator + itos(steamgriddb_id));
        for (int i = 0; i < tags.size(); i++) {
            result.append(tags[i]);
        }
        return result;
    }
    
    SteamShortcut() {};
};


