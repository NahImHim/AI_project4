#include <pch.h>
#include "Terrain/TerrainAnalysis.h"
#include "Terrain/MapMath.h"
#include "Agent/AStarAgent.h"
#include "Terrain/MapLayer.h"
#include "Projects/ProjectThree.h"

#include <iostream>
#include <cmath>
#include <algorithm>
#include <cfloat>
bool ProjectThree::implemented_fog_of_war() const // extra credit
{
    return false;
}

const float WALL_EPSILON = 0.001f;

// Helper function for line intersection (you need to implement this too)
bool line_intersect(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
    float denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    // Lines are parallel
    if (std::abs(denom) < 1e-10f) {
        return false;
    }

    float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denom;
    float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / denom;

    // Both parameters must be in [0,1] for intersection within segments
    return (t >= -1e-6f && t <= 1.0f + 1e-6f && u >= -1e-6f && u <= 1.0f + 1e-6f);
}

bool line_intersects_wall_cell(float startX, float startY, float endX, float endY, int wallRow, int wallCol)
{
    // Get puffed cell boundaries (slightly expanded to catch edge cases)
    float left = static_cast<float>(wallCol) - WALL_EPSILON;
    float right = static_cast<float>(wallCol + 1) + WALL_EPSILON;
    float top = static_cast<float>(wallRow) - WALL_EPSILON;
    float bottom = static_cast<float>(wallRow + 1) + WALL_EPSILON;

    // Check intersection with all four edges of the puffed wall cell
    bool intersects =
        line_intersect(startX, startY, endX, endY, left, top, left, bottom) ||     // Left edge
        line_intersect(startX, startY, endX, endY, right, top, right, bottom) ||   // Right edge  
        line_intersect(startX, startY, endX, endY, left, top, right, top) ||       // Top edge
        line_intersect(startX, startY, endX, endY, left, bottom, right, bottom);   // Bottom edge

    return intersects;
}

float distance_to_closest_wall(int row, int col)
{
    float minDistance = FLT_MAX;
    int height = terrain->get_map_height();
    int width = terrain->get_map_width();

    // Check distance to all cells (including boundaries)
    // Expand search to include boundary cells as walls
    for (int checkRow = -1; checkRow <= height; ++checkRow) {
        for (int checkCol = -1; checkCol <= width; ++checkCol) {

            bool isWall = false;

            // Check if this position is a wall
            if (checkRow < 0 || checkRow >= height || checkCol < 0 || checkCol >= width) {
                // Outside map bounds - treat as wall
                isWall = true;
            }
            else if (terrain->is_wall(checkRow, checkCol)) {
                // Inside map and is a wall
                isWall = true;
            }

            if (isWall) {
                float dx = static_cast<float>(col - checkCol);
                float dy = static_cast<float>(row - checkRow);
                float distance = sqrtf(dx * dx + dy * dy);
                minDistance = std::min(minDistance, distance);
            }
        }
    }

    return minDistance;
}


bool linePassesThroughCell(float x0, float y0, float x1, float y1, int row, int col)
{
    // Cell boundaries
    float left = static_cast<float>(col);
    float right = static_cast<float>(col + 1);
    float top = static_cast<float>(row);
    float bottom = static_cast<float>(row + 1);

    // Check if line intersects any of the four cell edges
    bool intersects =
        line_intersect(x0, y0, x1, y1, left, top, left, bottom) ||     // Left edge
        line_intersect(x0, y0, x1, y1, right, top, right, bottom) ||   // Right edge  
        line_intersect(x0, y0, x1, y1, left, top, right, top) ||       // Top edge
        line_intersect(x0, y0, x1, y1, left, bottom, right, bottom);   // Bottom edge

    if (intersects) {
        return true;
    }

    // Also check if either endpoint is inside the cell
    bool startInside = (x0 >= left && x0 <= right && y0 >= top && y0 <= bottom);
    bool endInside = (x1 >= left && x1 <= right && y1 >= top && y1 <= bottom);

    return startInside || endInside;
}
bool is_clear_path(int row0, int col0, int row1, int col1)
{
    const float EPSILON = 1e-3f;

    float x0 = static_cast<float>(col0) + 0.5f;
    float y0 = static_cast<float>(row0) + 0.5f;
    float x1 = static_cast<float>(col1) + 0.5f;
    float y1 = static_cast<float>(row1) + 0.5f;

    int height = terrain->get_map_height();
    int width = terrain->get_map_width();

    // Handle all in-bounds wall cells
    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            if (!terrain->is_wall(row, col))
                continue;

            float left = static_cast<float>(col) - EPSILON;
            float right = static_cast<float>(col + 1) + EPSILON;
            float top = static_cast<float>(row) - EPSILON;
            float bottom = static_cast<float>(row + 1) + EPSILON;

            if (line_intersect(x0, y0, x1, y1, left, top, right, top)) return false;
            if (line_intersect(x0, y0, x1, y1, left, bottom, right, bottom)) return false;
            if (line_intersect(x0, y0, x1, y1, left, top, left, bottom)) return false;
            if (line_intersect(x0, y0, x1, y1, right, top, right, bottom)) return false;
        }
    }

    // Handle map boundaries (rows -1 and height, cols -1 and width)
    // Four strips: top, bottom, left, right
    for (int col = -1; col <= width; ++col)
    {
        float left = static_cast<float>(col) - EPSILON;
        float right = static_cast<float>(col + 1) + EPSILON;

        // Top boundary at row = -1
        float top = -1.0f - EPSILON;
        float bottom = 0.0f + EPSILON;

        if (line_intersect(x0, y0, x1, y1, left, top, right, top)) return false;
        if (line_intersect(x0, y0, x1, y1, left, bottom, right, bottom)) return false;
        if (line_intersect(x0, y0, x1, y1, left, top, left, bottom)) return false;
        if (line_intersect(x0, y0, x1, y1, right, top, right, bottom)) return false;

        // Bottom boundary at row = height
        top = static_cast<float>(height) - EPSILON;
        bottom = static_cast<float>(height + 1) + EPSILON;

        if (line_intersect(x0, y0, x1, y1, left, top, right, top)) return false;
        if (line_intersect(x0, y0, x1, y1, left, bottom, right, bottom)) return false;
        if (line_intersect(x0, y0, x1, y1, left, top, left, bottom)) return false;
        if (line_intersect(x0, y0, x1, y1, right, top, right, bottom)) return false;
    }

    for (int row = -1; row <= height; ++row)
    {
        float top = static_cast<float>(row) - EPSILON;
        float bottom = static_cast<float>(row + 1) + EPSILON;

        // Left boundary at col = -1
        float left = -1.0f - EPSILON;
        float right = 0.0f + EPSILON;

        if (line_intersect(x0, y0, x1, y1, left, top, right, top)) return false;
        if (line_intersect(x0, y0, x1, y1, left, bottom, right, bottom)) return false;
        if (line_intersect(x0, y0, x1, y1, left, top, left, bottom)) return false;
        if (line_intersect(x0, y0, x1, y1, right, top, right, bottom)) return false;

        // Right boundary at col = width
        left = static_cast<float>(width) - EPSILON;
        right = static_cast<float>(width + 1) + EPSILON;

        if (line_intersect(x0, y0, x1, y1, left, top, right, top)) return false;
        if (line_intersect(x0, y0, x1, y1, left, bottom, right, bottom)) return false;
        if (line_intersect(x0, y0, x1, y1, left, top, left, bottom)) return false;
        if (line_intersect(x0, y0, x1, y1, right, top, right, bottom)) return false;
    }

    return true;
}



// Helper function to check if line passes through a specific cell


void analyze_openness(MapLayer<float>& layer)
{
    /*
        Mark every cell in the given layer with the value 1 / (d * d),
        where d is the distance to the closest wall or edge.  Make use of the
        distance_to_closest_wall helper function.  Walls should not be marked.
    */

    /*
         Mark every cell with value 1 / (d * d), where d is distance to closest wall.
         Walls should not be marked.
     */

    int height = terrain->get_map_height();
    int width = terrain->get_map_width();

    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            if (terrain->is_wall(row, col)) {
                // Don't mark walls
                continue;
            }

            float distance = distance_to_closest_wall(row, col);
            if (distance > 0.0f) {
                float openness = 1.0f / (distance * distance);
                layer.set_value(row, col, openness);
            }
        }
    }
}

void analyze_visibility(MapLayer<float>& layer)
{
    int height = terrain->get_map_height();
    int width = terrain->get_map_width();

    // Precompute all non-wall cells
    std::vector<std::pair<int, int>> openCells;
    openCells.reserve(height * width);

    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            if (terrain->is_wall(row, col))
            {
                layer.set_value(row, col, 0.0f); // Walls always 0
            }
            else
            {
                openCells.emplace_back(row, col);
            }
        }
    }

    // For each open cell, count how many other cells it can see
    for (const auto& cell : openCells)
    {
        int row0 = cell.first;
        int col0 = cell.second;

        int visibleCount = 0;

        for (const auto& target : openCells)
        {
            int row1 = target.first;
            int col1 = target.second;

            if (row0 == row1 && col0 == col1)
                continue;

            if (is_clear_path(row0, col0, row1, col1))
            {
                ++visibleCount;
                if (visibleCount >= 160)
                    break;
            }
        }

        float visibility = static_cast<float>(visibleCount) / 160.0f;
        visibility = std::min(visibility, 1.0f);
        layer.set_value(row0, col0, visibility);
    }
}




void analyze_visible_to_cell(MapLayer<float>& layer, int row, int col)
{
    int height = terrain->get_map_height();
    int width = terrain->get_map_width();
    if (row >= 0 && row < height && col >= 0 && col < width)
    {
        layer.set_value(row, col, 1.0f);

        // If standing on a wall, no other tiles are visible
        if (terrain->is_wall(row, col))
            return;
    }
    else
    {
        return; // Out of bounds, nothing to do
    }
    // First pass: mark visible cells using robust LOS
    std::vector<std::vector<bool>> isVisible(height, std::vector<bool>(width, false));

    for (int targetRow = 0; targetRow < height; ++targetRow) {
        for (int targetCol = 0; targetCol < width; ++targetCol) {
            if (terrain->is_wall(targetRow, targetCol)) continue;

            if (is_clear_path(row, col, targetRow, targetCol)) {
                isVisible[targetRow][targetCol] = true;
                layer.set_value(targetRow, targetCol, 1.0f);
            }
            else {
                layer.set_value(targetRow, targetCol, 0.0f);
            }
        }
    }

    // Second pass: mark cells adjacent to visible cells
    static const int dirs[8][2] = { {-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1} };

    for (int targetRow = 0; targetRow < height; ++targetRow) {
        for (int targetCol = 0; targetCol < width; ++targetCol) {
            if (terrain->is_wall(targetRow, targetCol)) continue;
            if (isVisible[targetRow][targetCol]) continue; // Already visible

            // Check if adjacent to any visible cell with proper diagonal blocking
            bool adjacentToVisible = false;
            for (int i = 0; i < 8; ++i) {
                int adjRow = targetRow + dirs[i][0];
                int adjCol = targetCol + dirs[i][1];

                if (adjRow >= 0 && adjRow < height && adjCol >= 0 && adjCol < width) {
                    if (isVisible[adjRow][adjCol]) {
                        // Check if this is a diagonal direction
                        bool isDiagonal = (dirs[i][0] != 0 && dirs[i][1] != 0);

                        if (isDiagonal) {
                            // For diagonal adjacency, check that both orthogonal neighbors are clear
                            int ortho1Row = targetRow + dirs[i][0]; // Only row movement
                            int ortho1Col = targetCol;              // Keep same col
                            int ortho2Row = targetRow;              // Keep same row
                            int ortho2Col = targetCol + dirs[i][1]; // Only col movement

                            // Check if either orthogonal cell is a wall (or out of bounds)
                            bool ortho1IsWall = (ortho1Row < 0 || ortho1Row >= height ||
                                ortho1Col < 0 || ortho1Col >= width ||
                                terrain->is_wall(ortho1Row, ortho1Col));

                            bool ortho2IsWall = (ortho2Row < 0 || ortho2Row >= height ||
                                ortho2Col < 0 || ortho2Col >= width ||
                                terrain->is_wall(ortho2Row, ortho2Col));

                            // Only allow diagonal adjacency if both orthogonal paths are clear
                            if (!ortho1IsWall && !ortho2IsWall) {
                                adjacentToVisible = true;
                                break;
                            }
                        }
                        else {
                            // Orthogonal adjacency is always allowed
                            adjacentToVisible = true;
                            break;
                        }
                    }
                }
            }

            if (adjacentToVisible) {
                layer.set_value(targetRow, targetCol, 0.5f);
            }
        }
    }
}




void analyze_agent_vision(MapLayer<float>& layer, const Agent* agent)
{
    int height = terrain->get_map_height();
    int width = terrain->get_map_width();

    // Clear layer
    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            layer.set_value(row, col, 0.0f);
        }
    }

    Vec3 agentPos = agent->get_position();
    Vec3 agentForward = agent->get_forward_vector();
    Vec2 agentPos2D(agentPos.x, agentPos.z);
    Vec2 agentForward2D(agentForward.x, agentForward.z);
    agentForward2D.Normalize();

    GridPos agentGrid = terrain->get_grid_position(agentPos);

    // Always mark agent's current cell
    if (agentGrid.row >= 0 && agentGrid.row < height && agentGrid.col >= 0 && agentGrid.col < width)
    {
        layer.set_value(agentGrid.row, agentGrid.col, 1.0f);

        // If standing on a wall, don't process further
        if (terrain->is_wall(agentGrid.row, agentGrid.col))
            return;
    }
    else
    {
        return; // Outside map bounds
    }

    const float FOV_COSINE = cosf(95.0f * 3.14159f / 180.0f);

    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            if (terrain->is_wall(row, col)) continue;
            if (row == agentGrid.row && col == agentGrid.col) continue;

            if (!is_clear_path(agentGrid.row, agentGrid.col, row, col)) continue;

            Vec3 cellWorldPos = terrain->get_world_position(row, col);
            Vec2 cellPos2D(cellWorldPos.x, cellWorldPos.z);

            Vec2 toCell = cellPos2D - agentPos2D;
            float distance = toCell.Length();
            if (distance < 0.01f) continue;

            toCell.Normalize();
            float dotProduct = agentForward2D.Dot(toCell);

            if (dotProduct >= FOV_COSINE)
            {
                layer.set_value(row, col, 1.0f);
            }
        }
    }
}



void propagate_solo_occupancy(MapLayer<float>& layer, float decay, float growth)
{
    int height = terrain->get_map_height();
    int width = terrain->get_map_width();

    float temp[40][40];

    static const int dirs[8][2] = {
        {-1,-1}, {-1,0}, {-1,1},
        { 0,-1},         { 0,1},
        { 1,-1}, { 1,0}, { 1,1}
    };

    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            if (terrain->is_wall(row, col))
            {
                temp[row][col] = layer.get_value(row, col);
                continue;
            }

            float currentValue = layer.get_value(row, col);
            float maxNeighborInfluence = 0.0f;

            for (int i = 0; i < 8; ++i)
            {
                int nRow = row + dirs[i][0];
                int nCol = col + dirs[i][1];

                if (nRow < 0 || nRow >= height || nCol < 0 || nCol >= width)
                    continue;

                float neighborValue = layer.get_value(nRow, nCol);
                if (neighborValue <= 0.0f) continue; // Ignore walls and negative values

                float distance = (dirs[i][0] == 0 || dirs[i][1] == 0) ? 1.0f : 1.414213562f;
                float decayedInfluence = neighborValue * expf(-1.0f * distance * decay);

                maxNeighborInfluence = std::max(maxNeighborInfluence, decayedInfluence);
            }

            // Linear interpolation
            float newInfluence = (1.0f - growth) * currentValue + growth * maxNeighborInfluence;
            if (newInfluence < 0.0f) newInfluence = 0.0f;

            temp[row][col] = newInfluence;
        }
    }

    // Copy back to main layer
    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            layer.set_value(row, col, temp[row][col]);
        }
    }
}


void normalize_solo_occupancy(MapLayer<float>& layer)
{
    int height = terrain->get_map_height();
    int width = terrain->get_map_width();

    float maxValue = 0.0f;

    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            float value = layer.get_value(row, col);
            if (value > maxValue)
                maxValue = value;
        }
    }

    if (maxValue > 0.0f)
    {
        for (int row = 0; row < height; ++row)
        {
            for (int col = 0; col < width; ++col)
            {
                float value = layer.get_value(row, col);
                if (value > 0.0f)
                    layer.set_value(row, col, value / maxValue);
            }
        }
    }
}

void enemy_field_of_view(MapLayer<float>& layer, float fovAngle, float closeDistance, float occupancyValue, AStarAgent* enemy)
{
    int height = terrain->get_map_height();
    int width = terrain->get_map_width();

    // Step 1: Clear old negative occupancy values
    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            if (layer.get_value(row, col) < 0.0f)
            {
                layer.set_value(row, col, 0.0f);
            }
        }
    }

    Vec3 enemyPos = enemy->get_position();
    Vec3 enemyForward = enemy->get_forward_vector();
    Vec2 enemyPos2D(enemyPos.x, enemyPos.z);
    Vec2 enemyForward2D(enemyForward.x, enemyForward.z);
    enemyForward2D.Normalize();

    GridPos enemyGrid = terrain->get_grid_position(enemyPos);

    const float fovCosine = cosf(fovAngle * 3.14159f / 180.0f / 2.0f); // Half FOV angle

    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            if (terrain->is_wall(row, col)) continue;

            // First, check LOS
            if (!is_clear_path(enemyGrid.row, enemyGrid.col, row, col))
                continue;

            // Grid distance for quick close-range check
            float dRow = static_cast<float>(row - enemyGrid.row);
            float dCol = static_cast<float>(col - enemyGrid.col);
            float gridDistance = sqrtf(dRow * dRow + dCol * dCol);

            bool inDetectionZone = false;

            if (gridDistance < closeDistance)
            {
                inDetectionZone = true;
            }
            else
            {
                // Need FOV cone check
                Vec3 cellWorldPos = terrain->get_world_position(row, col);
                Vec2 cellPos2D(cellWorldPos.x, cellWorldPos.z);

                Vec2 toCell = cellPos2D - enemyPos2D;
                float distance = toCell.Length();
                if (distance < 0.01f) continue;

                toCell.Normalize();

                float dotProduct = enemyForward2D.Dot(toCell);
                if (dotProduct >= fovCosine)
                {
                    inDetectionZone = true;
                }
            }

            if (inDetectionZone)
            {
                layer.set_value(row, col, occupancyValue);
            }
        }
    }
}


bool enemy_find_player(MapLayer<float>& layer, AStarAgent* enemy, Agent* player)
{
    /*
        Check if the player's current tile has a negative value, ie in the fov cone
        or within a detection radius.
    */

    const auto& playerWorldPos = player->get_position();

    const auto playerGridPos = terrain->get_grid_position(playerWorldPos);

    // verify a valid position was returned
    if (terrain->is_valid_grid_position(playerGridPos) == true)
    {
        if (layer.get_value(playerGridPos) < 0.0f)
        {
            return true;
        }
    }

    // player isn't in the detection radius or fov cone, OR somehow off the map
    return false;
}

bool enemy_seek_player(MapLayer<float>& layer, AStarAgent* enemy)
{
    int height = terrain->get_map_height();
    int width = terrain->get_map_width();

    GridPos enemyGrid = terrain->get_grid_position(enemy->get_position());

    float maxValue = 0.0f;
    std::vector<GridPos> candidates;

    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            float value = layer.get_value(row, col);
            if (value == 1.0f)
            {
                candidates.push_back({ row, col });
            }
        }
    }

    if (candidates.empty())
        return false;

    GridPos closest = candidates[0];
    float minDist = FLT_MAX;

    for (const GridPos& gp : candidates)
    {
        float dx = static_cast<float>(gp.col - enemyGrid.col);
        float dy = static_cast<float>(gp.row - enemyGrid.row);
        float dist = dx * dx + dy * dy;

        if (dist < minDist)
        {
            minDist = dist;
            closest = gp;
        }
    }

    Vec3 targetPos = terrain->get_world_position(closest.row, closest.col);
    enemy->path_to(targetPos);

    return true;
}