#include "RE8MethodLookup.hpp"
#include <cassert>
#include <string_view>
struct Method { bool native=true; void* get_function_raw(){return native?this:nullptr;} };
struct Type {
    Type* parent{}; std::string_view name; Method* method{};
    Method* find_method(std::string_view n){return n==name?method:nullptr;}
    Type* get_parent_type(){return parent;}
};
int main(){
    using re8_patch::managed::inherited_method;
    Method size,child,unavailable{false};
    Type rect{nullptr,"get_Size",&size}, texture{&rect,"",nullptr};
    Type play{},transform{&play,"get_Child",&child};
    assert(inherited_method(&texture,"get_Size")==&size);
    assert(inherited_method(&transform,"get_Child")==&child);
    assert(!inherited_method(&play,"get_Child"));
    assert(!inherited_method(&texture,"set_Unknown"));
    Type bad{&rect,"get_Size",&unavailable};
    assert(!inherited_method(&bad,"get_Size"));
    play.parent=&transform;
    assert(!inherited_method(&play,"missing"));
    assert(!inherited_method<Type>(nullptr,"get_Size"));
    std::array<Type,18> deep{};
    for(unsigned i=0;i+1<deep.size();++i)deep[i].parent=&deep[i+1];
    deep.back().name="get_Size";deep.back().method=&size;
    assert(!inherited_method(&deep[0],"get_Size"));
}
