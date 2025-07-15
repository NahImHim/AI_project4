#pragma once
#include "Misc/PathfindingDetails.hpp"
#include <queue>
#include <functional>



class AStarPather
{
public:
    /* 
        The class should be default constructible, so you might need to define a constructor.
        If needed, you can modify the framework where the class is constructed in the
        initialize functions of ProjectTwo and ProjectThree.
    */

    /* ************************************************** */
    // DO NOT MODIFY THESE SIGNATURES
    bool initialize();
    void shutdown();
    PathResult compute_path(PathRequest &request);
    /* ************************************************** */

    static constexpr float CARDINAL_COST = 1.0f;
    static constexpr float DIAGONAL_COST = 1.41421356f;

    // Heuristic constants
    static constexpr float SQRT_2 = 1.41421356f;
    static constexpr float SQRT_2_MINUS_1 = 0.41421356f;

    // let each grid be a node, if not very messy
    enum class NodeState { Unvisited, Open, Closed };
    struct Node {
        GridPos pos;
        float gCost, hCost;
        Node* parent;
        NodeState state;  // Unvisited, Open, Closed
        std::vector<Node*> neighbors;  // optional
        bool inOpenList = false;
       // int generation = 0;
        int generation = 0;  // 0 means "never initialized" - ADD THIS LINE

        float fCost() const { return gCost + hCost; }

        void reset() {
            gCost = 0.0f;
            hCost = 0.0f;
            parent = nullptr;
            state = NodeState::Unvisited;
            inOpenList = false;
            int generation = 0;  // 0 means "never initialized" - ADD THIS LINE


        }
        bool isValidForGeneration(int gen) const {
            return generation == gen;
        }

    };
    struct CompareNode {
        bool operator()(const Node* a, const Node* b) const {
            return a->fCost() > b->fCost();  // min-heap based on fCost
        }
    };

    std::priority_queue<Node*, std::vector<Node*>, CompareNode> openHeap;

    

    //std::vector<Node*> openList;


    //helper functions 
    void reset_nodes();
    float heuristic(const GridPos& a, const GridPos& b, Heuristic type);
    bool has_line_of_sight(const GridPos& from, const GridPos& to);
    void apply_rubberbanding(PathRequest& request);
    void apply_smoothing(PathRequest& request);
    bool is_area_clear(const GridPos& point1, const GridPos& point2, const GridPos& point3);
    void apply_smooth_rubberband(PathRequest& request);



private:
        int currentGeneration = 1;
        static const int MAX_MAP_SIZE = 40;
        Node nodes[MAX_MAP_SIZE][MAX_MAP_SIZE];
        std::vector<Node*> neighbors[MAX_MAP_SIZE][MAX_MAP_SIZE];

        // Map change callback
        void on_map_change();
    /*
        You should create whatever functions, variables, or classes you need.
        It doesn't all need to be in this header and cpp, structure it whatever way
        makes sense to you.
    */

};