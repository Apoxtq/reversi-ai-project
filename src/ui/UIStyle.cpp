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
    if (!font_path.empty() && font.loadFromFile(font_path)) {
        return font;
    }
    
    // Fallback: Try system fonts or default
    // For now, return empty font (SFML will use default)
    // In production, would try multiple fallback paths
    return font;
}

} // namespace UIStyle
} // namespace ui
} // namespace reversi

