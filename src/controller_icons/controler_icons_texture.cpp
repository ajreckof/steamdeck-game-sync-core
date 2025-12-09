#include "controler_icons_texture.h"
#include "controler_icons_buttons.h"

void ControlerIconsTexture::_bind_methods() {
}



ControlerIconsTexture::ControlerIconsTexture() {
    ControlerIconsButtons::get_singleton()->register_controller_texture(this);
}

ControlerIconsTexture::~ControlerIconsTexture() {
    ControlerIconsButtons::get_singleton()->unregister_controller_texture(this);
}

void ControlerIconsTexture::update_texture() const {
    Ref<Texture2D> previous_texture = _last_texture;
    const_cast<ControlerIconsTexture*>(this)->_update_texture();
    if (previous_texture != _last_texture) {
        const_cast<ControlerIconsTexture*>(this)->emit_changed();
    }
}

int32_t ControlerIconsTexture::_get_width() const {
	update_texture();
    Vector2 icon_size = ControlerIconsButtons::get_singleton()->get_icon_size();
    if (icon_size.length_squared() > 0) {
        return icon_size.x;
    }
    return _last_texture.is_valid() ? _last_texture->get_width() : 0;
}

int32_t ControlerIconsTexture::_get_height() const {
    update_texture();
    Vector2 icon_size = ControlerIconsButtons::get_singleton()->get_icon_size();
    if (icon_size.length_squared() > 0) {
        return icon_size.y;
    }
    return _last_texture.is_valid() ? _last_texture->get_height() : 0;
}

bool ControlerIconsTexture::_is_pixel_opaque(int32_t p_x, int32_t p_y) const {
    update_texture();
    return _last_texture.is_valid() ? _last_texture->_is_pixel_opaque(p_x, p_y) : false;
}

bool ControlerIconsTexture::_has_alpha() const {
    update_texture();
    return _last_texture.is_valid() ? _last_texture->has_alpha() : false;
}

void ControlerIconsTexture::_draw(const RID &p_to_canvas_item, const Vector2 &p_pos, const Color &p_modulate, bool p_transpose) const {
    
    update_texture();
    if (_last_texture.is_valid()) {
        _last_texture->draw(p_to_canvas_item, p_pos, p_modulate, p_transpose);
    }
}

void ControlerIconsTexture::_draw_rect(const RID &p_to_canvas_item, const Rect2 &p_rect, bool p_tile, const Color &p_modulate, bool p_transpose) const {
    
    update_texture();
    if (_last_texture.is_valid()) {
        _last_texture->draw_rect(p_to_canvas_item, p_rect, p_tile, p_modulate, p_transpose);
    } else {
        
    }
}

void ControlerIconsTexture::_draw_rect_region(const RID &p_to_canvas_item, const Rect2 &p_rect, const Rect2 &p_src_rect, const Color &p_modulate, bool p_transpose, bool p_clip_uv) const {
    
    update_texture();
    if (_last_texture.is_valid()) {
        _last_texture->draw_rect_region(p_to_canvas_item, p_rect, p_src_rect, p_modulate, p_transpose, p_clip_uv);
    } else {
        
    }
}
