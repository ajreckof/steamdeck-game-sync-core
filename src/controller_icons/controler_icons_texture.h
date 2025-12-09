#pragma once
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/classes/input_event.hpp>


using namespace godot;

class ControlerIconsTexture : public Texture2D {
    GDCLASS(ControlerIconsTexture, Texture2D);

protected:
    static void _bind_methods();
    Ref<Texture2D> _last_texture;
public:
    ControlerIconsTexture();
    ~ControlerIconsTexture();

    void update_texture() const;
    virtual void _update_texture() {};

    virtual int32_t _get_width() const override;
	virtual int32_t _get_height() const override;
	virtual bool _is_pixel_opaque(int32_t p_x, int32_t p_y) const override;
	virtual bool _has_alpha() const override;
	virtual void _draw(const RID &p_to_canvas_item, const Vector2 &p_pos, const Color &p_modulate, bool p_transpose) const override;
	virtual void _draw_rect(const RID &p_to_canvas_item, const Rect2 &p_rect, bool p_tile, const Color &p_modulate, bool p_transpose) const override;
	virtual void _draw_rect_region(const RID &p_to_canvas_item, const Rect2 &p_rect, const Rect2 &p_src_rect, const Color &p_modulate, bool p_transpose, bool p_clip_uv) const override;

};