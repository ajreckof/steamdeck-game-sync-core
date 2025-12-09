#include <godot_cpp/classes/font.hpp>
#include <godot_cpp/classes/style_box_flat.hpp>
#include <godot_cpp/classes/panel_container.hpp>
#include <godot_cpp/classes/v_scroll_bar.hpp>
#include <godot_cpp/classes/h_scroll_bar.hpp>

#include "suggestion_popup.h"




void SuggestionPopup::_bind_methods() {
    ClassDB::bind_method(D_METHOD("clear_suggestions"), &SuggestionPopup::clear_suggestions);
    ClassDB::bind_method(D_METHOD("add_suggestion", "suggestion"), &SuggestionPopup::add_suggestion);
    ClassDB::bind_method(D_METHOD("set_suggestions", "suggestions"), &SuggestionPopup::set_suggestions);
    ClassDB::bind_method(D_METHOD("get_suggestions"), &SuggestionPopup::get_suggestions);
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "suggestions"), "set_suggestions", "get_suggestions");

    ClassDB::bind_method(D_METHOD("set_current_index", "index"), &SuggestionPopup::set_current_index);
    ClassDB::bind_method(D_METHOD("get_current_index"), &SuggestionPopup::get_current_index);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "current_index"), "set_current_index", "get_current_index");

    ClassDB::bind_method(D_METHOD("select_next"), &SuggestionPopup::select_next);
    ClassDB::bind_method(D_METHOD("select_previous"), &SuggestionPopup::select_previous);
    ClassDB::bind_method(D_METHOD("get_current_suggestion"), &SuggestionPopup::get_current_suggestion);
}

void SuggestionPopup::_ensure_rect_visible(Rect2 p_rect) {
    VScrollBar *v_scroll = scroll_container->get_v_scroll_bar();
    HScrollBar *h_scroll = scroll_container->get_h_scroll_bar();
    float side_margin = v_scroll->is_visible() ? v_scroll->get_size().x : 0.0f;
	float bottom_margin = h_scroll->is_visible() ? h_scroll->get_size().y : 0.0f;

	Vector2 min_scroll_pos = Vector2(
        p_rect.position.x + p_rect.size.x - scroll_container->get_size().x + side_margin,
        p_rect.position.y + p_rect.size.y - scroll_container->get_size().y + bottom_margin
    );

    Vector2 max_scroll_pos = Vector2(
        p_rect.position.x - side_margin,
        p_rect.position.y - bottom_margin
    );
	scroll_container->set_h_scroll(CLAMP(scroll_container->get_h_scroll(), min_scroll_pos.x, max_scroll_pos.x));
	scroll_container->set_v_scroll(CLAMP(scroll_container->get_v_scroll(), min_scroll_pos.y, max_scroll_pos.y));
}

void SuggestionPopup::_draw_suggestions() {
    int font_size = get_theme_default_font_size();

	float display_width = control->get_size().width;

    int stop_index = MIN(suggestions.size(), max_visible_suggestions);
	// Loop through all suggestions and draw each.
    Vector2 item_pos(0, 0);
    RID ci = control->get_canvas_item();
    for (int idx = 0; idx < stop_index; idx++) {
        Ref<TextLine> text_line = text_lines[idx];
        text_line->clear();
        text_line->add_string(suggestions[idx], control->get_theme_default_font(), get_theme_default_font_size());
        text_line->set_width(display_width);
        float text_height = text_line->get_size().y;
        if (idx == current_index) {
            // Draw selection rectangle
            Rect2 selection_rect(0, item_pos.y, display_width, text_height * (1.0 + extra_inter_line_spacing));
            control->draw_rect(selection_rect, Color(1,1,1,0.3));
            _ensure_rect_visible(selection_rect);
        }
        text_line->draw(ci, item_pos + Vector2(0, text_height * extra_inter_line_spacing / 2), Color(1, 1, 1, 1));
        
        item_pos.y += text_line->get_size().y * (1.0 + extra_inter_line_spacing);
    }
	control->set_custom_minimum_size(Size2(0, item_pos.y));
    set_size(Vector2(get_size().x, MIN(item_pos.y + 16, max_size)));
}

void SuggestionPopup::clear_suggestions() {
    suggestions.clear();
    control->queue_redraw();
}

void SuggestionPopup::add_suggestion(const String &p_suggestion) {
    suggestions.push_back(p_suggestion);
    control->queue_redraw();
}

void SuggestionPopup::set_suggestions(const PackedStringArray &p_suggestions) {
	suggestions = p_suggestions;
    control->queue_redraw();
}

PackedStringArray SuggestionPopup::get_suggestions() const {
	return suggestions;
}

void SuggestionPopup::select_next() {
    if (current_index + 1 >= suggestions.size()) {
        set_current_index(0);
    } else {
        set_current_index(current_index + 1);
    }
}

void SuggestionPopup::select_previous() {
    if (current_index - 1 < 0) {
        set_current_index(suggestions.size() - 1);
    } else {
        set_current_index(current_index - 1);
    }
}

String SuggestionPopup::get_current_suggestion() const {
    if (current_index < 0 || current_index >= suggestions.size()) {
        return String();
    }
	return suggestions[current_index];
}

void SuggestionPopup::set_current_index(int p_index) {
    if (!suggestions_available){
        return;
    }
    current_index = p_index;
    control->queue_redraw();
}

int SuggestionPopup::get_current_index() const {
	return current_index;
}

void SuggestionPopup::set_max_visible_suggestions(int p_max) {
    max_visible_suggestions = p_max;
    text_lines.resize(max_visible_suggestions);
    for (int i = 0; i < max_visible_suggestions; i++) {
        if (!text_lines[i].is_valid()) {
            text_lines.set(i, Ref<TextLine>(memnew(TextLine)));
        } 
    }
    control->queue_redraw();
    
}



void SuggestionPopup::set_extra_inter_line_spacing(float p_spacing){
    extra_inter_line_spacing = p_spacing;
    control->queue_redraw();
}

float SuggestionPopup::get_extra_inter_line_spacing() const {
	return extra_inter_line_spacing;
}

void SuggestionPopup::set_max_size(float p_size) {
    max_size = p_size;
    set_size(Vector2(get_size().x, MIN(control->get_size().y + 16, max_size)));

}

float SuggestionPopup::get_max_size() const {
	return max_size;
}

void SuggestionPopup::set_suggestions_available(bool p_available) {
    suggestions_available = p_available;
    current_index = p_available ? 0 : -1;
    queue_redraw();
}

bool SuggestionPopup::are_suggestions_available() const {
	return suggestions_available;
}

SuggestionPopup::SuggestionPopup() {
    
    
    Ref<StyleBoxFlat> style_box = memnew(StyleBoxFlat);
    style_box->set_border_width_all(4);
    style_box->set_border_width(SIDE_TOP, 0);
    style_box->set_corner_radius(CORNER_BOTTOM_LEFT, 4);
    style_box->set_corner_radius(CORNER_BOTTOM_RIGHT, 4);
    style_box->set_content_margin_all(8);
    style_box->set_bg_color(Color(0.2,0.2,0.2,1));
    style_box->set_border_color(Color(0.5,0.5,0.5,1));
    style_box->set_border_blend(true);
    add_theme_stylebox_override("panel", style_box);

    
    scroll_container = memnew(ScrollContainer);
    scroll_container->set_horizontal_scroll_mode(ScrollContainer::SCROLL_MODE_DISABLED);
    add_child(scroll_container);
    
    control = memnew(Control);
    control->connect("draw", callable_mp(this, &SuggestionPopup::_draw_suggestions));
    control->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    scroll_container->add_child(control);
    
    set_max_visible_suggestions(max_visible_suggestions);
}
