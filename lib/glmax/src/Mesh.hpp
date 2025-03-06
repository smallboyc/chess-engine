#pragma once
#include <vector>
#include "Material.hpp"
#include "Vertex.hpp"

namespace glmax {

struct Submesh {
    unsigned int m_index_offset;
    unsigned int m_index_count;
    unsigned int m_material_id;
};

class Mesh {
private:
    std::vector<Vertex>       m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Material>     m_materials;
    std::vector<Submesh>      m_submeshes;

public:
    Mesh() = default;
    void                         loadMesh(const std::string& objPath, const std::string& mtlPath);
    std::vector<Vertex>          getVertices() const { return m_vertices; };
    std::vector<unsigned int>    getIndices() const { return m_indices; };
    const std::vector<Material>& getMaterials() const { return m_materials; };
    std::vector<Submesh>         getSubmeshes() const { return m_submeshes; };
};
} // namespace glmax