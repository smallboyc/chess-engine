#pragma once
#include "RNG.hpp"

namespace Markov {

enum class InstabilityState { NORMAL,
                              BOOST_SPEED,
                              TELEPORTATION };

class PawnInstability {
private:
    glm::mat3          m_transition_matrix;
    InstabilityState   m_state;
    Probabilities::RNG m_rng;

public:
    PawnInstability()
        : m_state(InstabilityState::NORMAL)
    {
        m_transition_matrix = glm::mat3(
            0.8f, 0.17f, 0.03f, // NORMAL -> (NORMAL, BOOST_SPEED,, TELEPORTATION)
            0.5f, 0.3f, 0.2f,   // BOOST_SPEED, -> (NORMAL, BOOST_SPEED,, TELEPORTATION)
            0.8f, 0.15f, 0.05f  // TELEPORTATION -> (NORMAL, BOOST_SPEED,, TELEPORTATION)
        );
    }

    InstabilityState get_state() const { return m_state; }
    void             update_state();
};

} // namespace Markov