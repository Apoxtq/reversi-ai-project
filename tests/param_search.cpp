#include "../src/core/Board.hpp"
#include "ai/MinimaxEngine.hpp"
#include <iostream>
#include <vector>
#include <tuple>

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

    std::vector<int> pvs_margins = {0, 20, 50, 100, 200};
    std::vector<int> asp_windows = {50, 150, 300, 600};
    std::vector<int> killer_weights = {0, 100, 200, 400};
    std::vector<int> tt_weights = {6000, 8000, 10000};
    std::vector<int> history_weights = {0};

    for (int pm : pvs_margins) {
        for (int aw : asp_windows) {
            for (int kw : killer_weights) {
                for (int tt : tt_weights) {
                    for (int hw : history_weights) {
                        MinimaxEngine::Config cfg;
                        cfg.max_depth = 5;
                        // Use fixed-depth for targeted Killer+PVS tuning
                        cfg.use_iterative_deepening = false;
                        cfg.use_pvs = true;
                        cfg.use_aspiration = true;
                        cfg.use_killer_moves = true;
                        cfg.pvs_research_margin = pm;
                        cfg.aspiration_window = aw;
                        cfg.killer_weight = kw;
                        cfg.tt_weight = tt;
                        cfg.history_weight = hw;

                        MinimaxEngine engine(cfg);
                        auto rc = engine.find_best_move(board);
                        int nodes_comb = rc.nodes_searched;

                        // Test killer-only improvement
                        MinimaxEngine::Config cfg_k = base;
                        cfg_k.use_killer_moves = true;
                        MinimaxEngine engine_k(cfg_k);
                        auto rk = engine_k.find_best_move(board);
                        int nodes_k = rk.nodes_searched;

                        double combined_reduction = (1.0 - (double)nodes_comb / nodes_base) * 100.0;
                        double killer_reduction = (1.0 - (double)nodes_k / nodes_base) * 100.0;

                        std::cout << "pm=" << pm << " aw=" << aw << " kw=" << kw << " tt=" << tt << " hw=" << hw
                                  << " -> comb_red=" << combined_reduction << "% killer_red=" << killer_reduction
                                  << "% nodes_comb=" << nodes_comb << " nodes_k=" << nodes_k << "\n";

                        if (combined_reduction > 10.0 && killer_reduction > 0.0) {
                            std::cout << "[FOUND] good combo pm=" << pm << " aw=" << aw << " kw=" << kw
                                      << " tt=" << tt << " hw=" << hw << "\n";
                            return 0;
                        }
                    }
                }
            }
        }
    }

    std::cout << "No good combo found in grid.\n";
    return 1;
}


