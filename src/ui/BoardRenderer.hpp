/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Board Renderer (MVC View)
 * Renders the Reversi board following UI/UX Design Specification
 */

#pragma once

#include "../core/Board.hpp"
#include "UIStyle.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <memory>
#include <vector>

namespace reversi {
namespace ui {

/**
 * @brief Board renderer class (MVC View)
 * 
 * Responsible for rendering the Reversi board.
 * Follows UI/UX Design Specification for board appearance.
 * 
 * Responsibilities:
 * - Render 8x8 grid (600×600px)
 * - Render discs (black/white)
 * - Render legal move hints
 * - Render coordinate labels (optional)
 * - Handle coordinate conversion (screen → board position)
 * 
 * Does NOT manage:
 * - Game logic (handled by Board and GameState)
 * - Input handling (handled by EventHandler)
 */
class BoardRenderer : public sf::Drawable {
public:
    /**
     * @brief Construct board renderer
     * @param position Top-left position of board
     * @param board_size Board size in pixels (default: 600)
     */
    BoardRenderer(const sf::Vector2f& position, float board_size = UIStyle::BOARD_SIZE);
    
    /**
     * @brief Set board to render
     * @param board Board state to render
     */
    void set_board(const core::Board& board);
    
    /**
     * @brief Set legal moves to highlight
     * @param legal_moves Vector of legal move positions
     */
    void set_legal_moves(const std::vector<core::Move>& legal_moves);
    
    /**
     * @brief Set hovered cell (for hover effect)
     * @param cell_pos Cell position (0-63), or -1 for none
     */
    void set_hovered_cell(int cell_pos);
    
    /**
     * @brief Set last move highlight
     * @param cell_pos Last move position (0-63), or -1 for none
     */
    void set_last_move(int cell_pos);
    
    /**
     * @brief Enable/disable coordinate labels
     */
    void set_show_coordinates(bool show) { show_coordinates_ = show; }
    
    /**
     * @brief Convert screen coordinates to board position
     * @param screen_pos Screen coordinates
     * @return Board position (0-63), or -1 if outside board
     */
    int screen_to_board(const sf::Vector2f& screen_pos) const;
    
    /**
     * @brief Convert board position to screen coordinates (cell center)
     * @param board_pos Board position (0-63)
     * @return Screen coordinates of cell center
     */
    sf::Vector2f board_to_screen(int board_pos) const;
    
    /**
     * @brief Get board bounds
     */
    sf::FloatRect get_bounds() const;
    
    /**
     * @brief Update renderer (for animations)
     * @param dt Delta time
     */
    void update(float dt);
    
    /**
     * @brief Set animation system (for disc animations)
     */
    void set_animation_system(class AnimationSystem* anim_system) {
        animation_system_ = anim_system;
    }
    
protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
private:
    sf::Vector2f position_;
    float board_size_;
    float cell_size_;
    
    const core::Board* board_ = nullptr;
    std::vector<core::Move> legal_moves_;
    int hovered_cell_ = -1;
    int last_move_ = -1;
    bool show_coordinates_ = false;
    
    // Cached rendering data
    mutable sf::VertexArray grid_vertices_;
    mutable bool grid_dirty_ = true;
    
    // Animation system (optional)
    class AnimationSystem* animation_system_ = nullptr;
    
    // Helper methods
    void build_grid() const;
    void draw_grid(sf::RenderTarget& target, sf::RenderStates states) const;
    void draw_discs(sf::RenderTarget& target, sf::RenderStates states) const;
    void draw_legal_moves(sf::RenderTarget& target, sf::RenderStates states) const;
    void draw_hover_effect(sf::RenderTarget& target, sf::RenderStates states) const;
    void draw_last_move(sf::RenderTarget& target, sf::RenderStates states) const;
    void draw_coordinates(sf::RenderTarget& target, sf::RenderStates states) const;
    
    // Disc rendering helpers
    void draw_disc(sf::RenderTarget& target, int position, bool is_black, sf::RenderStates states) const;
    sf::CircleShape create_disc_shape(bool is_black) const;
};

} // namespace ui
} // namespace reversi

