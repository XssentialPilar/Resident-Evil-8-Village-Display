#pragma once
#include <algorithm>
#include <cmath>
#include <optional>

namespace re8_patch::media {
enum class Fit { Cover, Contain };
struct Size { float width{}, height{}; bool operator==(const Size&) const = default; };
struct Placement { float scale{}, x{}, y{}, width{}, height{}; };
inline bool valid(Size size) {
    return std::isfinite(size.width) && std::isfinite(size.height) &&
        size.width >= 1 && size.height >= 1 && size.width <= 65536 && size.height <= 65536;
}
// Decoder allocation dimensions can include a small macroblock border, e.g.
// 1920x1088 for a 1920x1080 image. Preserve the authored visible image only
// within that tightly bounded padding; unrelated canvas ratios are rejected.
inline std::optional<Size> source_size(Size decoded, Size authored) {
    if (!valid(decoded)) return std::nullopt;
    if (!valid(authored)) return decoded;
    const auto padding=[](float storage,float image) {
        const float extra=storage-image;
        return extra>=0 && extra<=16 && extra/storage<0.02f;
    };
    if (padding(decoded.width,authored.width) && padding(decoded.height,authored.height)) return authored;
    return decoded;
}
inline std::optional<Placement> fit(Size source, Size output, Fit mode) {
    if (!valid(source) || !valid(output)) return std::nullopt;
    const float x = output.width / source.width, y = output.height / source.height;
    const float scale = mode == Fit::Cover ? std::max(x,y) : std::min(x,y);
    const float width = source.width * scale, height = source.height * scale;
    return Placement{scale, (output.width-width)*0.5f, (output.height-height)*0.5f, width, height};
}
// Each property is restored only while it still contains our last write.
// Engine updates therefore retain ownership of animation and playback state.
template<class T> struct Owned {
    T original{}, applied{}; bool active{};
    void observe(T value) { if (!active || value != applied) original=value; }
    T restored(T value) const { return active && value==applied ? original : value; }
};
}
