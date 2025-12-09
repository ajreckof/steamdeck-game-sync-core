#pragma once

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/panel_container.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/text_line.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>
#include <godot_cpp/classes/scroll_container.hpp>

using namespace godot;

class SuggestionPopup : public PanelContainer {
    GDCLASS(SuggestionPopup, PanelContainer)

    public:
        PackedStringArray suggestions;
        int current_index = -1;
        int max_visible_suggestions = 10;
        float extra_inter_line_spacing = .25f;
        bool suggestions_available = false;
        ScrollContainer *scroll_container = nullptr;


        
        void select_next();
        void select_previous();
        String get_current_suggestion() const;
        
    protected:
        static void _bind_methods();
        void _ensure_rect_visible(Rect2 p_rect);
    private:
        Control *control = nullptr;
        Vector<Ref<TextLine>> text_lines;
        float max_size = 200.;

        void _draw_suggestions();
    
    public:
        void clear_suggestions();
        void add_suggestion(const String &p_suggestion);
        void set_suggestions(const PackedStringArray &p_suggestions);
        PackedStringArray get_suggestions() const;

        void set_current_index(int p_index);
        int get_current_index() const;

        void set_max_visible_suggestions(int p_max);
        int get_max_visible_suggestions() const;

        void set_extra_inter_line_spacing(float p_spacing);
        float get_extra_inter_line_spacing() const;

        void set_max_size(float p_size);
        float get_max_size() const;

        void set_suggestions_available(bool p_available);
        bool are_suggestions_available() const;

        SuggestionPopup();
};


