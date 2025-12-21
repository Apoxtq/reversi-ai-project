/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * UI Style Configuration Implementation
 */

#include "UIStyle.hpp"
#include <SFML/Graphics/Font.hpp>
#include <stdexcept>

namespace reversi {
namespace ui {
namespace UIStyle {

sf::Font load_font(const std::string& font_path) {
    sf::Font font;
    
    // Try to load from path if provided
    if (!font_path.empty()) {
        try {
            if (font.loadFromFile(font_path)) {
                return font;
            }
        } catch (...) {
            // Font loading failed, continue to fallback
        }
    }
    
    // Fallback: Try system fonts (Windows)
    #ifdef _WIN32
    const char* system_fonts[] = {
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/calibri.ttf",
        "C:/Windows/Fonts/segoeui.ttf",
        "C:/Windows/Fonts/tahoma.ttf"
    };
    
    for (const char* system_font : system_fonts) {
        try {
            if (font.loadFromFile(system_font)) {
                return font;
            }
        } catch (...) {
            // Continue to next font
        }
    }
    #endif
    
    // Return empty font - caller should check font.getInfo().family.empty()
    return font;
}

} // namespace UIStyle
} // namespace ui
} // namespace reversi

