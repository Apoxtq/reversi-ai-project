/*
 * TranspositionTable.cpp - Implementation of transposition table
 * COMP390 Honours Year Project
 */

#include "ai/TranspositionTable.hpp"
#include <cassert>
#include <algorithm>

namespace reversi::ai {

TranspositionTable::TranspositionTable(int size_bits) {
    // Table size: 2^size_bits entries
    size_t table_size = 1ULL << size_bits;
    
    // Validate size_bits (avoid too large tables)
    assert(size_bits >= 10 && size_bits <= 24);
    
    // Allocate table (initialized to zero)
    table_.resize(table_size);
    
    // Precompute mask for fast modulo: index = hash & mask
    size_mask_ = table_size - 1;
    
    // Initialize statistics
    entries_ = 0;
    hits_ = 0;
    misses_ = 0;
}

TTEntry* TranspositionTable::probe(uint64_t hash) const {
    // Calculate index: hash & mask (fast modulo)
    size_t index = hash & size_mask_;
    const TTEntry& entry = table_[index];
    
    // Check if entry is valid and matches hash
    if (entry.is_valid() && entry.matches(hash)) {
        ++hits_;  // mutable, can be modified in const method
        return const_cast<TTEntry*>(&entry);
    }
    
    // Cache miss
    ++misses_;  // mutable, can be modified in const method
    return nullptr;
}

void TranspositionTable::store(const TTEntry& entry) {
    // Calculate index: hash & mask (fast modulo)
    size_t index = entry.hash & size_mask_;
    TTEntry& existing = table_[index];
    
    // Check if slot was empty before replacement
    bool was_empty = !existing.is_valid();
    
    // Replacement strategy: depth-preferred
    // Replace if:
    // 1. Slot is empty (hash == 0)
    // 2. New entry has greater depth
    // 3. Same depth but different position (collision)
    if (was_empty ||  // Empty slot
        existing.depth < entry.depth ||  // Deeper entry
        (existing.depth == entry.depth && !existing.matches(entry.hash))) {
        // Replace entry
        existing = entry;
        
        // Update entry count (only count new entries)
        if (was_empty) {
            ++entries_;
        }
    }
}

void TranspositionTable::clear() {
    // Clear all entries (set hash to 0)
    for (TTEntry& entry : table_) {
        entry.clear();
    }
    
    // Reset statistics
    entries_ = 0;
    hits_ = 0;
    misses_ = 0;
}

} // namespace reversi::ai

