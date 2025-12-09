#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/engine.hpp>

#include "controller_icons/controler_icons_buttons.h"
#include "controller_icons/controler_icons_texture.h"
#include "controller_icons/controler_icons_texture_action.h"
#include "steam_shortcuts.h"
#include "parallel_downloader.h"
#include "steamdeck_game_sync_core.h"
#include "suggestion_popup.h"
#include "suggestion_line_edit.h"

using namespace godot;

void initialize_gdextension_types(ModuleInitializationLevel p_level)
{
	switch(p_level) {
		case MODULE_INITIALIZATION_LEVEL_SCENE:
			GDREGISTER_CLASS(SteamShortcut);
			GDREGISTER_CLASS(ParallelDownloader);
			GDREGISTER_CLASS(SuggestionPopup);
			GDREGISTER_CLASS(SuggestionLineEdit);

			GDREGISTER_CLASS(SteamDeckGameSyncCore);
			SteamDeckGameSyncCore::initialize_singleton();
			Engine::get_singleton()->register_singleton("SteamDeckGameSyncCore", SteamDeckGameSyncCore::get_singleton());
			
			GDREGISTER_CLASS(ControlerIconsButtons);
			ControlerIconsButtons::initialize_singleton();
			Engine::get_singleton()->register_singleton("ControlerIconsButtons", ControlerIconsButtons::get_singleton());

			GDREGISTER_CLASS(ControlerIconsTexture);
			GDREGISTER_CLASS(ControlerIconsTextureAction);
			
			break;
		case MODULE_INITIALIZATION_LEVEL_EDITOR:
			break;
		default:
			break;
	}
}

void uninitialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	SteamDeckGameSyncCore::cleanup_singleton();
	Engine::get_singleton()->unregister_singleton("SteamDeckGameSyncCore");
}

extern "C"
{
	// Initialization
	GDExtensionBool GDE_EXPORT steamdeck_game_sync_core_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization)
	{
		GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
		init_obj.register_initializer(initialize_gdextension_types);
		init_obj.register_terminator(uninitialize_gdextension_types);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}