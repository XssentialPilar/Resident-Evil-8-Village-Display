#pragma once
#include "RE8PluginContext.hpp"
#include <initializer_list>
#include <algorithm>
#include <array>

namespace re8_plugin {
// Small synchronous managed calls need no heap allocation. Never truncate args.
inline std::optional<reframework::InvokeRet> invoke_small(reframework::API::Method* method,
    reframework::API::ManagedObject* object, std::initializer_list<void*> arguments={}) {
    if (!method || !method->get_function_raw() || arguments.size()>8) return std::nullopt;
    std::array<void*,8> args{};
    std::copy(arguments.begin(),arguments.end(),args.begin());
    reframework::InvokeRet result{};
    const auto status=Context::get().api().sdk()->method->invoke(*method,object,
        arguments.size()?args.data():nullptr,static_cast<unsigned int>(arguments.size()*sizeof(void*)),
        &result,sizeof(result));
    if(status!=REFRAMEWORK_ERROR_NONE || result.exception_thrown)return std::nullopt;
    return result;
}
}
