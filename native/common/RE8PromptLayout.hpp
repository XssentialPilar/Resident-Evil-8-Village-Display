#pragma once
#include <cmath>
#include <optional>

namespace re8_patch::display {
// RE8's interaction panel counter-scales Y for a stretched 16:9 canvas.
// Once that canvas is uniform, remove ONLY that recognized compensation.
// Arbitrary animation scales and invalid/ambiguous geometry stay untouched.
inline std::optional<float> proportional_prompt_y(float x,float y,float canvas_aspect,float output_aspect) {
    if(!std::isfinite(x)||!std::isfinite(y)||!std::isfinite(canvas_aspect)||!std::isfinite(output_aspect)||
        x<=0 || y<=0 || canvas_aspect<=0 || output_aspect<=0)return std::nullopt;
    const float correction=output_aspect/canvas_aspect;
    if(correction<0.2f || correction>5.0f || std::abs(correction-1.0f)<0.0001f)return std::nullopt;
    if(std::abs(y/x-correction)>0.001f)return std::nullopt;
    return x;
}
}
