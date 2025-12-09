#pragma once
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

#include "suggestion_popup.h"
using namespace godot;
class SuggestionLineEdit : public LineEdit {
    GDCLASS(SuggestionLineEdit, LineEdit)

    protected:
        float popup_max_size = 5.0;
        SuggestionPopup *suggestion_popup = nullptr;
        static void _bind_methods();
        void _on_edit(bool toggle);
        virtual void _generate_suggestions() {};
        GDVIRTUAL0(_generate_suggestions);
        
    public:
        virtual void _gui_input(const Ref<InputEvent> &p_event) override;
        void set_suggestions(PackedStringArray p_suggestions);
        void set_placeholder_suggestion(const String &p_suggestion);
        void generate_suggestions();

        void set_popup_max_size(float p_size);
        float get_popup_max_size() const { return popup_max_size; }

        SuggestionLineEdit();
};