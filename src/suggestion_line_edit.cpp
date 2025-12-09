#include "suggestion_line_edit.h"

void SuggestionLineEdit::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_popup_max_size", "size"), &SuggestionLineEdit::set_popup_max_size);
    ClassDB::bind_method(D_METHOD("get_popup_max_size"), &SuggestionLineEdit::get_popup_max_size);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "popup_max_size"), "set_popup_max_size", "get_popup_max_size");

    ClassDB::bind_method(D_METHOD("set_suggestions", "suggestions"), &SuggestionLineEdit::set_suggestions);
    ClassDB::bind_method(D_METHOD("set_placeholder_suggestion", "suggestion"), &SuggestionLineEdit::set_placeholder_suggestion);

    GDVIRTUAL_BIND(_generate_suggestions);
}

void SuggestionLineEdit::_on_edit(bool toggle) {
    if (toggle) {
        generate_suggestions();
        suggestion_popup->show();
        suggestion_popup->set_position(Vector2(0, get_size().y));
        suggestion_popup->set_max_size(popup_max_size * get_size().y);
        suggestion_popup->set_size(Vector2(get_size().x, suggestion_popup->get_size().y));
    } else {
        suggestion_popup->hide();
    }
}

void SuggestionLineEdit::_gui_input(const Ref<InputEvent> &p_event) {
    if (p_event->is_action_pressed("ui_down")) {
        suggestion_popup->select_next();
    } else if (p_event->is_action_pressed("ui_up")) {
        suggestion_popup->select_previous();
    } else if (p_event->is_action_pressed("ui_text_submit")) {
        String suggestion = suggestion_popup->get_current_suggestion();
        if (!suggestion.is_empty()) {
            set_text(suggestion);
        }
        emit_signal("text_submitted", get_text());
        suggestion_popup->hide();
        unedit();
    } else {
        return;
    }
    accept_event();


}

void SuggestionLineEdit::set_suggestions(PackedStringArray p_suggestions) {
    suggestion_popup->set_suggestions_available(true);
    suggestion_popup->set_suggestions(p_suggestions);
}

void SuggestionLineEdit::set_placeholder_suggestion(const String &p_suggestion) {
    suggestion_popup->clear_suggestions();
    suggestion_popup->set_suggestions_available(false);
    suggestion_popup->add_suggestion(p_suggestion);
}

void SuggestionLineEdit::generate_suggestions() {

    if (GDVIRTUAL_IS_OVERRIDDEN(_generate_suggestions)) {
        GDVIRTUAL_CALL(_generate_suggestions);
        return;
    }
    _generate_suggestions();

}

void SuggestionLineEdit::set_popup_max_size(float p_size) {
    popup_max_size = p_size;
    suggestion_popup->set_max_size(p_size * get_size().y);
}

SuggestionLineEdit::SuggestionLineEdit() {
    suggestion_popup = memnew(SuggestionPopup);
    suggestion_popup->hide();
    add_child(suggestion_popup);
    connect("text_changed", callable_mp(this, &SuggestionLineEdit::generate_suggestions).unbind(1));
    connect("editing_toggled", callable_mp(this, &SuggestionLineEdit::_on_edit));

}
