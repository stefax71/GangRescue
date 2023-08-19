#pragma once

#include "../rdr2.h"

namespace ui
{
    // void ShowHelp(const std::string& text_to_show, int duration);
    void display_help_text(const char* text, int duration);
    
    struct UIHelpTextParam1
    {
        alignas(8) int duration;
        alignas(8) int unknown1;
        alignas(8) int unknown2;
        alignas(8) int unknown3;
    };

    struct UIHelpTextParam2
    {
        alignas(8) int unknown1;
        alignas(8) const char* help_text;
    };

    class HudDisplay
    {
    private:
        HudDisplay() = default;
        HudDisplay(const HudDisplay &other) = delete;
        void operator=(const HudDisplay &other) = delete;
        
    public:
        static HudDisplay& get_instance()
        {
            static HudDisplay instance;
            return  instance;
        }


        /**
         * \brief You can display any help in the status bar. You can color the text with
         *COLOR_RED
         * Example:
         * ui_display->ShowSubtitle("Hello, ~COLOR_YELLOW~yellow~COLOR_WHITE~ subtitle or ~COLOR_RED~subtitle~COLOR_WHITE~ or maybe ~COLOR_BLUE~blue~COLOR_WHITE~!");
         * \param text_to_show 
         */
        
        static void show_subtitle(const char* text_to_show);
        static void show_subtitle(std::string& text_to_show);
        
    };

    inline void DrawText(const float x, const float y, char *str)
    {
        HUD::_DISPLAY_TEXT(MISC::VAR_STRING(10, "LITERAL_STRING", str), x, y);
    }
    
}
