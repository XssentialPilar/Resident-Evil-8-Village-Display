#pragma once
#include <array>
#include <string_view>

namespace re8_patch::managed {
// The host's find_method searches declarations, not inherited members.
// Bound metadata traversal and reject cycles or non-invocable declarations.
template<class Type>
auto inherited_method(Type* type, std::string_view name) -> decltype(type->find_method(name)) {
    std::array<Type*, 16> visited{};
    for (unsigned depth=0; type && depth<visited.size(); ++depth) {
        for (unsigned i=0; i<depth; ++i) if (visited[i]==type) return nullptr;
        visited[depth]=type;
        if (auto method=type->find_method(name))
            return method->get_function_raw() ? method : nullptr;
        type=type->get_parent_type();
    }
    return nullptr;
}
}
