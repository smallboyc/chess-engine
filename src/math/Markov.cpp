#include "Markov.hpp"

/// MarkovInstability class implementation
void Markov::PawnInstability::update_state()
{
    double random_value = m_rng.random_0_1();
    double sum          = 0.0;

    for (int i = 0; i < 3; i++)
    {
        sum += m_transition_matrix[static_cast<int>(m_state)][i];
        if (random_value < sum)
        {
            m_state = static_cast<InstabilityState>(i);
            break;
        }
    }
}
