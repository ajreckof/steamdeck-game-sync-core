#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/json.hpp>
#include <godot_cpp/classes/image_texture.hpp>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/input_event_joypad_button.hpp>
#include <godot_cpp/classes/input_event_joypad_motion.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/accept_dialog.hpp>
#include <godot_cpp/classes/confirmation_dialog.hpp>
#include <godot_cpp/classes/button.hpp>


#ifdef UNIX_ENABLED
#include <unistd.h>
#endif


#include "controler_icons_buttons.h"
#include "controler_icons_texture_action.h"

ControlerIconsButtons *ControlerIconsButtons::controler_icons_buttons_singleton = nullptr;


void ControlerIconsButtons::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_icon_from_events", "events"), &ControlerIconsButtons::get_icon_from_events);
}

ControlerIconsButtons::ControlerIconsButtons() {
	
	callable_mp(this, &ControlerIconsButtons::add_singleton_to_scene_tree).call_deferred();
}

void ControlerIconsButtons::add_singleton_to_scene_tree() {
	
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
	if (!Engine::get_singleton()->is_editor_hint()){
		
		scene_tree->connect("node_added", callable_mp(this, &ControlerIconsButtons::_on_node_added).bind(false));
		_on_node_added(scene_tree->get_root());
	}
	
}


void ControlerIconsButtons::_set_last_input_type( Devices p_new_controller) {
	if (last_controller == p_new_controller) {
		return;
	}
	
	last_controller = p_new_controller;
	update_all_textures();
}

void ControlerIconsButtons::_on_node_added(Node *node, bool look_for_children) {
	
	AcceptDialog *accept_dialog = Object::cast_to<AcceptDialog>(node);
	if (accept_dialog) {
		
		Button *ok_button = accept_dialog->get_ok_button();
		if (!ok_button->get_button_icon().is_valid()) {
			
			Ref<ControlerIconsTextureAction> texture = memnew(ControlerIconsTextureAction);
			texture->set_action("ui_accept");
			ok_button->set_button_icon(texture);
		} else {
			
		}
	}

	ConfirmationDialog *confirmation_dialog = Object::cast_to<ConfirmationDialog>(node);
	if (confirmation_dialog) {
		
		Button *cancel_button = confirmation_dialog->get_cancel_button();
		if (!cancel_button->get_button_icon().is_valid()) {
			Ref<ControlerIconsTextureAction> texture = memnew(ControlerIconsTextureAction);
			texture->set_action("ui_cancel");
			cancel_button->set_button_icon(texture);
		}
	}

	Window *window = Object::cast_to<Window>(node);
	if (window) {
		window->connect("window_input", callable_mp(this, &ControlerIconsButtons::_input));
	}

	if (look_for_children) {
		for (int i = 0; i < node->get_child_count(); i++) {
			_on_node_added(node->get_child(i));
		}
	}
}

void ControlerIconsButtons::_input(const Ref<InputEvent> &p_event) {
	if (!Engine::get_singleton()->is_editor_hint()) {
		
	}
	Devices controller = last_controller;
	Ref<InputEventKey> key_event = p_event;
	Ref<InputEventMouseButton> mouse_button_event = p_event;
	Ref<InputEventMouseMotion> mouse_motion_event = p_event;
	Ref<InputEventJoypadButton> joypad_button_event = p_event;
	Ref<InputEventJoypadMotion> joypad_motion_event = p_event;
	if (key_event.is_valid() || mouse_button_event.is_valid()) {
		controller = Devices::KEYBOARD_MOUSE;
	} else if (mouse_motion_event.is_valid())
	{
		if (p_event->get_device() != -1) {
			controller = Devices::KEYBOARD_MOUSE;
		}
	} else if (joypad_button_event.is_valid()){
		controller = _get_joypad_type(joypad_button_event->get_device());
	} else if (joypad_motion_event.is_valid()) {
		controller = _get_joypad_type(joypad_motion_event->get_device());
	}
	
	_set_last_input_type(controller);
}

Devices ControlerIconsButtons::_get_joypad_type(int device) {
	TypedArray<int32_t> available = Input::get_singleton()->get_connected_joypads();
	if (available.is_empty()) {
		return fallback;
	}
	// If the requested joypad is not on the connected joypad list, try using the last known connected joypad
	if (!available.has(device)) {
		device = available.front();
	}

	String controller_name = Input::get_singleton()->get_joy_name(device);
	if (controller_name.containsn("Luna")) {
		return Devices::LUNA;
	} else if (controller_name.containsn("PS3")) {
		return Devices::PS3;
	} else if (controller_name.containsn("PS4") || controller_name.containsn("DUALSHOCK 4")) {
		return Devices::PS4;
	} else if (controller_name.containsn("PS5") || controller_name.containsn("DualSense")) {
		return Devices::PS5;
	} else if (controller_name.containsn("Stadia")) {
		return Devices::STADIA;
	} else if (controller_name.containsn("Steam")) {
		return Devices::STEAM;
	} else if (controller_name.containsn("Switch")) {
		return Devices::SWITCH;
	} else if (controller_name.containsn("Joy-Con")) {
		return Devices::JOYCON;
	} else if (controller_name.containsn("Xbox 360")) {
		return Devices::XBOX360;
	} else if (controller_name.containsn("Xbox One") || \
			controller_name.containsn("X-Box One") || \
			controller_name.containsn("Xbox Wireless")) {
		return Devices::XBOXONE;
	} else if (controller_name.containsn("Xbox Series") || \
			controller_name.containsn("X-Box Series")) {
		return Devices::XBOXSERIES;
	} else if (controller_name.containsn("Steam")) {
		return Devices::STEAM_DECK;
	} else if (controller_name.containsn("OUYA Controller")) {
		return Devices::OUYA;
	} else {
		return fallback;
	}
}

Ref<Texture2D> ControlerIconsButtons::get_icon_from_events(TypedArray<InputEvent> events) {
	if (events.is_empty()) {
		return nullptr;
	}
	if (last_controller == Devices::KEYBOARD_MOUSE ){
		if (!show_button_for_mkb) {
			return nullptr;
		}

		Ref<InputEventKey> key_event;
		Ref<InputEventMouseButton> mouse_button_event;
		Ref<InputEventMouseMotion> mouse_motion_event;

		for (int i = 0; i < events.size(); i++) {
			key_event = events[i];
			mouse_button_event = events[i];
			mouse_motion_event = events[i];
			if (key_event.is_valid() || mouse_button_event.is_valid() || mouse_motion_event.is_valid()) {
				
				String path = _convert_event_to_path(events[i], last_controller, Devices::AUTO);
				
				if (path.is_empty()) {
					continue;
				}
				String full_path = _convert_path_for_device(path, Devices::KEYBOARD_MOUSE);
				
				if (full_path.is_empty()) {
					continue;
				}
				return ResourceLoader::get_singleton()->load(full_path);
			}
		}
	} else {
		Ref<InputEventJoypadButton> joypad_button_event;
		Ref<InputEventJoypadMotion> joypad_motion_event;
		for (int i = 0; i < events.size(); i++) {
			joypad_button_event = events[i];
			joypad_motion_event = events[i];
			if (joypad_button_event.is_valid() || joypad_motion_event.is_valid()) {
				String path = _convert_event_to_path(events[i], last_controller, Devices::AUTO);
				if (path.is_empty()) {
					continue;
				}
				String converted_path = _convert_path_for_device(path, last_controller);
				if (converted_path.is_empty()) {
					continue;
				}
				return ResourceLoader::get_singleton()->load(converted_path);
			}
		}
	}

	return nullptr;


}

void ControlerIconsButtons::update_all_textures() {
	for (int i = 0; i < registered_textures.size(); i++) {
		registered_textures[i]->update_texture();
	}
}

void ControlerIconsButtons::register_controller_texture(ControlerIconsTexture *texture) {
	
	registered_textures.append(texture);
}

void ControlerIconsButtons::unregister_controller_texture(ControlerIconsTexture *texture) {
	
	registered_textures.remove_at(registered_textures.find(texture));
}

String ControlerIconsButtons::_convert_path_for_device(String base_path, Devices device) {
	String folder = icon_folder_path.path_join(device_folder_name[device]);
	if (device == Devices::KEYBOARD_MOUSE) {
		// No remapping for keyboard and mouse
		return folder.path_join(base_path + ".png");
	}
	ERR_FAIL_COND_V_MSG(!icon_path_remap_per_device.has(device), folder.path_join(base_path+ ".png"), "No icon remap found for device enum value: " + itos(static_cast<int>(device)));
	const HashMap<String, String> &device_map = icon_path_remap_per_device[device];
	String converted_base_path = device_map.has(base_path) ? device_map[base_path] : base_path;
	
	if (converted_base_path.is_empty()) {
		return "";
	}
	return folder.path_join(converted_base_path) + ".png";
}

String ControlerIconsButtons::_convert_event_to_path(const Ref<InputEvent> event, Devices controller, Devices forced_controller_icon_style = Devices::AUTO) {
	if (forced_controller_icon_style != Devices::AUTO) {
		controller = forced_controller_icon_style;
	}

	Ref<InputEventKey> key_event = event;
	Ref<InputEventMouseButton> mouse_button_event = event;
	Ref<InputEventMouseMotion> mouse_motion_event = event;
	Ref<InputEventJoypadButton> joypad_button_event = event;
	Ref<InputEventJoypadMotion> joypad_motion_event = event;
	if (key_event.is_valid()) {
		// If this is a physical key, convert to localized scancode
		if (key_event->get_keycode() == 0) {
			
			
			return _convert_key_to_path(DisplayServer::get_singleton()->keyboard_get_keycode_from_physical(key_event->get_physical_keycode()));
		}
		return _convert_key_to_path(key_event->get_keycode());
	} else if (mouse_button_event.is_valid()) {
		return _convert_mouse_button_to_path(mouse_button_event->get_button_index());
	} else if (joypad_button_event.is_valid()) {
		return _convert_joypad_button_to_path(joypad_button_event->get_button_index());
	} else if (joypad_motion_event.is_valid()) {
		return _convert_joypad_motion_to_path(joypad_motion_event->get_axis());
	}
	return "";
}

String ControlerIconsButtons::_convert_key_to_path(Key scancode) {
	
	if (scancode == KEY_A) {
		
	}
	switch (scancode) {
		case KEY_ESCAPE:
			return "key/esc";
		case KEY_TAB:
			return "key/tab";
		case KEY_BACKSPACE:
			return "key/backspace_alt";
		case KEY_ENTER:
			return "key/enter_alt";
		case KEY_KP_ENTER:
			return "key/enter_tall";
		case KEY_INSERT:
			return "key/insert";
		case KEY_DELETE:
			return "key/del";
		case KEY_PRINT:
			return "key/print_screen";
		case KEY_HOME:
			return "key/home";
		case KEY_END:
			return "key/end";
		case KEY_LEFT:
			return "key/arrow_left";
		case KEY_UP:
			return "key/arrow_up";
		case KEY_RIGHT:
			return "key/arrow_right";
		case KEY_DOWN:
			return "key/arrow_down";
		case KEY_PAGEUP:
			return "key/page_up";
		case KEY_PAGEDOWN:
			return "key/page_down";
		case KEY_SHIFT:
			return "key/shift_alt";
		case KEY_CTRL:
			return "key/ctrl";
		case KEY_META:
			if (OS::get_singleton()->get_name() == "macOS") {
				return "key/command";
			} else {
				return "key/meta";
			}
		case KEY_ALT:
			return "key/alt";
		case KEY_CAPSLOCK:
			return "key/caps_lock";
		case KEY_NUMLOCK:
			return "key/num_lock";
		case KEY_F1:
			return "key/f1";
		case KEY_F2:
			return "key/f2";
		case KEY_F3:
			return "key/f3";
		case KEY_F4:
			return "key/f4";
		case KEY_F5:
			return "key/f5";
		case KEY_F6:
			return "key/f6";
		case KEY_F7:
			return "key/f7";
		case KEY_F8:
			return "key/f8";
		case KEY_F9:
			return "key/f9";
		case KEY_F10:
			return "key/f10";
		case KEY_F11:
			return "key/f11";
		case KEY_F12:
			return "key/f12";
		case KEY_KP_MULTIPLY:
		case KEY_ASTERISK:
			return "key/asterisk";
		case KEY_KP_SUBTRACT:
		case KEY_MINUS:
			return "key/minus";
		case KEY_KP_ADD:
			return "key/plus_tall";
		case KEY_KP_0:
			return "key/0";
		case KEY_KP_1:
			return "key/1";
		case KEY_KP_2:
			return "key/2";
		case KEY_KP_3:
			return "key/3";
		case KEY_KP_4:
			return "key/4";
		case KEY_KP_5:
			return "key/5";
		case KEY_KP_6:
			return "key/6";
		case KEY_KP_7:
			return "key/7";
		case KEY_KP_8:
			return "key/8";
		case KEY_KP_9:
			return "key/9";
		case KEY_UNKNOWN:
			return "";
		case KEY_SPACE:
			return "key/space";
		case KEY_QUOTEDBL:
			return "key/quote";
		case KEY_PLUS:
			return "key/plus";
		case KEY_0:
			return "key/0";
		case KEY_1:
			return "key/1";
		case KEY_2:
			return "key/2";
		case KEY_3:
			return "key/3";
		case KEY_4:
			return "key/4";
		case KEY_5:
			return "key/5";
		case KEY_6:
			return "key/6";
		case KEY_7:
			return "key/7";
		case KEY_8:
			return "key/8";
		case KEY_9:
			return "key/9";
		case KEY_SEMICOLON:
			return "key/semicolon";
		case KEY_LESS:
			return "key/mark_left";
		case KEY_GREATER:
			return "key/mark_right";
		case KEY_QUESTION:
			return "key/question";
		case KEY_A:
			return "key/a";
		case KEY_B:
			return "key/b";
		case KEY_C:
			return "key/c";
		case KEY_D:
			return "key/d";
		case KEY_E:
			return "key/e";
		case KEY_F:
			return "key/f";
		case KEY_G:
			return "key/g";
		case KEY_H:
			return "key/h";
		case KEY_I:
			return "key/i";
		case KEY_J:
			return "key/j";
		case KEY_K:
			return "key/k";
		case KEY_L:
			return "key/l";
		case KEY_M:
			return "key/m";
		case KEY_N:
			return "key/n";
		case KEY_O:
			return "key/o";
		case KEY_P:
			return "key/p";
		case KEY_Q:
			return "key/q";
		case KEY_R:
			return "key/r";
		case KEY_S:
			return "key/s";
		case KEY_T:
			return "key/t";
		case KEY_U:
			return "key/u";
		case KEY_V:
			return "key/v";
		case KEY_W:
			return "key/w";
		case KEY_X:
			return "key/x";
		case KEY_Y:
			return "key/y";
		case KEY_Z:
			return "key/z";
		case KEY_BRACKETLEFT:
			return "key/bracket_left";
		case KEY_BACKSLASH:
			return "key/slash";
		case KEY_SLASH:
			return "key/forward_slash";
		case KEY_BRACKETRIGHT:
			return "key/bracket_right";
		case KEY_ASCIITILDE:
			return "key/tilda";
		case KEY_QUOTELEFT:
			return "key/backtick";
		case KEY_APOSTROPHE:
			return "key/apostrophe";
		case KEY_COMMA:
			return "key/comma";
		case KEY_EQUAL:
			return "key/equals";
		case KEY_PERIOD:
		case KEY_KP_PERIOD:
			return "key/period";
		default:
			return "";
	}
}

String ControlerIconsButtons::_convert_mouse_button_to_path(MouseButton button_index) {
	switch (button_index) {
		case MOUSE_BUTTON_LEFT:
			return "mouse/left";
		case MOUSE_BUTTON_RIGHT:
			return "mouse/right";
		case MOUSE_BUTTON_MIDDLE:
			return "mouse/middle";
		case MOUSE_BUTTON_WHEEL_UP:
			return "mouse/wheel_up";
		case MOUSE_BUTTON_WHEEL_DOWN:
			return "mouse/wheel_down";
		case MOUSE_BUTTON_XBUTTON1:
			return "mouse/side_down";
		case MOUSE_BUTTON_XBUTTON2:
			return "mouse/side_up";
		default:
			return "mouse/sample";
	}
}

String ControlerIconsButtons::_convert_joypad_button_to_path(JoyButton button_index) {
	switch (button_index) {
		case JOY_BUTTON_A:
			return "a";
		case JOY_BUTTON_B:
			return "b";
		case JOY_BUTTON_X:
			return "x";
		case JOY_BUTTON_Y:
			return "y";
		case JOY_BUTTON_LEFT_SHOULDER:
			return "lb";
		case JOY_BUTTON_RIGHT_SHOULDER:
			return "rb";
		case JOY_BUTTON_LEFT_STICK:
			return "l_stick_click";
		case JOY_BUTTON_RIGHT_STICK:
			return "r_stick_click";
		case JOY_BUTTON_BACK:
			return "select";
		case JOY_BUTTON_START:
			return "start";
		case JOY_BUTTON_DPAD_UP:
			return "dpad_up";
		case JOY_BUTTON_DPAD_DOWN:
			return "dpad_down";
		case JOY_BUTTON_DPAD_LEFT:
			return "dpad_left";
		case JOY_BUTTON_DPAD_RIGHT:
			return "dpad_right";
		case JOY_BUTTON_GUIDE:
			return "home";
		case JOY_BUTTON_MISC1:
			return "share";
		default:
			return "";
	}
}

String ControlerIconsButtons::_convert_joypad_motion_to_path(JoyAxis axis) {
	switch(axis) {
		case JOY_AXIS_LEFT_Y:
		case JOY_AXIS_LEFT_X:
			return "l_stick";
		case JOY_AXIS_RIGHT_Y:
		case JOY_AXIS_RIGHT_X:
			return "r_stick";
		case JOY_AXIS_TRIGGER_LEFT:
			return "lt";
		case JOY_AXIS_TRIGGER_RIGHT:
			return "rt";
		default:
			return "";
	}
}

