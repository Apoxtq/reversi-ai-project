#include "../src/core/Board.hpp"
#include "ai/MinimaxEngine.hpp"
#include <iostream>
#include <vector>
#include <string>

using namespace reversi::core;
using namespace reversi::ai;

int main() {
    Board board;
    MinimaxEngine::Config base;
    base.max_depth = 5;
    MinimaxEngine eb(base);
    auto rb = eb.find_best_move(board);
    int nodes_base = rb.nodes_searched;
    std::cout << "Baseline nodes=" << nodes_base << "\n";

    struct Opt { bool id, pvs, asp, killer; };
    std::vector<Opt> combos;
    for (int m = 0; m < 16; ++m) {
        combos.push_back({ (m&1)!=0, (m&2)!=0, (m&4)!=0, (m&8)!=0 });
    }
    for (auto c : combos) {
        MinimaxEngine::Config cfg;
        cfg.max_depth = 5;
        cfg.use_iterative_deepening = c.id;
        cfg.use_pvs = c.pvs;
        cfg.use_aspiration = c.asp;
        cfg.use_killer_moves = c.killer;
        MinimaxEngine e(cfg);
        auto r = e.find_best_move(board);
        int nodes = r.nodes_searched;
        double red = (1.0 - (double)nodes / nodes_base) * 100.0;
        std::string name = std::string(c.id ? "ID+" : "ID-") + (c.pvs ? "PVS+" : "PVS-") +
                           (c.asp ? "ASP+" : "ASP-") + (c.killer ? "KIL+" : "KIL-");
        std::cout << name << " nodes=" << nodes << " reduction=" << red << "%\n";
    }
    return 0;
}



