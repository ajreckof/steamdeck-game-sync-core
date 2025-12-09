#pragma once

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/wrapped.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/classes/http_request.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/shortcut.hpp>
#include <godot_cpp/variant/typed_array.hpp>

#include "steam_shortcuts.h"
#include "parallel_downloader.h"
#include "controler_icons_texture.h"

using namespace godot;


enum Devices {
	AUTO = -1,
	KEYBOARD_MOUSE,
	LUNA,
	OUYA,
	PS3,
	PS4,
	PS5,
	STADIA,
	STEAM,
	SWITCH,
	JOYCON,
	XBOX360,
	XBOXONE,
	XBOXSERIES,
	STEAM_DECK
};


class ControlerIconsButtons : public Node {
	GDCLASS(ControlerIconsButtons, Node)

	
	protected:
		static ControlerIconsButtons* controler_icons_buttons_singleton;
		static void _bind_methods();

		Devices last_controller;
		Devices fallback = Devices::XBOX360;
		bool show_button_for_mkb = true;
		Vector2 icon_size = Vector2(32, 32);
		
		Devices _get_joypad_type(int device);
		const String icon_folder_path = "res://addons/steamdeck-game-sync-core/asset/";
		static HashMap<Devices, HashMap<String, String>> _create_icon_path_remap_per_device(){
			HashMap<Devices, HashMap<String, String>> map;

			HashMap<String, String> ps3_map;
			ps3_map.insert("a", "cross");
			ps3_map.insert("b", "circle");
			ps3_map.insert("x", "square");
			ps3_map.insert("y", "triangle");
			ps3_map.insert("lb", "l1");
			ps3_map.insert("rb", "r1");
			ps3_map.insert("lt", "l2");
			ps3_map.insert("rt", "r2");
			map.insert(Devices::PS3, ps3_map);

			HashMap<String, String> ps4_map = ps3_map;
			ps4_map.insert("select", "share");
			ps4_map.insert("start", "options");
			ps4_map.insert("share", "");
			map.insert(Devices::PS4, ps4_map);

			HashMap<String, String> ps5_map = ps3_map;
			ps5_map.insert("select", "share");
			ps5_map.insert("start", "options");
			ps5_map.insert("home", "assistant");
			ps5_map.insert("share", "microphone");
			map.insert(Devices::PS5, ps5_map);

			HashMap<String, String> luna_map;
			luna_map.insert("select", "circle");
			luna_map.insert("start", "menu");
			luna_map.insert("share", "microphone");
			map.insert(Devices::LUNA, luna_map);

			HashMap<String, String> stadia_map;
			stadia_map.insert("lb", "l1");
			stadia_map.insert("rb", "r1");
			stadia_map.insert("lt", "l2");
			stadia_map.insert("rt", "r2");
			stadia_map.insert("select", "dots");
			stadia_map.insert("start", "menu");
			stadia_map.insert("share", "select");
			map.insert(Devices::STADIA, stadia_map);

			HashMap<String, String> steam_map;
			steam_map.insert("r_stick_click", "right_track_center");
			steam_map.insert("select", "back");
			steam_map.insert("home", "system");
			steam_map.insert("dpad", "left_track");
			steam_map.insert("dpad_up", "left_track_up");
			steam_map.insert("dpad_down", "left_track_down");
			steam_map.insert("dpad_left", "left_track_left");
			steam_map.insert("dpad_right", "left_track_right");
			steam_map.insert("l_stick", "stick");
			steam_map.insert("r_stick", "right_track");
			map.insert(Devices::STEAM, steam_map);

			HashMap<String, String> steam_deck_map;
			steam_deck_map.insert("lb", "l1");
			steam_deck_map.insert("rb", "r1");
			steam_deck_map.insert("lt", "l2");
			steam_deck_map.insert("rt", "r2");
			steam_deck_map.insert("select", "inventory");
			steam_deck_map.insert("start", "menu");
			steam_deck_map.insert("home", "steam");
			steam_deck_map.insert("share", "dots");
			map.insert(Devices::STEAM_DECK, steam_deck_map);

			HashMap<String, String> switch_map;
			switch_map.insert("a", "b");
			switch_map.insert("b", "a");
			switch_map.insert("x", "y");
			switch_map.insert("y", "x");
			switch_map.insert("lb", "l");
			switch_map.insert("rb", "r");
			switch_map.insert("lt", "zl");
			switch_map.insert("rt", "zr");
			switch_map.insert("select", "minus");
			switch_map.insert("start", "plus");
			switch_map.insert("share", "square");
			map.insert(Devices::SWITCH, switch_map);

			HashMap<String, String> joycon_map = switch_map;
			joycon_map.insert("dpad_up", "up");
			joycon_map.insert("dpad_down", "down");
			joycon_map.insert("dpad_left", "left");
			joycon_map.insert("dpad_right", "right");
			map.insert(Devices::JOYCON, joycon_map);

			HashMap<String, String> xbox360_map;
			xbox360_map.insert("select", "back");
			map.insert(Devices::XBOX360, xbox360_map);

			HashMap<String, String> xboxone_map;
			xboxone_map.insert("select", "view");
			xboxone_map.insert("start", "menu");
			map.insert(Devices::XBOXONE, xboxone_map);
			map.insert(Devices::XBOXSERIES, xboxone_map);

			HashMap<String, String> ouya_map;
			ouya_map.insert("a", "o");
			ouya_map.insert("x", "u");
			ouya_map.insert("b", "a");
			ouya_map.insert("lb", "l1");
			ouya_map.insert("rb", "r1");
			ouya_map.insert("lt", "l2");
			ouya_map.insert("rt", "r2");
			ouya_map.insert("start", "menu");
			ouya_map.insert("share", "microphone");
			map.insert(Devices::OUYA, ouya_map);
			
			return map;


		}
		const HashMap<Devices, HashMap<String, String>> icon_path_remap_per_device = _create_icon_path_remap_per_device();
		static HashMap<Devices, String> _create_device_folder_name() {
			HashMap<Devices, String> map;
			map.insert(Devices::KEYBOARD_MOUSE, "");
			map.insert(Devices::LUNA, "luna");
			map.insert(Devices::OUYA, "ouya");
			map.insert(Devices::PS3, "ps3");
			map.insert(Devices::PS4, "ps4");
			map.insert(Devices::PS5, "ps5");
			map.insert(Devices::STADIA, "stadia");
			map.insert(Devices::STEAM, "steam");
			map.insert(Devices::SWITCH, "switch");
			map.insert(Devices::JOYCON, "joycon");
			map.insert(Devices::XBOX360, "xbox360");
			map.insert(Devices::XBOXONE, "xboxone");
			map.insert(Devices::XBOXSERIES, "xboxseries");
			map.insert(Devices::STEAM_DECK, "steamdeck");
			return map;
		}
		const HashMap<Devices, String> device_folder_name = _create_device_folder_name();

		String _convert_path_for_device(String base_path, Devices device);
		String _convert_event_to_path(const Ref<InputEvent> event, Devices controller, Devices forced_controller_icon_style);
		String _convert_key_to_path(Key scancode);

		String _convert_mouse_button_to_path(MouseButton button_index);

		String _convert_joypad_button_to_path(JoyButton button_index);

		String _convert_joypad_motion_to_path(JoyAxis axis);
		void _set_last_input_type(Devices p_new_controller);
		Vector<ControlerIconsTexture *> registered_textures;
		void _on_node_added(Node *node, bool look_for_children = true);

	public:
		ControlerIconsButtons();
		~ControlerIconsButtons() override = default;
		Ref<Texture2D> get_icon_from_events(TypedArray<InputEvent> events);
		void update_all_textures();
		void register_controller_texture(ControlerIconsTexture *texture);
		void unregister_controller_texture(ControlerIconsTexture *texture);
		static ControlerIconsButtons *get_singleton() { return controler_icons_buttons_singleton; }
		static void initialize_singleton() { controler_icons_buttons_singleton = memnew(ControlerIconsButtons);};
		static void cleanup_singleton() { controler_icons_buttons_singleton = nullptr;};
		void add_singleton_to_scene_tree();

		Vector2 get_icon_size() const { return icon_size; }
		void set_icon_size(const Vector2 &p_icon_size) { icon_size = p_icon_size; }

		virtual void _input(const Ref<InputEvent> &p_event) override;
};
