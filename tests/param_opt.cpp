#include "../src/core/Board.hpp"
#include "ai/MinimaxEngine.hpp"
#include <iostream>
#include <random>
#include <tuple>
#include <vector>

using namespace reversi::core;
using namespace reversi::ai;

struct Params {
    int pvs_margin;
    int asp_window;
    int killer_weight;
    int tt_weight;
    int history_weight;
};

double evaluate(const Params& p, int& nodes_comb, int& nodes_killer) {
    Board board;
    MinimaxEngine::Config cfg;
    cfg.max_depth = 5;
    cfg.use_iterative_deepening = true;
    cfg.use_pvs = true;
    cfg.use_aspiration = true;
    cfg.use_killer_moves = true;
    cfg.pvs_research_margin = p.pvs_margin;
    cfg.aspiration_window = p.asp_window;
    cfg.killer_weight = p.killer_weight;
    cfg.tt_weight = p.tt_weight;
    cfg.history_weight = p.history_weight;

    MinimaxEngine engine(cfg);
    auto r = engine.find_best_move(board);
    nodes_comb = r.nodes_searched;

    MinimaxEngine::Config kcfg;
    kcfg.max_depth = 5;
    kcfg.use_killer_moves = true;
    MinimaxEngine ek(kcfg);
    auto rk = ek.find_best_move(board);
    nodes_killer = rk.nodes_searched;

    // return combined reduction percentage relative to baseline  (caller must compute baseline)
    return 0.0;
}

int main() {
    // baseline
    Board board;
    MinimaxEngine::Config base;
    base.max_depth = 5;
    MinimaxEngine eb(base);
    auto rb = eb.find_best_move(board);
    int nodes_base = rb.nodes_searched;
    std::cout << "Baseline nodes=" << nodes_base << "\n";

    // initial params (start from earlier reasonable defaults)
    Params cur{50, 150, 100, 8000, 0};
    int nodes_cur = INT_MAX;
    int nodes_k_cur = INT_MAX;
    int best_nodes = INT_MAX;
    Params best = cur;

    std::mt19937 rng(1234567);
    std::uniform_int_distribution<int> dpm(0, 200);
    std::uniform_int_distribution<int> daw(50, 600);
    std::uniform_int_distribution<int> dkw(0, 400);
    std::uniform_int_distribution<int> dtt(6000, 12000);
    std::uniform_real_distribution<double> ud01(0.0, 1.0);

    // Evaluate initial
    evaluate(cur, nodes_cur, nodes_k_cur);
    best_nodes = nodes_cur;

    const int MAX_ITERS = 5000;
    double T0 = 200.0;
    for (int iter = 0; iter < MAX_ITERS; ++iter) {
        // propose neighbor (small random tweaks)
        Params cand = cur;
        if ((iter & 1) == 0) cand.pvs_margin = std::max(0, cur.pvs_margin + (int)(rng() % 31) - 15);
        if ((iter & 3) == 0) cand.asp_window = std::max(10, cur.asp_window + (int)(rng() % 61) - 30);
        cand.killer_weight = std::max(0, cur.killer_weight + (int)(rng() % 101) - 50);
        cand.tt_weight = std::max(1000, cur.tt_weight + (int)(rng() % 2001) - 1000);

        int nodes_comb, nodes_k;
        evaluate(cand, nodes_comb, nodes_k);

        // acceptance: prefer lower nodes_comb, but require nodes_k < nodes_base to keep killer helpful
        bool accept = false;
        if (nodes_k < nodes_base) {
            if (nodes_comb < nodes_cur) accept = true;
            else {
                // temperature schedule
                double t = T0 * (1.0 - (double)iter / MAX_ITERS);
                double prob = std::exp((nodes_cur - nodes_comb) / std::max(1.0, t));
                if (ud01(rng) < prob) accept = true;
            }
        }

        if (accept) {
            cur = cand;
            nodes_cur = nodes_comb;
            nodes_k_cur = nodes_k;
        }

        if (nodes_comb < best_nodes && nodes_k < nodes_base) {
            best_nodes = nodes_comb;
            best = cand;
            std::cout << "[NEW BEST] iter=" << iter << " nodes=" << nodes_comb << " pm=" << cand.pvs_margin
                      << " aw=" << cand.asp_window << " kw=" << cand.killer_weight << " tt=" << cand.tt_weight << "\n";
        }

        if ((iter & 63) == 0) {
            std::cout << "iter=" << iter << " cur_nodes=" << nodes_cur << " best_nodes=" << best_nodes << "\n";
        }
    }

    std::cout << "Best nodes=" << best_nodes << " pm=" << best.pvs_margin << " aw=" << best.asp_window
              << " kw=" << best.killer_weight << " tt=" << best.tt_weight << "\n";
    return 0;
}


