/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Network Lobby State Implementation
 */

#include "NetworkLobbyState.hpp"
#include "UIStyle.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <chrono>

namespace reversi {
namespace ui {

NetworkLobbyState::NetworkLobbyState(Mode mode)
    : mode_(mode), room_created_(false), current_input_box_(0),
      all_boxes_filled_(false), network_state_(network::NetworkGameState::DISCONNECTED),
      network_rtt_ms_(0), status_animation_time_(0.0f) {
    room_code_input_.fill('\0');
    
    // Load font with fallback mechanism
    try {
    font_ = std::make_unique<sf::Font>();
    } catch (const std::exception& e) {
        std::cerr << "Error: Failed to create font object: " << e.what() << "\n";
        font_.reset();  // Ensure font_ is nullptr on failure
        // Continue without font - UI will use fallback rendering
    }
    
    bool font_loaded = false;
    
    if (!font_) {
        std::cerr << "Warning: Font object creation failed. Text rendering will be disabled.\n";
        // Continue without font - UI will use fallback rendering
    } else {
    
    // Try to load primary font
    try {
        if (font_->loadFromFile("fonts/Roboto-Regular.ttf")) {
            font_loaded = true;
        }
    } catch (const std::exception& e) {
        std::cerr << "Warning: Failed to load font from fonts/Roboto-Regular.ttf: " << e.what() << "\n";
        font_loaded = false;
    } catch (...) {
        font_loaded = false;
    }
    
    // Fallback: Try alternative font paths
    if (!font_loaded) {
        try {
            if (font_->loadFromFile("assets/fonts/Roboto-Regular.ttf")) {
                font_loaded = true;
            }
        } catch (const std::exception& e) {
            std::cerr << "Warning: Failed to load font from assets/fonts/Roboto-Regular.ttf: " << e.what() << "\n";
            font_loaded = false;
        } catch (...) {
            font_loaded = false;
        }
    }
    
    // Fallback: Try system fonts (Windows)
    #ifdef _WIN32
    if (!font_loaded) {
        const char* system_fonts[] = {
            "C:/Windows/Fonts/arial.ttf",
            "C:/Windows/Fonts/calibri.ttf",
            "C:/Windows/Fonts/segoeui.ttf",
            "C:/Windows/Fonts/tahoma.ttf"
        };
        for (const char* font_path : system_fonts) {
            try {
                if (font_ && font_->loadFromFile(font_path)) {
                font_loaded = true;
                    std::cout << "Loaded system font: " << font_path << "\n";
                break;
                }
            } catch (const std::exception& e) {
                // Continue to next font
                continue;
            } catch (...) {
                continue;
            }
        }
    }
    #endif
    
    // Final fallback: Log warning but continue without font
    if (!font_loaded) {
            std::cerr << "Warning: Could not load any font. Text rendering will be disabled.\n";
            std::cerr << "The application will continue but text may not be displayed.\n";
        // Font object exists but is empty - rendering code checks font_->getInfo().family.empty()
        } else {
            std::cout << "Font loaded successfully.\n";
        }
    }
    
    // Setup UI (works even without font)
    try {
    if (mode_ == Mode::CREATE_ROOM) {
        setup_create_room_ui();
    } else {
        setup_join_room_ui();
        }
    } catch (const std::exception& e) {
        std::cerr << "Warning: Failed to setup UI: " << e.what() << "\n";
        // Continue - basic functionality may still work
    } catch (...) {
        std::cerr << "Warning: Unknown error during UI setup.\n";
    }
}

NetworkLobbyState::~NetworkLobbyState() = default;

void NetworkLobbyState::update(float dt) {
    status_animation_time_ += dt;
    
    if (mode_ == Mode::CREATE_ROOM) {
        update_create_room(dt);
    } else {
        update_join_room(dt);
    }
}

void NetworkLobbyState::render(sf::RenderTarget& target) {
    if (mode_ == Mode::CREATE_ROOM) {
        render_create_room(target);
    } else {
        render_join_room(target);
    }
}

bool NetworkLobbyState::handle_event(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        handle_keyboard_input(event.key);
        return true;
    } else if (event.type == sf::Event::TextEntered) {
        handle_text_input(event.text);
        return true;
    }
    
    return false;
}

std::unique_ptr<MenuState> NetworkLobbyState::get_next_state() {
    if (connection_result_.success) {
        // Return to main menu or transition to game
        // This will be handled by MenuSystem
        return nullptr;
    }
    return nullptr;
}

bool NetworkLobbyState::is_finished() const {
    return connection_result_.success || 
           network_state_ == network::NetworkGameState::ERROR_STATE;
}

void NetworkLobbyState::setup_create_room_ui() {
    // UI setup will be implemented with actual SFML rendering
    // For now, this is a placeholder
}

void NetworkLobbyState::setup_join_room_ui() {
    // UI setup will be implemented with actual SFML rendering
    // For now, this is a placeholder
}

void NetworkLobbyState::update_create_room(float dt) {
    if (!room_created_) {
        // Create room when first updated
        handle_create_room();
    }
    
    // Check for timeout (5 minutes)
    if (room_created_) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::minutes>(
            now - room_created_time_);
        
        if (elapsed.count() >= 5) {
            set_status_message("No one joined. Room expired.");
            connection_result_.success = false;
        }
    }
}

void NetworkLobbyState::update_join_room(float dt) {
    update_input_boxes();
}

void NetworkLobbyState::render_create_room(sf::RenderTarget& target) {
    // Render create room UI
    // This is a simplified version - full implementation would use UIComponent system
    
    // Safe font check: ensure font_ is not nullptr before accessing
    bool font_available = false;
    if (font_) {
        try {
            // Check if font is loaded by checking if family is not empty
            font_available = !font_->getInfo().family.empty();
        } catch (...) {
            // If getInfo() throws, font is not available
            font_available = false;
        }
    }
    
    // If font not available, render basic UI without text
    if (!font_available) {
        // Draw room code as colored boxes instead
        if (!room_code_.empty()) {
            float start_x = 100;
            float start_y = 200;
            float box_size = 80;
            float spacing = 10;
            
            for (size_t i = 0; i < room_code_.length() && i < 6; ++i) {
                sf::RectangleShape box(sf::Vector2f(box_size, box_size));
                box.setPosition(start_x + i * (box_size + spacing), start_y);
                box.setFillColor(sf::Color(74, 157, 95, 100));  // ACCENT_GREEN with alpha
                box.setOutlineColor(sf::Color(74, 157, 95));
                box.setOutlineThickness(3.0f);
                target.draw(box);
            }
        }
        return;
    }
    
    // Font is available - render text
    try {
    // Title
    sf::Text title("Create Room", *font_, 48);
    title.setPosition(100, 50);
    title.setFillColor(sf::Color::White);
    target.draw(title);
    
    // Room code (large, 72px as per UI/UX spec)
    if (!room_code_.empty()) {
        sf::Text code_text(room_code_, *font_, 72);
        code_text.setPosition(100, 200);
        code_text.setFillColor(sf::Color::Green);
        code_text.setStyle(sf::Text::Bold);
        target.draw(code_text);
        
        // Status message
            if (!status_message_.empty()) {
        sf::Text status(status_message_, *font_, 24);
        status.setPosition(100, 300);
        status.setFillColor(sf::Color::Yellow);
        target.draw(status);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error rendering text: " << e.what() << "\n";
        // Fall back to non-text rendering
    } catch (...) {
        std::cerr << "Unknown error rendering text.\n";
    }
}

void NetworkLobbyState::render_join_room(sf::RenderTarget& target) {
    // Render join room UI
    
    // Safe font check: ensure font_ is not nullptr before accessing
    bool font_available = false;
    if (font_) {
        try {
            // Check if font is loaded by checking if family is not empty
            font_available = !font_->getInfo().family.empty();
        } catch (...) {
            // If getInfo() throws, font is not available
            font_available = false;
        }
    }
    
    // Input boxes (6 boxes for room code) - always render these
    float start_x = 100;
    float start_y = 200;
    float box_size = 60;
    float spacing = 10;
    
    for (int i = 0; i < 6; ++i) {
        sf::RectangleShape box(sf::Vector2f(box_size, box_size));
        box.setPosition(start_x + i * (box_size + spacing), start_y);
        box.setFillColor(sf::Color::Transparent);
        box.setOutlineColor(i == current_input_box_ ? sf::Color::Yellow : sf::Color::White);
        box.setOutlineThickness(2);
        target.draw(box);
        
        // Character in box
        if (room_code_input_[i] != '\0') {
            if (font_available) {
                try {
            sf::Text char_text(std::string(1, room_code_input_[i]), *font_, 36);
            char_text.setPosition(start_x + i * (box_size + spacing) + 15, start_y + 10);
            char_text.setFillColor(sf::Color::White);
            target.draw(char_text);
                } catch (...) {
                    // Fallback: Draw filled box to indicate character entered
                    sf::RectangleShape filled_box(sf::Vector2f(box_size - 10, box_size - 10));
                    filled_box.setPosition(start_x + i * (box_size + spacing) + 5, start_y + 5);
                    filled_box.setFillColor(sf::Color::White);
                    target.draw(filled_box);
                }
            } else {
            // Fallback: Draw filled box to indicate character entered
            sf::RectangleShape filled_box(sf::Vector2f(box_size - 10, box_size - 10));
            filled_box.setPosition(start_x + i * (box_size + spacing) + 5, start_y + 5);
            filled_box.setFillColor(sf::Color::White);
            target.draw(filled_box);
        }
        }
    }
    
    // Render title and status message if font is available
    if (font_available) {
        try {
            // Title
            sf::Text title("Join Room", *font_, 48);
            title.setPosition(100, 50);
            title.setFillColor(sf::Color::White);
            target.draw(title);
            
            // Status message
            if (!status_message_.empty()) {
        sf::Text status(status_message_, *font_, 24);
        status.setPosition(100, 300);
        status.setFillColor(get_network_status_color());
        target.draw(status);
            }
        } catch (const std::exception& e) {
            std::cerr << "Error rendering text: " << e.what() << "\n";
        } catch (...) {
            std::cerr << "Unknown error rendering text.\n";
        }
    } else if (!status_message_.empty()) {
        // Fallback: Draw colored indicator for status
        sf::CircleShape status_indicator(8.0f);
        status_indicator.setPosition(100, 300 + 8.0f);
        status_indicator.setFillColor(get_network_status_color());
        target.draw(status_indicator);
    }
}

void NetworkLobbyState::handle_keyboard_input(const sf::Event::KeyEvent& event) {
    if (mode_ == Mode::JOIN_ROOM) {
        if (event.code == sf::Keyboard::BackSpace) {
            if (current_input_box_ > 0 && room_code_input_[current_input_box_] == '\0') {
                --current_input_box_;
            }
            room_code_input_[current_input_box_] = '\0';
            update_input_boxes();
        } else if (event.code == sf::Keyboard::Enter && all_boxes_filled_) {
            handle_join_room();
        }
    }
}

void NetworkLobbyState::handle_text_input(const sf::Event::TextEvent& event) {
    if (mode_ == Mode::JOIN_ROOM) {
        char c = static_cast<char>(event.unicode);
        
        // Check if valid character (A-Z, 0-9, excluding O/0/I/1/L)
        if ((c >= 'A' && c <= 'Z' && c != 'O' && c != 'I' && c != 'L') ||
            (c >= 'a' && c <= 'z' && c != 'o' && c != 'i' && c != 'l') ||
            (c >= '2' && c <= '9')) {
            
            // Convert to uppercase
            c = std::toupper(c);
            
            room_code_input_[current_input_box_] = c;
            
            // Auto-advance to next box
            if (current_input_box_ < 5) {
                ++current_input_box_;
            }
            
            update_input_boxes();
        }
    }
}

void NetworkLobbyState::handle_paste(const std::string& text) {
    // Handle paste: fill all boxes if text is 6 characters
    if (text.length() == 6) {
        for (int i = 0; i < 6 && i < static_cast<int>(text.length()); ++i) {
            char c = std::toupper(text[i]);
            if (network::RoomManager::is_valid_room_code(std::string(1, c))) {
                room_code_input_[i] = c;
            }
        }
        current_input_box_ = 5;
        update_input_boxes();
    }
}

void NetworkLobbyState::update_input_boxes() {
    // Check if all boxes are filled
    all_boxes_filled_ = true;
    for (int i = 0; i < 6; ++i) {
        if (room_code_input_[i] == '\0') {
            all_boxes_filled_ = false;
            break;
        }
    }
}

std::string NetworkLobbyState::get_room_code_from_input() const {
    std::string code;
    for (int i = 0; i < 6; ++i) {
        if (room_code_input_[i] != '\0') {
            code += room_code_input_[i];
        } else {
            break;
        }
    }
    return code;
}

void NetworkLobbyState::handle_create_room() {
    // Create network game as Host
    auto network_game = std::make_unique<network::NetworkGame>(network::NetworkRole::HOST);
    
    // Start listening
    room_code_ = network_game->start_as_host(0);  // Auto-assign port
    
    if (!room_code_.empty()) {
        room_created_ = true;
        room_created_time_ = std::chrono::steady_clock::now();
        set_status_message("Waiting for player 2...");
        
        // Store connection result
        connection_result_.success = true;
        connection_result_.room_code = room_code_;
        connection_result_.role = network::NetworkRole::HOST;
        // IP and port will be set by NetworkGame
    } else {
        set_status_message("Failed to create room");
        connection_result_.success = false;
    }
}

void NetworkLobbyState::handle_join_room() {
    std::string code = get_room_code_from_input();
    
    if (!network::RoomManager::is_valid_room_code(code)) {
        set_status_message("Invalid room code");
        return;
    }
    
    // Find room
    auto room = network::RoomManager::get_instance().find_room(code);
    if (!room) {
        set_status_message("Room not found or expired");
        return;
    }
    
    set_status_message("Connecting...");
    
    // Create network game as Client
    auto network_game = std::make_unique<network::NetworkGame>(network::NetworkRole::CLIENT);
    
    // Connect to host
    bool connected = network_game->start_as_client(code, room->host_ip, room->host_port);
    
    if (connected) {
        connection_result_.success = true;
        connection_result_.room_code = code;
        connection_result_.host_ip = room->host_ip;
        connection_result_.host_port = room->host_port;
        connection_result_.role = network::NetworkRole::CLIENT;
        set_status_message("Connected! Game starting...");
    } else {
        set_status_message("Failed to connect");
        connection_result_.success = false;
    }
}

void NetworkLobbyState::set_status_message(const std::string& message) {
    status_message_ = message;
    status_animation_time_ = 0.0f;
}

sf::Color NetworkLobbyState::get_network_status_color() const {
    if (network_rtt_ms_ < 50) {
        return sf::Color::Green;  // 🟢 LAN
    } else if (network_rtt_ms_ < 150) {
        return sf::Color::Yellow;  // 🟡 Internet
    } else {
        return sf::Color::Red;  // 🔴 High latency
    }
}

} // namespace ui
} // namespace reversi

