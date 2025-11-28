/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * UI Style Configuration (Strategy Pattern)
 * Follows UI/UX Design Specification
 */

#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <string>

namespace reversi {
namespace ui {

/**
 * @brief UI Style configuration following UI/UX Design Specification
 * 
 * Implements Strategy Pattern for configurable UI styling.
 * All colors, fonts, and dimensions follow the design specification.
 */
namespace UIStyle {
    // ==================== Color System ====================
    // Follows UI_UX_Design_Specification.md Section 5.1
    
    // Background Layers
    inline constexpr sf::Color BACKGROUND_L1{0x1a, 0x1a, 0x1a};  // #1a1a1a - Base background
    inline constexpr sf::Color BACKGROUND_L2{0x25, 0x25, 0x25};  // #252525 - Panel background
    inline constexpr sf::Color BACKGROUND_L3{0x30, 0x30, 0x30};  // #303030 - Elevated panels
    
    // Board Colors
    inline constexpr sf::Color BOARD_DARK{0x2d, 0x5a, 0x3d};     // #2d5a3d - Dark green squares
    inline constexpr sf::Color BOARD_LIGHT{0x35, 0x70, 0x45};    // #357045 - Light green squares
    inline constexpr sf::Color BOARD_BORDER{0x1a, 0x3a, 0x2a};   // #1a3a2a - Grid lines
    
    // Player Colors
    inline constexpr sf::Color DISC_BLACK{0x1a, 0x1a, 0x1a};     // #1a1a1a - Black disc
    inline constexpr sf::Color DISC_WHITE{0xe8, 0xe8, 0xe8};     // #e8e8e8 - White disc
    
    // Accent Colors
    inline constexpr sf::Color ACCENT_GREEN{0x4a, 0x9d, 0x5f};   // #4a9d5f - Primary accent
    inline constexpr sf::Color SUCCESS{0x5c, 0xb8, 0x5c};         // #5cb85c - Success/confirmation
    inline constexpr sf::Color WARNING{0xf0, 0xad, 0x4e};         // #f0ad4e - Warnings/alerts
    inline constexpr sf::Color ERROR_RED{0xd9, 0x53, 0x4f};       // #d9534f - Errors/danger
    inline constexpr sf::Color INFO{0x5b, 0xc0, 0xde};           // #5bc0de - Informational
    
    // Text Colors
    inline constexpr sf::Color TEXT_PRIMARY{0xe0, 0xe0, 0xe0};    // #e0e0e0 - Main text
    inline constexpr sf::Color TEXT_SECONDARY{0xa0, 0xa0, 0xa0};  // #a0a0a0 - Subtitles
    inline constexpr sf::Color TEXT_DISABLED{0x60, 0x60, 0x60};   // #606060 - Inactive text
    inline constexpr sf::Color TEXT_HIGHLIGHT{0xff, 0xff, 0xff};   // #ffffff - Emphasized text
    
    // ==================== Typography System ====================
    // Follows UI_UX_Design_Specification.md Section 5.2
    
    // Font Family: Roboto (fallback to system fonts)
    inline constexpr const char* FONT_PRIMARY = "Roboto";
    inline constexpr const char* FONT_MONOSPACE = "Roboto Mono";
    
    // Type Scale (8px base unit)
    inline constexpr unsigned int FONT_H1 = 48;      // Page titles
    inline constexpr unsigned int FONT_H2 = 32;       // Section titles
    inline constexpr unsigned int FONT_H3 = 24;       // Subsections
    inline constexpr unsigned int FONT_H4 = 18;       // Labels
    inline constexpr unsigned int FONT_BODY = 16;     // Body text
    inline constexpr unsigned int FONT_SMALL = 14;    // Footer text
    inline constexpr unsigned int FONT_TINY = 12;     // Tooltips
    
    // Special Cases
    inline constexpr unsigned int FONT_BUTTON = 18;   // Button labels
    inline constexpr unsigned int FONT_ROOM_CODE = 72; // Room codes (monospace)
    inline constexpr unsigned int FONT_SCORE = 64;     // Score numbers
    inline constexpr unsigned int FONT_COORDINATE = 14; // Board coordinates (monospace)
    
    // ==================== Spacing & Layout ====================
    // Follows UI_UX_Design_Specification.md Section 5.3
    
    // Spacing Scale (8px base unit)
    inline constexpr float SPACING_XS = 4.0f;   // Tight spacing
    inline constexpr float SPACING_SM = 8.0f;   // Compact
    inline constexpr float SPACING_MD = 16.0f;  // Default
    inline constexpr float SPACING_LG = 24.0f;  // Comfortable
    inline constexpr float SPACING_XL = 32.0f;  // Spacious
    inline constexpr float SPACING_2XL = 48.0f; // Dramatic
    
    // Layout Dimensions
    inline constexpr unsigned int WINDOW_WIDTH = 1280;
    inline constexpr unsigned int WINDOW_HEIGHT = 800;
    inline constexpr unsigned int BOARD_SIZE = 600;        // 8x8 board (600×600px)
    inline constexpr unsigned int BOARD_WITH_LABELS = 640; // With coordinate labels
    inline constexpr unsigned int PANEL_WIDTH = 400;       // Right panel width
    inline constexpr unsigned int CONTROL_BAR_HEIGHT = 50; // Top control bar
    inline constexpr unsigned int FOOTER_HEIGHT = 40;       // Bottom footer
    
    // Board Cell Dimensions
    inline constexpr unsigned int CELL_SIZE = 75;          // 75×75px per cell (including border)
    inline constexpr unsigned int DISC_DIAMETER = 60;      // 60px disc diameter
    inline constexpr unsigned int COORD_LABEL_HEIGHT = 20; // Coordinate label height
    
    // ==================== Animation Timings ====================
    // Follows UI/UX Design Specification.md Section 6.1
    
    inline constexpr float ANIMATION_FAST = 0.1f;      // 100ms - Micro-interactions
    inline constexpr float ANIMATION_NORMAL = 0.2f;    // 200ms - Standard transitions
    inline constexpr float ANIMATION_SLOW = 0.4f;      // 400ms - Dramatic effects
    
    // Specific Animations
    inline constexpr float ANIM_DISC_PLACE = 0.2f;     // Disc placement (200ms)
    inline constexpr float ANIM_DISC_FLIP = 0.3f;      // Disc flip (300ms)
    inline constexpr float ANIM_SCREEN_TRANSITION = 0.3f; // Screen transition (300ms)
    inline constexpr float ANIM_HOVER = 0.2f;         // Hover states (200ms)
    inline constexpr float ANIM_ERROR_SHAKE = 0.4f;   // Error shake (400ms)
    
    // ==================== Performance Targets ====================
    // Follows UI/UX Design Specification.md Section 10.1
    
    inline constexpr float TARGET_FPS = 60.0f;
    inline constexpr float MIN_FPS = 30.0f;
    inline constexpr float FRAME_TIME_MS = 16.67f;    // 60 FPS = 16.67ms per frame
    
    inline constexpr float STARTUP_TIME_MAX = 2.0f;   // <2 seconds cold start
    inline constexpr float INPUT_RESPONSE_MAX = 0.05f; // <50ms input to visual feedback
    
    // ==================== Helper Functions ====================
    
    /**
     * @brief Load font with fallback chain
     * @param font_path Path to font file
     * @return Loaded font or default system font
     */
    sf::Font load_font(const std::string& font_path = "");
    
    /**
     * @brief Get color with alpha channel
     * @param base_color Base color
     * @param alpha Alpha value (0-255)
     * @return Color with alpha
     */
    inline sf::Color with_alpha(const sf::Color& base_color, uint8_t alpha) {
        return sf::Color(base_color.r, base_color.g, base_color.b, alpha);
    }
    
} // namespace UIStyle
} // namespace ui
} // namespace reversi

