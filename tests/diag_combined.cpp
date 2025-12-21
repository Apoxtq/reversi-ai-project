#include "../src/core/Board.hpp"
#include "ai/MinimaxEngine.hpp"
#include <iostream>

using namespace reversi::core;
using namespace reversi::ai;

int main() {
    Board board;
    MinimaxEngine::Config config_all;
    config_all.max_depth = 5;
    config_all.use_iterative_deepening = true;
    config_all.use_pvs = true;
    config_all.use_aspiration = true;
    config_all.use_killer_moves = true;

    MinimaxEngine engine(config_all);
    auto result = engine.find_best_move(board);
    auto d = engine.get_pvs_diagnostics();

    std::cout << "Combined diag:\n";
    std::cout << "  nodes=" << result.nodes_searched << " time_ms=" << result.time_ms << "\n";
    std::cout << "  pvs_zero_window_failures=" << d.zero_window_failures
              << " researches=" << d.researches
              << " beta_cutoffs=" << d.zero_window_beta_cutoffs << "\n";
    std::cout << "  failures_per_ply:";
    for (int v : d.failures_per_ply) std::cout << " " << v;
    std::cout << "\n";
    return 0;
}



