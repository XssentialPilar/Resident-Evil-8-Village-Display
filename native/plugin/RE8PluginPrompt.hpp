#pragma once
#include "RE8PluginInvoke.hpp"
#include "../common/RE8MethodLookup.hpp"
#include "../common/RE8PromptLayout.hpp"
#include <cstring>
#include <unordered_map>

namespace re8_plugin {
// A verified RE8 interaction panel compensates Y for the old stretched canvas.
// Remove only that recognizable compensation, during rendering. The engine
// retains its world position, interaction, animation and all other UI geometry.
class PromptLayers {
    using API=reframework::API;
    using Object=API::ManagedObject;
    using Method=API::Method;
    struct Vec3 { float x{},y{},z{}; };
    struct Size { float x{},y{}; };
    struct Entry {
        Object* panel{};
        uint64_t seen{},searched{};
        float original_y{},applied_y{};
        bool active{},logged{};
    };
public:
    void tick() {
        ++epoch;
        for(auto it=entries.begin();it!=entries.end();) {
            auto& e=it->second;
            const bool restored=restore(e);
            if(restored&&epoch-e.seen>120) {
                if(e.panel)e.panel->release();
                it->first->release();
                it=entries.erase(it);
            } else ++it;
        }
    }
    void restore_all() {for(auto& [view,e]:entries)restore(e);}
    void apply(Object* owner,Object* view) {
        if(!owner||!view||!initialize())return;
        auto it=entries.find(view);
        if(it==entries.end()) {
            if(entries.size()>=256)return;
            view->add_ref();it=entries.emplace(view,Entry{}).first;
            if(!named(owner,owner_name_get,prompt_name))it->second.searched=UINT64_MAX;
        }
        auto& e=it->second;e.seen=epoch;
        if(e.searched==UINT64_MAX||e.active)return;
        if(!e.panel) {
            if(e.searched&&epoch-e.searched<1)return;
            e.searched=epoch;
            auto first=invoke_small(child_get,view);if(!first)return;
            auto node=static_cast<Object*>(first->ptr);
            std::array<Object*,64> visited{};
            for(size_t count=0;node&&count<visited.size();++count) {
                if(std::find(visited.begin(),visited.begin()+count,node)!=visited.begin()+count)return;
                visited[count]=node;
                if(named(node,node_name_get,main_name)) {
                    auto type=node->get_type_definition();
                    if(!type||!(type->get_full_name()=="via.gui.Panel"||type->is_derived_from("via.gui.Panel")))return;
                    e.panel=node;node->add_ref();break;
                }
                auto next=invoke_small(next_get,node);if(!next)return;
                node=static_cast<Object*>(next->ptr);
            }
        }
        if(!e.panel)return;
        const auto output=Context::get().output_dimensions();if(!output)return;
        auto canvas=invoke_small(screen_get,view);if(!canvas)return;
        Size size{};std::memcpy(&size,canvas->bytes.data(),sizeof(size));
        if(!std::isfinite(size.x)||!std::isfinite(size.y)||size.x<=0||size.y<=0)return;
        auto scale=read(e.panel);if(!scale)return;
        const auto wanted=re8_patch::display::proportional_prompt_y(scale->x,scale->y,size.x/size.y,(*output)[0]/(*output)[1]);
        if(!wanted)return;
        e.original_y=scale->y;e.applied_y=*wanted;e.active=true;
        scale->y=*wanted;
        const auto changed=invoke_small(scale_set,e.panel,{&*scale});
        const auto after=read(e.panel);
        if(!changed||!after||after->y!=*wanted) {restore(e);return;}
        if(!e.logged) {
            Context::get().api().log_info("[RE8Display.Prompt] GUIInteractIcon/main proportional scale active; native Y=%.6f render Y=%.6f; position retained",e.original_y,e.applied_y);
            e.logged=true;
        }
    }
private:
    uint64_t epoch{1},checked{};
    bool ready{};
    Object *prompt_name{},*main_name{};
    Method *owner_name_get{},*node_name_get{},*equals{},*child_get{},*next_get{},*screen_get{},*scale_get{},*scale_set{};
    std::unordered_map<Object*,Entry> entries;
    bool initialize() {
        if(ready)return true;
        if(checked&&epoch-checked<120)return false;
        checked=epoch;
        auto& api=Context::get().api();auto tdb=api.tdb();if(!tdb)return false;
        auto method=[&](const char* t,const char* n){return re8_patch::managed::inherited_method(tdb->find_type(t),n);};
        owner_name_get=method("via.GameObject","get_Name");node_name_get=method("via.gui.PlayObject","get_Name");
        equals=method("System.String","Equals(System.String)");
        child_get=method("via.gui.TransformObject","get_Child");next_get=method("via.gui.PlayObject","get_Next");
        screen_get=method("via.gui.View","get_ScreenSize");
        scale_get=method("via.gui.TransformObject","get_Scale");scale_set=method("via.gui.TransformObject","set_Scale");
        for(auto required:{owner_name_get,node_name_get,equals,child_get,next_get,screen_get,scale_get,scale_set})if(!required)return false;
        if(!prompt_name){prompt_name=api.create_managed_string_normal("GUIInteractIcon");if(prompt_name)prompt_name->add_ref();}
        if(!main_name){main_name=api.create_managed_string_normal("main");if(main_name)main_name->add_ref();}
        ready=prompt_name&&main_name;
        return ready;
    }
    bool named(Object* object,Method* getter,Object* expected) {
        auto name=invoke_small(getter,object);if(!name||!name->ptr)return false;
        auto equal=invoke_small(equals,static_cast<Object*>(name->ptr),{expected});
        return equal&&equal->byte;
    }
    std::optional<Vec3> read(Object* object) {
        auto r=invoke_small(scale_get,object);if(!r)return std::nullopt;
        Vec3 scale{};std::memcpy(&scale,r->bytes.data(),sizeof(scale));
        if(!std::isfinite(scale.x)||!std::isfinite(scale.y)||!std::isfinite(scale.z))return std::nullopt;
        return scale;
    }
    bool restore(Entry& e) {
        if(!e.active)return true;
        auto observed=read(e.panel);if(!observed)return false;
        // Never undo an animation or engine change made after our write.
        if(observed->y==e.applied_y) {
            observed->y=e.original_y;
            if(!invoke_small(scale_set,e.panel,{&*observed}))return false;
            auto after=read(e.panel);if(!after||after->y!=e.original_y)return false;
        }
        e.active=false;return true;
    }
};
}
