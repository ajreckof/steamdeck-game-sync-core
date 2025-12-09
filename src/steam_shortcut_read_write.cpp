#include "steam_shortcuts.h"
#include "steam_shortcut_read_write.h"
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/dir_access.hpp>

String SteamShortcutRead::read_null_terminated_string(PackedByteArray data, int &position){
	PackedByteArray null_terminated_byte_string;
	while (true)
	{
		int b = data[position++];
		if (b == 0) break;
		null_terminated_byte_string.append(b);
	}
	return null_terminated_byte_string.get_string_from_utf8();
}


TypedArray<SteamShortcut> SteamShortcutRead::read_shortcuts(String filepath)
{

	TypedArray<SteamShortcut> shortcuts;
	int position = 0;
	if (!FileAccess::file_exists(filepath))
		return shortcuts;

	PackedByteArray data = FileAccess::get_file_as_bytes(filepath);

	int rootType = data[position++];
	String rootKey = read_null_terminated_string(data, position);

	ERR_FAIL_COND_V_EDMSG(rootType != 0x00 || rootKey != "shortcuts",shortcuts, "Unexpected root key or type");
	

	String grid_folder = filepath.get_base_dir().path_join("grid");
	String local_path;
	Ref<Image> image;
	while (true)
	{
		int type = data[position++];
		if (type == 0x08) break; // end of shortcuts dictionary
		String rootKey = read_null_terminated_string(data, position);

		ERR_FAIL_COND_V_EDMSG(type != 0x00, shortcuts, "Expected dictionary key (0x00), got 0x" + String::num_int64(type, 16).to_upper());
		Ref<SteamShortcut> steam_shortcut = parse_shortcut(data, position);

		local_path = grid_folder.path_join(itos(steam_shortcut->appid) + "_hero.png");
		if (FileAccess::file_exists(local_path)) {
			image = Image::load_from_file(local_path);
			steam_shortcut->set_hero_image(image);
		}

		local_path = grid_folder.path_join(itos(steam_shortcut->appid) + "p.png");
		if (FileAccess::file_exists(local_path)){
			image = Image::load_from_file(local_path);
			steam_shortcut->set_grid_image(image);
		}

		local_path = grid_folder.path_join(itos(steam_shortcut->appid) + ".png");
		if (FileAccess::file_exists(local_path)){
			image = Image::load_from_file(local_path);
			steam_shortcut->set_large_grid_image(image);
		}

		local_path = grid_folder.path_join(itos(steam_shortcut->appid) + "_logo.png");
		if (FileAccess::file_exists(local_path)){
			image = Image::load_from_file(local_path);
			steam_shortcut->set_logo_image(image);
		}

		shortcuts.append(steam_shortcut);
	}

	return shortcuts;
}

Ref<SteamShortcut> SteamShortcutRead::parse_shortcut(PackedByteArray data, int &position)
{
	Ref<SteamShortcut> sc = memnew(SteamShortcut);

	while (true)
	{
		int type = data[position++];
		if (type == 0x08) break; // end of this shortcut dict

		String key = read_null_terminated_string(data, position);
		uint32_t value = 0;
		switch (type)
		{
			case 0x00: // nested dict, e.g. tags
				if (key == "tags")
					for (String tag : parse_tags(data, position)){
						if (tag.begins_with(sc->steamgriddb_id_string_indicator)){
							String id_str = tag.trim_prefix(sc->steamgriddb_id_string_indicator);
							sc->steamgriddb_id = id_str.to_int();
						}
						else {
							sc->tags.append(tag);
						}
					}
				else
					skip_dictionary(data, position);
				break;
			case 0x01: // string
				assign_string(sc, key, read_null_terminated_string(data, position));
				break;
			case 0x02: // int32
				ERR_FAIL_COND_V_EDMSG(data.size() < position + 4, sc, "Not enough data for int32 for key '" + key + "'");
				value |= (data[position] << 0);
				value |= (data[position + 1] << 8);
				value |= (data[position + 2] << 16);
				value |= (data[position + 3] << 24);
				assign_int(sc, key, value);
				position += 4; // skip int32
				break;
			default:
				ERR_FAIL_V_EDMSG(sc, "Unknown type 0x" + String::num_int64(type, 16).to_upper() + " for key '" + key + "'");
		}
	}
	return sc;
}

PackedStringArray SteamShortcutRead::parse_tags(PackedByteArray data, int &position)
{
	PackedStringArray tags;
	while (true)
	{
		int type = data[position++];
		if (type == 0x08) break; // end of tags dict

		ERR_FAIL_COND_V_EDMSG(type != 0x01, tags, "Expected string type (0x01) in tags dict, got 0x" + String::num_int64(type, 16).to_upper());

		String key = read_null_terminated_string(data, position);
		String val = read_null_terminated_string(data, position);
		tags.append(val);
	}
	return tags;
}


void SteamShortcutRead::skip_dictionary(PackedByteArray data, int &position)
{
	while (true)
	{
		int type = data[position++];
		if (type == 0x08) break; // end of dictionary

		switch (type)
		{
			case 0x00:
				skip_dictionary(data, position);
				break;
			case 0x01:
				read_null_terminated_string(data, position);
				break;
			case 0x02:
				position+= 4; // skip int32
				break;
			default:
				ERR_FAIL_EDMSG("Unknown type 0x" + String::num_int64(type, 16).to_upper() + " while skipping dict");
		}
	}
}

void SteamShortcutRead::assign_string(Ref<SteamShortcut> sc, String key, String val)
{
	if (key == "AppName"){ 
		sc->appname = val; 
	}
	else if (key == "Exe"){ 
		sc->exe = val; 
	}
	else if (key == "StartDir"){ 
		sc->start_dir = val; 
	}
	else if (key == "icon"){ 
		sc->icon = val; 
	}
	else if (key == "ShortcutPath"){ 
		sc->shortcut_path = val; 
	}
	else if (key == "LaunchOptions"){ 
		sc->launch_options = val; 
	}
	else if (key == "FlatpakAppID"){ 
		sc->flatpak_app_id = val; 
	}
	else if (key == "sortas"){ 
		sc->sort_as = val; 
	}
	else if (key == "DevkitGameID"){ 
		sc->devkit_game_id = val; 
	}
}

void SteamShortcutRead::assign_int(Ref<SteamShortcut> sc, String key, uint32_t val)
{
	if (key == "appid"){
		sc->appid = val;
	}
	else if (key == "IsHidden"){
		sc->is_hidden = val != 0;
	}
	else if (key == "AllowDesktopConfig"){
		sc->allow_desktop_config = val != 0;
	}
	else if (key == "AllowOverlay"){
		sc->allow_overlay = val != 0;
	}
	else if (key == "OpenVR"){
		sc->open_vr = val != 0;
	}
	else if (key == "Devkit"){
		sc->devkit = val != 0;
	}
	else if (key == "DevkitOverrideAppID"){
		sc->devkit_override_app_id = val;
	}
	else if (key == "LastPlayTime"){
		sc->last_play_time = val;
	}
	
}



// Write section


PackedByteArray SteamShortcutWrite::encode_shortcut(Ref<SteamShortcut> shortcut)
{
	PackedByteArray shortcut_bytes;

	 // Write AppID: type (0x02), key, null terminator, then 4 raw bytes (no terminator)
	shortcut_bytes.append(0x02); // int32 type
	shortcut_bytes.append_array(String("appid").to_utf8_buffer());
	shortcut_bytes.append(0x00);

	shortcut_bytes.append_array(encode_u32_value(shortcut->appid));

	shortcut_bytes.append_array(encode_string_field("AppName", shortcut->appname));
	shortcut_bytes.append_array(encode_string_field("Exe", shortcut->exe));
	shortcut_bytes.append_array(encode_string_field("StartDir", shortcut->start_dir));
	shortcut_bytes.append_array(encode_string_field("icon", shortcut->icon));
	shortcut_bytes.append_array(encode_string_field("ShortcutPath", shortcut->shortcut_path));
	shortcut_bytes.append_array(encode_string_field("LaunchOptions", shortcut->launch_options));
	shortcut_bytes.append_array(encode_bool_field("IsHidden", shortcut->is_hidden));
	shortcut_bytes.append_array(encode_bool_field("AllowDesktopConfig", shortcut->allow_desktop_config));
	shortcut_bytes.append_array(encode_bool_field("AllowOverlay", shortcut->allow_overlay));
	shortcut_bytes.append_array(encode_bool_field("OpenVR", shortcut->open_vr));
	shortcut_bytes.append_array(encode_bool_field("Devkit", shortcut->devkit));
	shortcut_bytes.append_array(encode_string_field("DevkitGameID", shortcut->devkit_game_id));
	shortcut_bytes.append_array(encode_int_field("DevkitOverrideAppID", shortcut->devkit_override_app_id));
	shortcut_bytes.append_array(encode_int_field("LastPlayTime", shortcut->last_play_time));
	shortcut_bytes.append_array(encode_string_field("FlatpakAppID", shortcut->flatpak_app_id));
	shortcut_bytes.append_array(encode_string_field("sortas", shortcut->sort_as));
	 
	shortcut_bytes.append_array(encode_tags(shortcut->get_steamgriddb_id_and_shortcut_tags()));
	
	

	return shortcut_bytes;
}
// Helper to write string fields

PackedByteArray SteamShortcutWrite::encode_u32_value(int value){
	PackedByteArray bytes;
	bytes.resize(4); // Ensure enough space for 4 bytes
	bytes.encode_u32(0, value);
	return bytes;
}

PackedByteArray SteamShortcutWrite::encode_string_field(String key, String value){
	PackedByteArray bytes;
	bytes.append(0x01); // string type
	bytes.append_array(key.to_utf8_buffer());
	bytes.append(0x00);
	bytes.append_array(value.to_utf8_buffer());
	bytes.append(0x00);
	return bytes;
}

// Helper to write bool fields (int32 with 0 or 1)
PackedByteArray SteamShortcutWrite::encode_bool_field(String key, bool value){
	PackedByteArray bytes;
	bytes.append(0x02); // int32 type
	bytes.append_array(key.to_utf8_buffer());
	bytes.append(0x00);
	bytes.append_array(encode_u32_value(value ? 1 : 0));
	return bytes;
}

// Helper to write int32 fields
PackedByteArray SteamShortcutWrite::encode_int_field(String key, int value){
	PackedByteArray bytes;
	bytes.append(0x02); // int32 type
	bytes.append_array(key.to_utf8_buffer());
	bytes.append(0x00);
	bytes.append_array(encode_u32_value(value));
	return bytes;
}
PackedByteArray SteamShortcutWrite::encode_tags(PackedStringArray tags){
	PackedByteArray bytes;
	bytes.append(0x00); // begin dict
	bytes.append_array(String("tags").to_utf8_buffer());
	bytes.append(0x00);

	for (int i = 0; i < tags.size(); i++){
		bytes.append(0x01); // string type
		bytes.append_array(itos(i).to_utf8_buffer());
		bytes.append(0x00);
		bytes.append_array(tags[i].to_utf8_buffer());
		bytes.append(0x00);
	}
	bytes.append(0x08); // end dict
	return bytes;
}


void SteamShortcutWrite::write_shortcuts(String filepath, TypedArray<SteamShortcut> shortcuts){
	PackedByteArray shortcuts_bytes;

	shortcuts_bytes.append(0x00); // root dict type
	shortcuts_bytes.append_array(String("shortcuts").to_utf8_buffer());
	shortcuts_bytes.append(0x00);
	String local_path;
	Error loading_error;
	String grid_folder = filepath.get_base_dir().path_join("grid");
	Ref<Image> image;

	for (int i = 0; i < shortcuts.size(); i++){
		Ref<SteamShortcut> shortcut = shortcuts[i];
		shortcuts_bytes.append(0x00); // dict key type (string)
		shortcuts_bytes.append_array(itos(i).to_utf8_buffer());
		shortcuts_bytes.append(0x00);
		shortcuts_bytes.append_array(encode_shortcut(shortcut));
		shortcuts_bytes.append(0x08); // end of shortcut dictionary


		local_path = grid_folder.path_join(itos(shortcut->appid) + "_hero.png");
		image = shortcut->get_hero_image();
		if (image.is_valid()) {
			image->save_png(local_path);
		}
		
		local_path = grid_folder.path_join(itos(shortcut->appid) + "p.png");
		image = shortcut->get_grid_image();
		if (image.is_valid()) {
			image->save_png(local_path);
		}

		local_path = grid_folder.path_join(itos(shortcut->appid) + ".png");
		image = shortcut->get_large_grid_image();
		if (image.is_valid()) {
			image->save_png(local_path);
		}
		
		local_path = grid_folder.path_join(itos(shortcut->appid) + "_logo.png");
		image = shortcut->get_logo_image();
		if (image.is_valid()) {
			image->save_png(local_path);
		}
	}
	shortcuts_bytes.append(0x08); // end of shortcuts dictionary
	shortcuts_bytes.append(0x08); // end of file

	if (!FileAccess::file_exists(filepath))
		DirAccess::make_dir_recursive_absolute(filepath.get_base_dir());
	Ref<FileAccess> file = FileAccess::open(filepath, FileAccess::WRITE);
	file->store_buffer(shortcuts_bytes);
}


void SteamShortcutWrite::save_images_of_shortcut(String grid_folder, Ref<SteamShortcut> p_shortcut){
	String local_path = grid_folder.path_join(itos(p_shortcut->appid) + "_hero");
	Error saving_error = p_shortcut->get_hero_image()->save_png(local_path + ".png");
	if (saving_error != OK) {
		ERR_PRINT_ED("There was a problem while saving one of the images :(");
	}

	local_path = grid_folder.path_join(itos(p_shortcut->appid) + "p");
	saving_error = p_shortcut->get_grid_image()->save_png(local_path + ".png");
	if (saving_error != OK) {
		ERR_PRINT_ED("There was a problem while saving one of the images :(");
	}


	local_path = grid_folder.path_join(itos(p_shortcut->appid));
	saving_error = p_shortcut->get_large_grid_image()->save_png(local_path + ".png");
	if (saving_error != OK) {
		ERR_PRINT_ED("There was a problem while saving one of the images :(");
	}

	local_path = grid_folder.path_join(itos(p_shortcut->appid) + "_logo");
	saving_error = p_shortcut->get_logo_image()->save_png(local_path + ".png");
	if (saving_error != OK) {
		ERR_PRINT_ED("There was a problem while saving one of the images :(");
	}

}