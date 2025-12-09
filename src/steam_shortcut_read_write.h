
#include "steam_shortcuts.h"

class SteamShortcutRead : public Object
{
	GDCLASS(SteamShortcutRead, Object);

	protected:
	static void _bind_methods();

	public:

	static TypedArray<SteamShortcut> read_shortcuts(String filepath);
	static Ref<SteamShortcut> parse_shortcut(PackedByteArray data, int &position);
	static PackedStringArray parse_tags(PackedByteArray data, int &position);
	static void skip_dictionary(PackedByteArray data, int &position);
	static void assign_string(Ref<SteamShortcut> sc, String key, String val);
	static void assign_int(Ref<SteamShortcut> sc, String key, uint32_t val);
	static String read_null_terminated_string(PackedByteArray reader, int &position);
};

class SteamShortcutWrite : public Object
{
	GDCLASS(SteamShortcutWrite, Object);

	protected:
	static void _bind_methods();

	public:

	static void write_shortcuts(String filepath, TypedArray<SteamShortcut> shortcuts);
	void save_images_of_shortcut(String grid_folder, Ref<SteamShortcut> p_shortcut);
	static PackedByteArray encode_u32_value(int value);
	static PackedByteArray encode_shortcut(Ref<SteamShortcut> sc);
	static PackedByteArray encode_string_field(String key, String value);
	static PackedByteArray encode_bool_field(String key, bool value);
	static PackedByteArray encode_int_field(String key, int value);
	static PackedByteArray encode_tags(PackedStringArray tags);
};

// Helper to write string fields

