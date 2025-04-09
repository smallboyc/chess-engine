#pragma once
#include <glm/glm.hpp>
#include <optional>
#include <string>
#include "Texture.hpp"

namespace glmax {

struct Material {
    Material()                = default;
    Material(const Material&) = delete;
    Material(Material&&)      = default;
    std::string m_name;
    glm::vec3   m_Ka; // Ambiante reflectance (ka)
    // glm::vec3   m_Kd;        // Diffuse reflectance (kd)
    glm::vec3 m_Ks;        // Specular reflectance (ks)
    glm::vec3 m_Tr;        // Transmission color (tr)
    glm::vec3 m_Le;        // Emissive color (le)
    float     m_Ns;        // Specular exponent (ns) (shininess)
    float     m_Ni;        // Refraction index (ni)
    float     m_d;         // Dissolve (d) (transparence)
    float     m_illum;     // Illumination model (illum)
    float     m_sharpness; // Sharpness of highlight (unused in all cases)
    //
    std::optional<Texture> m_mapKa; // ambient
    std::optional<Texture> m_mapKd; // diffuse
    std::optional<Texture> m_mapKs; // specular
    std::optional<Texture> m_mapNs;
    std::optional<Texture> m_mapD;
    std::optional<Texture> m_mapBump;
    // 1 slot pour chaque textures
};

} // namespace glmax