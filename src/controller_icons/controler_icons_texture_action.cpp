#include "controler_icons_texture_action.h"
#include "controler_icons_buttons.h"


#include <godot_cpp/classes/input_map.hpp>

void ControlerIconsTextureAction::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_action", "action"), &ControlerIconsTextureAction::set_action);
    ClassDB::bind_method(D_METHOD("get_action"), &ControlerIconsTextureAction::get_action);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "action"), "set_action", "get_action");
}

void ControlerIconsTextureAction::_update_texture() {
    InputMap::get_singleton()->load_from_project_settings();
	TypedArray<InputEvent> events = InputMap::get_singleton()->action_get_events(action);
    _last_texture = ControlerIconsButtons::get_singleton()->get_icon_from_events(events);
}

void ControlerIconsTextureAction::set_action(const String &p_action) {
    action = p_action;
    emit_changed();
}

String ControlerIconsTextureAction::get_action() const {
    return action;
}