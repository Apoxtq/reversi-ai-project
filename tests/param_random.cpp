#include "../src/core/Board.hpp"
#include "ai/MinimaxEngine.hpp"
#include <iostream>
#include <random>

using namespace reversi::core;
using namespace reversi::ai;

int main() {
    Board board;
    MinimaxEngine::Config base;
    base.max_depth = 5;
    base.use_iterative_deepening = false;
    base.use_pvs = false;
    base.use_aspiration = false;
    base.use_killer_moves = false;

    MinimaxEngine engine_base(base);
    auto rbase = engine_base.find_best_move(board);
    int nodes_base = rbase.nodes_searched;
    std::cout << "Baseline nodes=" << nodes_base << "\n";

    std::mt19937_64 rng(1234567);
    std::uniform_int_distribution<int> dist_pvs(0, 300);
    std::uniform_int_distribution<int> dist_asp(50, 600);
    std::uniform_int_distribution<int> dist_killer(0, 400);
    std::uniform_int_distribution<int> dist_tt(6000, 12000);
    std::uniform_int_distribution<int> dist_hist(0, 200);

    for (int iter = 0; iter < 500; ++iter) {
        MinimaxEngine::Config cfg;
        cfg.max_depth = 5;
        cfg.use_iterative_deepening = true;
        cfg.use_pvs = true;
        cfg.use_aspiration = true;
        cfg.use_killer_moves = true;
        cfg.pvs_research_margin = dist_pvs(rng);
        cfg.aspiration_window = dist_asp(rng);
        cfg.killer_weight = dist_killer(rng);
        cfg.tt_weight = dist_tt(rng);
        cfg.history_weight = dist_hist(rng);

        MinimaxEngine engine(cfg);
        auto rc = engine.find_best_move(board);
        int nodes_comb = rc.nodes_searched;

        MinimaxEngine::Config cfg_k = base;
        cfg_k.use_killer_moves = true;
        MinimaxEngine engine_k(cfg_k);
        auto rk = engine_k.find_best_move(board);
        int nodes_k = rk.nodes_searched;

        double combined_reduction = (1.0 - (double)nodes_comb / nodes_base) * 100.0;
        double killer_reduction = (1.0 - (double)nodes_k / nodes_base) * 100.0;

        if (combined_reduction > 10.0 && killer_reduction > 0.0) {
            std::cout << "[FOUND] iter=" << iter << " pm=" << cfg.pvs_research_margin
                      << " aw=" << cfg.aspiration_window << " kw=" << cfg.killer_weight
                      << " tt=" << cfg.tt_weight << " hw=" << cfg.history_weight
                      << " comb_red=" << combined_reduction << "% killer_red=" << killer_reduction << "%\n";
            return 0;
        }
        if ((iter & 31) == 0) {
            std::cout << "iter=" << iter << " comb_red=" << combined_reduction << "%\n";
        }
    }

    std::cout << "No good random combo found after 500 samples.\n";
    return 1;
}



