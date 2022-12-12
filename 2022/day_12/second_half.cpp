#include <iostream>
#include <string>
#include <cassert>
#include <cstdint>
#include <vector>
#include <fstream>
#include <sstream>
#include <list>
#include <algorithm>

typedef std::vector<std::string> Map;

struct Vertex {
    bool checked = false;
    uint32_t value = ~0;
    std::vector<Vertex*> edges = {};
    bool to_be_processed = false;
    char elevation = 0;
};

bool load_height_map(const std::string& filename, Map& out_map)
{
    std::ifstream file_map(filename);
    if (!file_map.is_open())
        return false;
    std::stringstream sstr;
    sstr << file_map.rdbuf();

    uint w = 0;
    std::string line;

    while (std::getline(sstr, line))
    {
        if (w == 0)
            w = line.length();
        else if (line.length() != w)
            return false;
        out_map.push_back(line);
    }

    file_map.close();
    return true;
}

bool check_neigh(const Map& map, char c, uint y, uint x)
{
    uint h = map.size(), w = map[0].length();
    if (c == 'S')
        c = 'a';
    else if (c == 'E')
        c = 'z';

    if (x < w && y < h) {
        if (map[y][x] == 'S')
            return (int)'a' - (int)c <= 1;
        if (map[y][x] == 'E')
            return (int)'z' - (int)c <= 1;
        return (int)c - (int)map[y][x] <= 1;
    }
    return false;
}


// Use Dijkstra's algorithm to find minimum path cost from E to all others.
// Then we choose the one with lowest value and elevation level 'a'.
uint32_t get_min_value(const Map& map)
{
    uint32_t map_width = map[0].length();
    uint32_t map_size = map.size() * map_width;
    Vertex* graph = new Vertex[map_size];
    const auto& in_graph = [&graph, map_width](uint y, uint x){ return graph + (y * map_width + x); };
    Vertex* p_start = nullptr;

    // Fill graph with edges.
    for (uint y = 0; y < map.size(); y++)
    {
        for (uint x = 0; x < map[0].length(); x++)
        {
            if (map[y][x] == 'E')
                p_start = in_graph(y, x);

            if (check_neigh(map, map[y][x], y - 1, x))
                in_graph(y, x)->edges.push_back(in_graph(y - 1, x));
            if (check_neigh(map, map[y][x], y, x - 1))
                in_graph(y, x)->edges.push_back(in_graph(y, x - 1));
            if (check_neigh(map, map[y][x], y + 1, x))
                in_graph(y, x)->edges.push_back(in_graph(y + 1, x));
            if (check_neigh(map, map[y][x], y, x + 1))
                in_graph(y, x)->edges.push_back(in_graph(y, x + 1));

            in_graph(y, x)->elevation = map[y][x];
        }
    }

    p_start->value = 0;
    std::list<Vertex*> to_process = { p_start };
    while (!to_process.empty())
    {
        // Get vertex with lowest value.
        std::list<Vertex*>::iterator lowest = std::min_element(to_process.begin(), to_process.end(), [](const auto& a, const auto& b) { return a->value < b->value; });
        Vertex* current = *lowest;
        to_process.erase(lowest);

        // Iterate over all edges.
        for (auto i = current->edges.begin(); i != current->edges.end(); i++)
        {
            Vertex* v = *i;
            if (v->checked)
                continue;

            if (current->value + 1 < v->value)
                v->value = current->value + 1;
            if (!v->to_be_processed) {
                to_process.push_back(v);
                v->to_be_processed = true;
            }
        }

        current->checked = true;
    }

    uint32_t min_value = ~0;
    for (uint32_t i = 0; i < map_size; i++)
        if (graph[i].elevation == 'a')
            min_value = std::min(min_value, graph[i].value);

    delete[] graph;
    return min_value;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 1;

    std::string filename(argv[1]);
    Map map;

    if (!load_height_map(filename, map)) {
        std::cerr << "error: Failed to load map from file." << std::endl;
        return 1;
    }

    uint32_t min_value = get_min_value(map);
    std::cout << "Minimal value: " << min_value << std::endl;

    return 0;
}
