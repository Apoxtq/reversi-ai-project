#include "../src/core/Board.hpp"
#include "ai/MinimaxEngine.hpp"
#include <iostream>
#include <vector>
#include <tuple>

using namespace reversi::core;
using namespace reversi::ai;

void print_result(const std::string &name, const MinimaxEngine::SearchResult &r) {
    std::cout << name << " nodes=" << r.nodes_searched
              << " depth=" << r.depth_reached
              << " time_ms=" << r.time_ms << "\n";
}

int main() {
    Board board;

    // Baseline (no optimizations)
    MinimaxEngine::Config baseline;
    baseline.max_depth = 5;
    baseline.use_iterative_deepening = false;
    baseline.use_pvs = false;
    baseline.use_aspiration = false;
    baseline.use_killer_moves = false;
    MinimaxEngine eng_base(baseline);
    auto r_base = eng_base.find_best_move(board);
    print_result("BASELINE", r_base);
    int nodes_prev = r_base.nodes_searched;

    // Stage 1: enable Killer only
    MinimaxEngine::Config cfg_k = baseline;
    cfg_k.use_killer_moves = true;
    cfg_k.max_depth = 5;
    MinimaxEngine eng_k(cfg_k);
    auto r_k = eng_k.find_best_move(board);
    print_result("KILLER_ONLY", r_k);
    if (r_k.nodes_searched < nodes_prev) {
        nodes_prev = r_k.nodes_searched;
        std::cout << "[STAGE1] Accept killer\n";
    } else {
        std::cout << "[STAGE1] Killer did not improve, keep baseline\n";
    }

    // Stage 2: enable PVS and tune pvs_research_margin
    int best_margin = 0;
    int best_nodes = nodes_prev;
    for (int margin : {0,5,10,20,50}) {
        MinimaxEngine::Config cfg = cfg_k;
        cfg.use_pvs = true;
        cfg.pvs_research_margin = margin;
        MinimaxEngine e(cfg);
        auto r = e.find_best_move(board);
        std::cout << "PVS margin=" << margin << " nodes=" << r.nodes_searched << "\n";
        if (r.nodes_searched < best_nodes) {
            best_nodes = r.nodes_searched;
            best_margin = margin;
        }
    }
    if (best_nodes < nodes_prev) {
        cfg_k.use_pvs = true;
        cfg_k.pvs_research_margin = best_margin;
        nodes_prev = best_nodes;
        std::cout << "[STAGE2] Accept PVS margin=" << best_margin << " nodes=" << best_nodes << "\n";
    } else {
        std::cout << "[STAGE2] PVS did not improve; keep previous\n";
    }

    // Stage 3: enable Aspiration and tune window
    int best_aw = 0;
    best_nodes = nodes_prev;
    for (int aw : {25,50,100,200}) {
        MinimaxEngine::Config cfg = cfg_k;
        cfg.use_aspiration = true;
        cfg.aspiration_window = aw;
        MinimaxEngine e(cfg);
        auto r = e.find_best_move(board);
        std::cout << "ASP window=" << aw << " nodes=" << r.nodes_searched << "\n";
        if (r.nodes_searched < best_nodes) {
            best_nodes = r.nodes_searched;
            best_aw = aw;
        }
    }
    if (best_nodes < nodes_prev) {
        cfg_k.use_aspiration = true;
        cfg_k.aspiration_window = best_aw;
        nodes_prev = best_nodes;
        std::cout << "[STAGE3] Accept Aspiration window=" << best_aw << " nodes=" << best_nodes << "\n";
    } else {
        std::cout << "[STAGE3] Aspiration did not improve; keep previous\n";
    }

    // Stage 4: enable Iterative Deepening (ID) and tune small PVS/ID params to avoid re-search explosion
    MinimaxEngine::Config best_cfg = cfg_k;
    best_cfg.use_iterative_deepening = true;
    best_cfg.max_depth = 6;
    int best_final_nodes = INT_MAX;
    int best_margin_id = cfg_k.pvs_research_margin;
    int best_fail_thresh = 4;

    // Broader grid search for ID to find robust parameters (may take time)
    for (int margin : {0,5,10,20,50}) {
        for (int fail_t : {1,2,4}) {
            for (int kw : {0,25,50,100}) {
                for (int tt : {1000,3000,6000}) {
                    for (int aw : {0,25,50,100}) {
                        MinimaxEngine::Config cfg = cfg_k;
                        cfg.use_iterative_deepening = true;
                        cfg.max_depth = 6;
                        cfg.pvs_research_margin = margin;
                        cfg.pvs_failure_threshold = fail_t;
                        cfg.killer_weight = kw;
                        cfg.tt_weight = tt;
                        cfg.use_aspiration = (aw > 0);
                        cfg.aspiration_window = aw;
                        MinimaxEngine e(cfg);
                        auto r = e.find_best_move(board);
                        std::cout << "ID test pm=" << margin << " ft=" << fail_t << " kw=" << kw << " tt=" << tt << " aw=" << aw << " -> nodes=" << r.nodes_searched << "\n";
                        if (r.nodes_searched < best_final_nodes) {
                            best_final_nodes = r.nodes_searched;
                            best_margin_id = margin;
                            best_fail_thresh = fail_t;
                            best_cfg = cfg;
                        }
                    }
                }
            }
        }
    }

    auto final_engine = MinimaxEngine(best_cfg);
    auto final_res = final_engine.find_best_move(board);
    print_result("ID_WITH_BEST", final_res);
    if (final_res.nodes_searched < nodes_prev) {
        std::cout << "[STAGE4] ID improved with margin=" << best_margin_id << " fail_t=" << best_fail_thresh << "\n";
    } else {
        std::cout << "[STAGE4] ID did not improve; consider disabling ID or further tuning\n";
    }

    std::cout << "[SUMMARY] final nodes=" << final_res.nodes_searched << " baseline=" << r_base.nodes_searched << "\n";
    std::cout << "[BEST CFG] pvs_margin=" << best_cfg.pvs_research_margin
              << " pvs_fail_t=" << best_cfg.pvs_failure_threshold
              << " killer_w=" << best_cfg.killer_weight
              << " tt_w=" << best_cfg.tt_weight
              << " use_asp=" << best_cfg.use_aspiration
              << " asp_w=" << best_cfg.aspiration_window
              << " id=" << best_cfg.use_iterative_deepening << "\n";
    return 0;
}


