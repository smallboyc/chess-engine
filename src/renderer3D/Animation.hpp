#pragma once

struct Animation {
    bool  is_animating{false};
    float animation_start_time{0.0f};
    float animation_duration{1.0f};
    float elevation{0.0f};
};
