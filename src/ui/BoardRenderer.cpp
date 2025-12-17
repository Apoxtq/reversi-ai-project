/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Board Renderer Implementation
 */

#include "BoardRenderer.hpp"
#include <cmath>
#include <algorithm>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

namespace reversi {
namespace ui {

BoardRenderer::BoardRenderer(const sf::Vector2f& position, float board_size)
    : position_(position)
    , board_size_(board_size)
    , cell_size_(board_size / 8.0f)
    , grid_vertices_(sf::Quads)
{
}

void BoardRenderer::set_board(const core::Board& board) {
    board_ = &board;
    grid_dirty_ = true;
}

void BoardRenderer::set_legal_moves(const std::vector<core::Move>& legal_moves) {
    legal_moves_ = legal_moves;
}

void BoardRenderer::set_hovered_cell(int cell_pos) {
    hovered_cell_ = cell_pos;
}

void BoardRenderer::set_last_move(int cell_pos) {
    last_move_ = cell_pos;
}

int BoardRenderer::screen_to_board(const sf::Vector2f& screen_pos) const {
    // Check if point is within board bounds
    if (screen_pos.x < position_.x || screen_pos.x > position_.x + board_size_ ||
        screen_pos.y < position_.y || screen_pos.y > position_.y + board_size_) {
        return -1;
    }
    
    // Calculate cell coordinates
    float relative_x = screen_pos.x - position_.x;
    float relative_y = screen_pos.y - position_.y;
    
    int col = static_cast<int>(relative_x / cell_size_);
    int row = static_cast<int>(relative_y / cell_size_);
    
    // Clamp to valid range
    col = std::clamp(col, 0, 7);
    row = std::clamp(row, 0, 7);
    
    return row * 8 + col;
}

sf::Vector2f BoardRenderer::board_to_screen(int board_pos) const {
    if (board_pos < 0 || board_pos >= 64) {
        return position_;
    }
    
    int row = board_pos / 8;
    int col = board_pos % 8;
    
    float center_x = position_.x + col * cell_size_ + cell_size_ / 2.0f;
    float center_y = position_.y + row * cell_size_ + cell_size_ / 2.0f;
    
    return sf::Vector2f(center_x, center_y);
}

sf::FloatRect BoardRenderer::get_bounds() const {
    return sf::FloatRect(position_.x, position_.y, board_size_, board_size_);
}

void BoardRenderer::update(float dt) {
    // Update animations if needed (future enhancement)
    (void)dt; // Suppress unused parameter warning
}

void BoardRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (!board_) return;
    
    // Build grid if dirty
    if (grid_dirty_) {
        build_grid();
        grid_dirty_ = false;
    }
    
    // Draw components in order
    draw_grid(target, states);
    draw_legal_moves(target, states);
    draw_discs(target, states);
    draw_hover_effect(target, states);
    draw_last_move(target, states);
    
    if (show_coordinates_) {
        draw_coordinates(target, states);
    }
}

void BoardRenderer::build_grid() const {
    grid_vertices_.clear();
    grid_vertices_.resize(8 * 8 * 4); // 8x8 cells, 4 vertices per quad
    
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            float x = position_.x + col * cell_size_;
            float y = position_.y + row * cell_size_;
            
            // Alternate colors (checkerboard pattern)
            bool is_dark = (row + col) % 2 == 0;
            sf::Color cell_color = is_dark ? UIStyle::BOARD_DARK : UIStyle::BOARD_LIGHT;
            
            // Create quad for this cell
            int index = (row * 8 + col) * 4;
            grid_vertices_[index + 0] = sf::Vertex(sf::Vector2f(x, y), cell_color);
            grid_vertices_[index + 1] = sf::Vertex(sf::Vector2f(x + cell_size_, y), cell_color);
            grid_vertices_[index + 2] = sf::Vertex(sf::Vector2f(x + cell_size_, y + cell_size_), cell_color);
            grid_vertices_[index + 3] = sf::Vertex(sf::Vector2f(x, y + cell_size_), cell_color);
        }
    }
}

void BoardRenderer::draw_grid(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(grid_vertices_, states);
    
    // Draw grid lines
    sf::RectangleShape line;
    line.setFillColor(UIStyle::BOARD_BORDER);
    
    // Vertical lines
    for (int i = 0; i <= 8; ++i) {
        float x = position_.x + i * cell_size_;
        line.setSize(sf::Vector2f(1.0f, board_size_));
        line.setPosition(x, position_.y);
        target.draw(line, states);
    }
    
    // Horizontal lines
    for (int i = 0; i <= 8; ++i) {
        float y = position_.y + i * cell_size_;
        line.setSize(sf::Vector2f(board_size_, 1.0f));
        line.setPosition(position_.x, y);
        target.draw(line, states);
    }
}

void BoardRenderer::draw_discs(sf::RenderTarget& target, sf::RenderStates states) const {
    for (int pos = 0; pos < 64; ++pos) {
        uint64_t mask = 1ULL << pos;
        
        if (board_->player & mask) {
            draw_disc(target, pos, true, states);  // Black
        } else if (board_->opponent & mask) {
            draw_disc(target, pos, false, states); // White
        }
    }
}

void BoardRenderer::draw_legal_moves(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::RectangleShape highlight;
    highlight.setSize(sf::Vector2f(cell_size_ - 2.0f, cell_size_ - 2.0f));
    highlight.setOutlineThickness(3.0f);
    highlight.setOutlineColor(UIStyle::ACCENT_GREEN);
    highlight.setFillColor(sf::Color::Transparent);
    
    for (const auto& move : legal_moves_) {
        if (move.is_valid()) {
            sf::Vector2f cell_pos = board_to_screen(move.position);
            highlight.setPosition(cell_pos - sf::Vector2f(cell_size_ / 2.0f, cell_size_ / 2.0f));
            target.draw(highlight, states);
        }
    }
}

void BoardRenderer::draw_hover_effect(sf::RenderTarget& target, sf::RenderStates states) const {
    if (hovered_cell_ < 0 || hovered_cell_ >= 64) return;
    
    // Check if hovered cell is a legal move (same pattern as draw_legal_moves)
    bool is_legal = false;
    for (const auto& move : legal_moves_) {
        if (move.is_valid() && move.position == hovered_cell_) {
            is_legal = true;
            break;
        }
    }
    
    if (is_legal) {
        // Draw preview disc
        sf::CircleShape preview(UIStyle::DISC_DIAMETER / 2.0f);
        preview.setFillColor(UIStyle::with_alpha(UIStyle::DISC_BLACK, 128));
        sf::Vector2f center = board_to_screen(hovered_cell_);
        preview.setPosition(center - sf::Vector2f(UIStyle::DISC_DIAMETER / 2.0f, UIStyle::DISC_DIAMETER / 2.0f));
        target.draw(preview, states);
    }
}

void BoardRenderer::draw_last_move(sf::RenderTarget& target, sf::RenderStates states) const {
    if (last_move_ < 0 || last_move_ >= 64) return;
    
    // Draw yellow corner indicators (simplified - just a highlight)
    sf::RectangleShape indicator;
    indicator.setSize(sf::Vector2f(10.0f, 10.0f));
    indicator.setFillColor(UIStyle::WARNING);
    sf::Vector2f center = board_to_screen(last_move_);
    indicator.setPosition(center - sf::Vector2f(5.0f, 5.0f));
    target.draw(indicator, states);
}

void BoardRenderer::draw_coordinates(sf::RenderTarget& target, sf::RenderStates states) const {
    // Placeholder - would need font loading
    // Draw column labels (a-h) at top and bottom
    // Draw row labels (1-8) at left and right
    (void)target;
    (void)states;
}

void BoardRenderer::draw_disc(sf::RenderTarget& target, int position, bool is_black, sf::RenderStates states) const {
    sf::CircleShape disc = create_disc_shape(is_black);
    sf::Vector2f center = board_to_screen(position);
    disc.setPosition(center - sf::Vector2f(UIStyle::DISC_DIAMETER / 2.0f, UIStyle::DISC_DIAMETER / 2.0f));
    target.draw(disc, states);
}

sf::CircleShape BoardRenderer::create_disc_shape(bool is_black) const {
    sf::CircleShape disc(UIStyle::DISC_DIAMETER / 2.0f);
    disc.setFillColor(is_black ? UIStyle::DISC_BLACK : UIStyle::DISC_WHITE);
    
    // Add subtle shadow effect
    disc.setOutlineThickness(1.0f);
    disc.setOutlineColor(sf::Color(0, 0, 0, 100));
    
    return disc;
}

} // namespace ui
} // namespace reversi

