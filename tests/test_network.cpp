/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Network Module Test
 * Tests basic network functionality: protocol, room management, socket
 */

#include "../src/network/NetworkMessage.hpp"
#include "../src/network/NetworkProtocol.hpp"
#include "../src/network/RoomManager.hpp"
#include "../src/core/Board.hpp"
#include "../src/core/Move.hpp"
#include <iostream>
#include <cassert>
#include <bit>

using namespace reversi;
using namespace reversi::network;
using namespace reversi::core;

void test_network_message() {
    std::cout << "Testing NetworkMessage...\n";
    
    NetworkMessage msg(MessageType::MOVE);
    assert(msg.message_type == static_cast<uint8_t>(MessageType::MOVE));
    assert(msg.protocol_version == PROTOCOL_VERSION);
    assert(msg.sequence_number == 0);
    
    std::cout << "  ✓ NetworkMessage creation passed\n";
}

void test_network_protocol_serialization() {
    std::cout << "Testing NetworkProtocol serialization...\n";
    
    // Create a test message
    Board board;
    Move move(28);  // Position d4
    NetworkMessage msg = NetworkProtocol::create_move_message(move, board, 1);
    
    // Serialize
    std::vector<uint8_t> buffer;
    bool serialized = NetworkProtocol::serialize(msg, buffer);
    assert(serialized);
    assert(buffer.size() == 82);
    
    // Deserialize
    NetworkMessage msg2;
    bool deserialized = NetworkProtocol::deserialize(buffer, msg2);
    assert(deserialized);
    
    // Verify
    assert(msg2.message_type == msg.message_type);
    assert(msg2.sequence_number == msg.sequence_number);
    assert(msg2.board_hash == msg.board_hash);
    
    // Extract move
    Move extracted = NetworkProtocol::extract_move(msg2);
    assert(extracted.position == move.position);
    
    std::cout << "  ✓ Serialization/deserialization passed\n";
}

void test_network_protocol_checksum() {
    std::cout << "Testing NetworkProtocol checksum...\n";
    
    Board board;
    Move move(28);
    NetworkMessage msg = NetworkProtocol::create_move_message(move, board, 1);
    
    // Verify checksum
    bool valid = NetworkProtocol::verify_checksum(msg);
    assert(valid);
    
    // Corrupt message
    msg.data[0] ^= 0xFF;
    bool invalid = NetworkProtocol::verify_checksum(msg);
    assert(!invalid);
    
    std::cout << "  ✓ Checksum verification passed\n";
}

void test_room_manager() {
    std::cout << "Testing RoomManager...\n";
    
    RoomManager& rm = RoomManager::get_instance();
    
    // Test room code generation
    std::string code1 = RoomManager::generate_room_code();
    assert(code1.length() == 6);
    assert(RoomManager::is_valid_room_code(code1));
    
    // Test room creation
    std::string code2 = rm.create_room("192.168.1.100", 12345);
    assert(!code2.empty());
    assert(code2.length() == 6);
    
    // Test room lookup
    auto room = rm.find_room(code2);
    assert(room != nullptr);
    assert(room->host_ip == "192.168.1.100");
    assert(room->host_port == 12345);
    
    // Test invalid room code
    assert(!RoomManager::is_valid_room_code("INVALID"));
    assert(!RoomManager::is_valid_room_code("ABC"));
    assert(!RoomManager::is_valid_room_code("ABCDEFG"));  // Too long
    
    // Test room removal
    rm.remove_room(code2);
    auto removed_room = rm.find_room(code2);
    assert(removed_room == nullptr);
    
    std::cout << "  ✓ RoomManager operations passed\n";
}

void test_board_hash_usage() {
    std::cout << "Testing Board::hash() usage...\n";
    
    Board board1;
    uint64_t hash1 = board1.hash();
    assert(hash1 != 0);  // Hash should not be zero
    
    // Make a move (position 28 = d4, which is a valid move from initial position)
    // Note: We need to check if move is valid first
    if (board1.legal_moves() & (1ULL << 28)) {
        board1.make_move(28);
        uint64_t hash2 = board1.hash();
        
        // Hashes should be different after move
        assert(hash1 != hash2);
        
        // Create message with hash
        Move move(28);
        NetworkMessage msg = NetworkProtocol::create_move_message(move, board1, 1);
        assert(msg.board_hash == hash2);
    } else {
        // If position 28 is not valid, try a valid move
        uint64_t legal = board1.legal_moves();
        if (legal != 0) {
            int pos = static_cast<int>(std::countr_zero(legal));  // Get first valid move (C++20)
            board1.make_move(pos);
            uint64_t hash2 = board1.hash();
            assert(hash1 != hash2);
            
            Move move(pos);
            NetworkMessage msg = NetworkProtocol::create_move_message(move, board1, 1);
            assert(msg.board_hash == hash2);
        }
    }
    
    std::cout << "  ✓ Board::hash() usage passed\n";
}

int main() {
    std::cout << "=== Network Module Test Suite ===\n\n";
    
    try {
        test_network_message();
        test_network_protocol_serialization();
        test_network_protocol_checksum();
        test_room_manager();
        test_board_hash_usage();
        
        std::cout << "\n=== All Tests Passed! ===\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Test failed with unknown error\n";
        return 1;
    }
}

