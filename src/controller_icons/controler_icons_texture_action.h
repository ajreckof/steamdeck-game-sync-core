#pragma once
#include "controler_icons_texture.h"


using namespace godot;

class ControlerIconsTextureAction : public ControlerIconsTexture {
    GDCLASS(ControlerIconsTextureAction, ControlerIconsTexture);
public:
    String action;
protected:
    static void _bind_methods();
public:
    virtual void _update_texture() override;

    void set_action(const String &p_action);
    String get_action() const;
};