#include <pch.h>
#include "Projects/ProjectTwo.h"
#include "P2_Pathfinding.h"

#pragma region Extra Credit 
bool ProjectTwo::implemented_floyd_warshall()
{
    return false;
}

bool ProjectTwo::implemented_goal_bounding()
{
    return false;
}
#pragma endregion


//helper functions:
void AStarPather::reset_nodes()
{
    currentGeneration++;
    if (currentGeneration == 0) currentGeneration = 1;  // Handle overflow
    openHeap = decltype(openHeap)();  // Reset heap efficiently
}


bool AStarPather::has_line_of_sight(const GridPos& from, const GridPos& to)
{
    int x = from.col, y = from.row;
    int x1 = to.col, y1 = to.row;

    int dx = abs(x1 - x);
    int dy = abs(y1 - y);
    int sx = (x < x1) ? 1 : -1;
    int sy = (y < y1) ? 1 : -1;

    int err = dx - dy;

    while (x != x1 || y != y1) {
        // Check current position
        if (!terrain->is_valid_grid_position(y, x) || terrain->is_wall(y, x))
            return false;

        int e2 = 2 * err;
        bool will_move_x = (e2 > -dy && x != x1);
        bool will_move_y = (e2 < dx && y != y1);

        // If moving diagonally, check corner cutting prevention
        if (will_move_x && will_move_y) {
            GridPos adj1 = { y + sy, x };
            GridPos adj2 = { y, x + sx };

            if (!terrain->is_valid_grid_position(adj1) || terrain->is_wall(adj1) ||
                !terrain->is_valid_grid_position(adj2) || terrain->is_wall(adj2)) {
                return false;
            }
        }

        // Make the moves
        if (will_move_x) {
            err -= dy;
            x += sx;
        }
        if (will_move_y) {
            err += dx;
            y += sy;
        }
    }

    return terrain->is_valid_grid_position(y, x) && !terrain->is_wall(y, x);
}
void AStarPather::on_map_change()
{
    // Safety check - make sure terrain is valid
    if (!terrain) return;

    static const int DIRS[8][2] = {
        {-1, 0}, {-1, 1}, {0, 1}, {1, 1},
        {1, 0}, {1, -1}, {0, -1}, {-1, -1}
    };

    int height = terrain->get_map_height();
    int width = terrain->get_map_width();

    // Safety bounds check
    if (height <= 0 || width <= 0 || height > MAX_MAP_SIZE || width > MAX_MAP_SIZE) {
        return;
    }

    // Clear all existing neighbors
    for (int row = 0; row < MAX_MAP_SIZE; ++row) {
        for (int col = 0; col < MAX_MAP_SIZE; ++col) {
            neighbors[row][col].clear();
        }
    }

    // Pre-calculate valid neighbors for each node
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            // Initialize node position
            nodes[row][col].pos = { row, col };

            // Skip if current position is a wall
            if (terrain->is_wall(row, col)) continue;

            for (int i = 0; i < 8; ++i) {
                int newRow = row + DIRS[i][0];
                int newCol = col + DIRS[i][1];
                GridPos neighborPos{ newRow, newCol };

                // Check if neighbor is valid and not a wall
                if (!terrain->is_valid_grid_position(neighborPos) ||
                    terrain->is_wall(neighborPos)) {
                    continue;
                }

                // Check for illegal corner cutting on diagonal moves
                if (i % 2 == 1) {
                    GridPos adj1 = { row + DIRS[i][0], col };
                    GridPos adj2 = { row, col + DIRS[i][1] };
                    if (terrain->is_wall(adj1) || terrain->is_wall(adj2)) {
                        continue;
                    }
                }

                // This neighbor is valid, add it to the list
                neighbors[row][col].push_back(&nodes[newRow][newCol]);
            }
        }
    }
}


float AStarPather::heuristic(const GridPos& a, const GridPos& b, Heuristic type)
{
    int dx = abs(a.col - b.col);
    int dy = abs(a.row - b.row);

    switch (type)
    {
    case Heuristic::OCTILE:
        return static_cast<float>(std::max(dx, dy) + (SQRT_2 - 1.0f) * std::min(dx, dy));

    case Heuristic::CHEBYSHEV:
        return static_cast<float>(std::max(dx, dy));

    case Heuristic::INCONSISTENT:
        if ((a.row + a.col) % 2 > 0)
            return sqrtf(static_cast<float>(dx * dx + dy * dy));
        return 0.0f;

    case Heuristic::MANHATTAN:
        return static_cast<float>(dx + dy);

    case Heuristic::EUCLIDEAN:
        return sqrtf(static_cast<float>(dx * dx + dy * dy));

    default:
        return 0.0f;
    }
}

void AStarPather::apply_smoothing(PathRequest& request)
{
    if (request.path.size() < 4)
        return; // Need at least 4 points for Catmull-Rom spline

    WaypointList smoothed;

    // Convert to vector for easier access
    std::vector<Vec3> pathVec(request.path.begin(), request.path.end());

    // Always keep the first waypoint
    smoothed.push_back(pathVec[0]);

    // Process each segment using sliding window of 4 control points
    // Continue until we've processed the segment between the last two waypoints
    for (size_t i = 0; i <= pathVec.size() - 4; ++i)
    {
        // Current 4 control points for Catmull-Rom
        Vec3 v1 = pathVec[i];     // p1 (previous point)
        Vec3 v2 = pathVec[i + 1]; // p2 (start of current segment)
        Vec3 v3 = pathVec[i + 2]; // p3 (end of current segment)
        Vec3 v4 = pathVec[i + 3]; // p4 (next point)

        // Add interpolated points between v2 and v3
        for (int t_step = 1; t_step <= 3; ++t_step)
        {
            float t = t_step / 4.0f; // t = 0.25, 0.5, 0.75

            // Use SimpleMath.h CatmullRom function
            Vec3 interpolatedPoint = Vec3::CatmullRom(v1, v2, v3, v4, t);
            smoothed.push_back(interpolatedPoint);
        }

        // Add the end point of current segment (v3)
        smoothed.push_back(v3);
    }

    request.path = std::move(smoothed);
}


bool AStarPather::is_area_clear(const GridPos& point1, const GridPos& point2, const GridPos& point3)
{
    // Find the bounding box that includes ALL three points
    // This ensures we check the area that could affect diagonal movement
    int min_row = std::min({ point1.row, point2.row, point3.row });
    int max_row = std::max({ point1.row, point2.row, point3.row });
    int min_col = std::min({ point1.col, point2.col, point3.col });
    int max_col = std::max({ point1.col, point2.col, point3.col });

    // Check every grid cell in the bounding rectangle of all 3 points
    for (int row = min_row; row <= max_row; ++row) {
        for (int col = min_col; col <= max_col; ++col) {
            if ( terrain->is_wall(row, col)) {
                return false; // Found a wall or invalid position
            }
        }
    }

    return true; // Entire area is clear
}

void AStarPather::apply_rubberbanding(PathRequest& request)
{
    if (request.path.size() < 3) return;

    // Convert path to vector for easier access
    std::vector<Vec3> pathVec(request.path.begin(), request.path.end());
    WaypointList result;

    // Start from the beginning (forward approach)
    int current = 0;
    result.push_back(pathVec[current]); // Always keep start point

    while (current < pathVec.size() - 2) { // Need at least 2 more points ahead
        int next_target = current + 1; // Default: move to next waypoint

        // Try to find the farthest point we can reach from current position
        for (int target = current + 2; target < pathVec.size(); target++) {
            // Get grid positions for the 3 waypoints: current, next, target
            GridPos currentPos = terrain->get_grid_position(pathVec[current]);
            GridPos middlePos = terrain->get_grid_position(pathVec[target - 1]); // The point we're considering skipping
            GridPos targetPos = terrain->get_grid_position(pathVec[target]);

            // Check if we can skip the middle point and go directly to target
            if (terrain->is_valid_grid_position(currentPos) &&
                terrain->is_valid_grid_position(targetPos) &&
                is_area_clear(currentPos, middlePos, targetPos)) {
                next_target = target; // We can skip the middle and reach this target
                // The middle node is effectively removed by not adding it to result
            }
            else {
                break; // Can't reach further, stop trying
            }
        }

        // Move to the chosen target (could be next point or a farther point)
        current = next_target;
        result.push_back(pathVec[current]);
    }

    // Make sure we have the goal point (if not already added)
    if (current < pathVec.size() - 1) {
        result.push_back(pathVec.back());
    }

    request.path = WaypointList(result.begin(), result.end());
}



void AStarPather::apply_smooth_rubberband(PathRequest& request)
{
    if (request.path.size() < 3) return;

    // Step 1: Apply standard rubberbanding
    WaypointList result;
    std::vector<Vec3> pathVec(request.path.begin(), request.path.end());

    // Start from goal (last index)
    int current = pathVec.size() - 1;
    result.push_front(pathVec[current]); // Always keep goal

    while (current > 1) { // Need at least 2 more points to check
        int best_target = current - 1; // Default: move to adjacent waypoint

        // Try to find the farthest point we can reach (skip multiple waypoints)
        for (int target = current - 2; target >= 0; target -= 1) {
            // Get grid positions for the 3 waypoints
            GridPos currentPos = terrain->get_grid_position(pathVec[current]);
            GridPos middlePos = terrain->get_grid_position(pathVec[current - 1]); // Always check the immediate middle
            GridPos targetPos = terrain->get_grid_position(pathVec[target]);

            // Check if the entire rectangular area is clear
            if (terrain->is_valid_grid_position(currentPos) &&
                terrain->is_valid_grid_position(targetPos) &&
                is_area_clear(currentPos, middlePos, targetPos)) {
                best_target = target; // We can reach this far
            }
            else {
                break; // Can't reach further, stop trying
            }
        }

        current = best_target;
        result.push_front(pathVec[current]);
    }

    // Make sure we have the start point
    if (current > 0) {
        result.push_front(pathVec[0]);
    }

    // Step 2: Add points back in when distance > 1.5 grid squares
    WaypointList densified;
    auto it = result.begin();
    densified.push_back(*it); // Keep first point

    auto prev = it++;
    while (it != result.end()) {
        Vec3 prevPoint = *prev;
        Vec3 currPoint = *it;

        // Calculate distance in grid squares
        GridPos prevGrid = terrain->get_grid_position(prevPoint);
        GridPos currGrid = terrain->get_grid_position(currPoint);

        float dx = static_cast<float>(currGrid.col - prevGrid.col);
        float dy = static_cast<float>(currGrid.row - prevGrid.row);
        float distance = sqrtf(dx * dx + dy * dy);

        if (distance > 1.5f) {
            // Add intermediate points
            int numSegments = static_cast<int>(distance / 1.5f) + 1;

            for (int i = 1; i < numSegments; ++i) {
                float t = static_cast<float>(i) / static_cast<float>(numSegments);
                Vec3 interpolatedPoint = prevPoint + t * (currPoint - prevPoint);
                densified.push_back(interpolatedPoint);
            }
        }

        densified.push_back(currPoint);
        prev = it++;
    }

    // Step 3: Apply Catmull-Rom smoothing
    request.path = std::move(densified);

    if (request.path.size() >= 4) {
        apply_smoothing(request);
    }
}

bool AStarPather::initialize()
{
    // Listen for map changes to rebuild neighbor cache
    Callback cb = std::bind(&AStarPather::on_map_change, this);
    Messenger::listen_for_message(Messages::MAP_CHANGE, cb);

    // Don't call on_map_change() here - let it be called when the map is actually ready

    return true;
}

void AStarPather::shutdown()
{
    /*
        Free any dynamically allocated memory or any other general house-
        keeping you need to do during shutdown.
    */
}

PathResult AStarPather::compute_path(PathRequest& request)
{
    // Convert world-space to grid-space
    GridPos start = terrain->get_grid_position(request.start);
    GridPos goal = terrain->get_grid_position(request.goal);

    if (!terrain->is_valid_grid_position(start) || terrain->is_wall(start) ||
        !terrain->is_valid_grid_position(goal) || terrain->is_wall(goal))
        return PathResult::IMPOSSIBLE;

    if (request.newRequest)
    {
        reset_nodes();

        Node* startNode = &nodes[start.row][start.col];
        if (!startNode->isValidForGeneration(currentGeneration)) {
            startNode->pos = start;
            startNode->generation = currentGeneration;
        }
        startNode->gCost = 0.0f;
        startNode->hCost = heuristic(start, goal, request.settings.heuristic) * request.settings.weight;
        startNode->parent = nullptr;
        startNode->state = NodeState::Open;
        openHeap.push(startNode);;

        request.newRequest = false;
    }

    static const int DIRS[8][2] = {
        {-1, 0}, {-1, 1}, {0, 1}, {1, 1},
        {1, 0}, {1, -1}, {0, -1}, {-1, -1}
    };

    while (!openHeap.empty())
    {
        Node* current = nullptr;

        // Pop until a non-closed node is found
        while (!openHeap.empty())
        {
            current = openHeap.top();
            openHeap.pop();

            if (current->state != NodeState::Closed)
                break;

            current = nullptr;
        }

        if (!current)
            return PathResult::IMPOSSIBLE;

        current->state = NodeState::Closed;

        if (request.settings.debugColoring)
            terrain->set_color(current->pos, Colors::Yellow);

        if (current->pos == goal)
        {
            // Reconstruct path
            for (Node* n = current; n; n = n->parent)
                request.path.push_front(terrain->get_world_position(n->pos));

            if (request.settings.rubberBanding && request.settings.smoothing) {
                apply_smooth_rubberband(request);
            }
            else {
                if (request.settings.rubberBanding)
                    apply_rubberbanding(request);
                if (request.settings.smoothing)
                    apply_smoothing(request);
            }

            return PathResult::COMPLETE;
        }

        // Replace the entire neighbor processing section with:
        for (Node* neighbor : neighbors[current->pos.row][current->pos.col]) {
            if (!neighbor->isValidForGeneration(currentGeneration)) {
                // Initialize for this search
                neighbor->pos = { neighbor->pos.row, neighbor->pos.col }; // pos already set
                neighbor->gCost = FLT_MAX;
                neighbor->hCost = 0.0f;
                neighbor->parent = nullptr;
                neighbor->state = NodeState::Unvisited;
                neighbor->generation = currentGeneration;
            }

            // Calculate movement cost (diagonal vs cardinal)
            int dx = abs(current->pos.col - neighbor->pos.col);
            int dy = abs(current->pos.row - neighbor->pos.row);
            float moveCost = (dx + dy == 1) ? 1.0f : 1.41421356f; // cardinal vs diagonal

            float tentativeG = current->gCost + moveCost;

            bool useEqualComparison = (request.settings.heuristic == Heuristic::INCONSISTENT ||
                request.settings.heuristic == Heuristic::CHEBYSHEV);

            if ((useEqualComparison && tentativeG <= neighbor->gCost) ||
                (!useEqualComparison && tentativeG < neighbor->gCost))
            {
                neighbor->gCost = tentativeG;
                neighbor->hCost = heuristic(neighbor->pos, goal, request.settings.heuristic) * request.settings.weight;
                neighbor->parent = current;
                neighbor->state = NodeState::Open;
                openHeap.push(neighbor);

                if (request.settings.debugColoring)
                    terrain->set_color(neighbor->pos, Colors::Blue);
            }
        }

        if (request.settings.singleStep)
            return PathResult::PROCESSING;
    }

    return PathResult::IMPOSSIBLE;
}