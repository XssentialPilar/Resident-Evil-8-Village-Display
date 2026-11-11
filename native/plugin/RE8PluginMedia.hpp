#pragma once
#include "RE8PluginContext.hpp"
#include "RE8PluginInvoke.hpp"
#include "../common/RE8MethodLookup.hpp"
#include "../common/RE8GuiLayout.hpp"
#include "../common/RE8MediaLayout.hpp"
#include <cstring>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace re8_plugin {
// Only verified transition layers and GUI owners carrying a real Movie
// component are eligible. No camera, render target, decoder, input or cursor
// properties are changed. Other screen views keep the 2.7.0 policy.
class MediaLayers {
    using API=reframework::API;
    using Object=API::ManagedObject;
    using Method=API::Method;
    using Size=re8_patch::media::Size;
    using Gui=re8_patch::display::GuiResolutionState;
    struct Texture { Object* object{}; re8_patch::media::Owned<Size> size; };
    struct Entry {
        Object* view{}; Object* movie{};
        uint64_t seen{}, checked{}, applied_epoch{}, collection_checked{};
        bool transition{}, collected{}, logged{}, gui_owned{}, failed{};
        re8_patch::display::GuiResolutionOverride transform;
        re8_patch::media::Owned<Size> screen;
        std::vector<Texture> textures;
    };
public:
    void tick() {
        ++epoch;
        for(auto it=entries.begin();it!=entries.end();) {
            if(epoch-it->second.seen>120) {
                if(restore(it->second)) { release(it->second); it=entries.erase(it); }
                else { it->second.seen=epoch; ++it; } // Retain ownership until restoration can be verified.
            }
            else ++it;
        }
    }
    bool classify(Object* owner,Object* view,bool transition) {
        if(!owner||!view||!initialize())return false;
        if(transition?!transition_ready:!movie_ready)return false;
        auto it=entries.find(view);
        if(it==entries.end()) {
            if(entries.size()>=256)return false;
            Entry entry; entry.view=view; view->add_ref(); entry.transition=transition;
            it=entries.emplace(view,std::move(entry)).first;
        }
        auto& e=it->second; e.seen=epoch;
        if(!e.transition&&!e.movie&&(e.checked==0||epoch-e.checked>=120)) {
            e.checked=epoch;
            auto result=invoke(component,owner,{movie_type});
            if(result&&result->ptr) {e.movie=static_cast<Object*>(result->ptr);e.movie->add_ref();}
        }
        return e.transition||e.movie;
    }
    void apply(Object* view,bool contain) {
        auto it=entries.find(view); if(it==entries.end())return;
        auto& e=it->second;
        if(e.applied_epoch==epoch)return;
        e.applied_epoch=epoch;
        const auto output=Context::get().output_dimensions(); if(!output){restore(e);return;}
        auto source=size(screen_get,view); if(!source){restore(e);return;}
        if(e.movie) {
            auto w=invoke(width_get,e.movie),h=invoke(height_get,e.movie);
            // Do not assume a 16:9 source while a decoder is opening/seeking.
            if(!w||!h||w->dword==0||h->dword==0||w->dword>16384||h->dword>16384) {restore(e);return;}
            const Size decoded{float(w->dword),float(h->dword)};
            // A first draw can precede texture creation. Retry failed discovery
            // at a bounded cadence and periodically notice replaced children.
            const auto interval=e.collected?120u:1u;
            if(e.collection_checked==0||epoch-e.collection_checked>=interval) {
                e.collection_checked=epoch;
                e.collected=collect(e);
            }
            // A movie GUI with an ambiguous texture hierarchy stays native.
            if(!e.collected||e.textures.size()!=1) {restore(e);return;}
            auto& texture=e.textures.front();
            auto authored=size(texture_size_get,texture.object);
            if(!authored){restore(e);return;}
            // Compare against the native image dimensions, never our previous
            // resize, so codec padding cannot accumulate across frames.
            if(texture.size.active) {
                if(authored->width==texture.size.applied.width)authored->width=texture.size.original.width;
                if(authored->height==texture.size.applied.height)authored->height=texture.size.original.height;
            }
            source=re8_patch::media::source_size(decoded,*authored);
        }
        auto placement=re8_patch::media::fit(*source,{(*output)[0],(*output)[1]},
            contain&&!e.transition?re8_patch::media::Fit::Contain:re8_patch::media::Fit::Cover);
        if(!placement){restore(e);return;}
        // Read every transform property before changing any canvas geometry.
        auto observed=read_gui(view); if(!observed){restore(e);return;}
        const auto wanted=e.transform.apply(*observed,contain&&!e.transition?fit_small:fit_large,center,always);
        bool success=true;
        if(e.movie) {
            success=write_size(screen_get,screen_set,view,e.screen,*source);
            for(auto& t:e.textures) {
                if(!success)break;
                success=write_size(texture_size_get,texture_size_set,t.object,t.size,*source);
            }
        }
        if(success) {
            e.gui_owned=e.gui_owned||!same(wanted,*observed);
            success=write_gui(view,wanted,*observed);
        }
        if(!success) {
            const bool recovered=restore(e);
            if(!e.failed)Context::get().api().log_warn("[RE8Display.Media] geometry update failed; rollback=%s",recovered?"verified":"pending retry");
            e.failed=true;
            return;
        }
        e.failed=false;
        if(!e.logged) {
            Context::get().api().log_info("[RE8Display.Media] %s source=%.0fx%.0f output=%.0fx%.0f mode=%s; uniform geometry only",
                e.transition?"transition":"decoded movie",source->width,source->height,(*output)[0],(*output)[1],
                contain&&!e.transition?"contain":"cover");
            e.logged=true;
        }
    }
    void restore(Object* view) {auto it=entries.find(view);if(it!=entries.end())restore(it->second);}
private:
    uint64_t epoch{1},binding_checked{}; bool transition_ready{},movie_ready{},attempted{};
    Object* movie_type{};
    std::unordered_map<Object*,Entry> entries;
    Method *component{},*width_get{},*height_get{},*screen_get{},*screen_set{},*child_get{},*next_get{},
        *texture_size_get{},*texture_size_set{},*scale_get{},*scale_set{},*anchor_get{},*anchor_set{},
        *condition_get{},*condition_set{},*adjust_get{},*adjust_set{};
    int32_t fit_small{},fit_large{},center{},always{};
    static void* integer(int32_t n){return reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(n)));}
    static std::optional<reframework::InvokeRet> invoke(Method* method,Object* object,std::initializer_list<void*> args={}) {
        if(!object)return std::nullopt;
        return invoke_small(method,object,args);
    }
    static std::optional<int32_t> enumeration(const char* type,const char* name) {
        auto t=Context::get().api().tdb()->find_type(type); if(!t)return std::nullopt;
        auto f=t->find_field(name);if(!f)return std::nullopt;
        auto p=f->get_init_data();if(!p)p=f->get_data_raw(nullptr,true);if(!p)return std::nullopt;
        int32_t value{};std::memcpy(&value,p,sizeof(value));return value;
    }
    bool initialize() {
        if(transition_ready&&movie_ready)return true;
        if(attempted&&epoch-binding_checked<120)return transition_ready||movie_ready;
        auto& api=Context::get().api();auto tdb=api.tdb();if(!tdb)return false;
        const bool first=!attempted,previous_transition=transition_ready,previous_movie=movie_ready;
        attempted=true;
        binding_checked=epoch;
        auto method=[&](const char* t,const char* n){return re8_patch::managed::inherited_method(tdb->find_type(t),n);};
        component=method("via.GameObject","getComponent(System.Type)");
        width_get=method("via.movie.Movie","get_VideoWidth");height_get=method("via.movie.Movie","get_VideoHeight");
        screen_get=method("via.gui.View","get_ScreenSize");screen_set=method("via.gui.View","set_ScreenSize");
        child_get=method("via.gui.TransformObject","get_Child");next_get=method("via.gui.PlayObject","get_Next");
        texture_size_get=method("via.gui.Texture","get_Size");texture_size_set=method("via.gui.Texture","set_Size");
        scale_get=method("via.gui.TransformObject","get_ResAdjustScale");scale_set=method("via.gui.TransformObject","set_ResAdjustScale");
        anchor_get=method("via.gui.TransformObject","get_ResAdjustAnchor");anchor_set=method("via.gui.TransformObject","set_ResAdjustAnchor");
        condition_get=method("via.gui.TransformObject","get_ResAdjustCondition");condition_set=method("via.gui.TransformObject","set_ResAdjustCondition");
        adjust_get=method("via.gui.TransformObject","get_ResolutionAdjust");adjust_set=method("via.gui.TransformObject","set_ResolutionAdjust");
        auto small_mode=enumeration("via.gui.ResolutionAdjustScale","FitSmallRatioAxis"),large_mode=enumeration("via.gui.ResolutionAdjustScale","FitLargeRatioAxis"),
            c=enumeration("via.gui.ResolutionAdjustAnchor","CenterCenter"),a=enumeration("via.gui.ResolutionAdjustCondition","Always");
        if(!movie_type) {movie_type=api.typeof("via.movie.Movie");if(movie_type)movie_type->add_ref();}
        transition_ready=small_mode&&large_mode&&c&&a&&screen_get&&
            scale_get&&scale_set&&anchor_get&&anchor_set&&condition_get&&condition_set&&adjust_get&&adjust_set;
        movie_ready=transition_ready&&movie_type&&component&&width_get&&height_get&&screen_set&&child_get&&next_get&&
            texture_size_get&&texture_size_set;
        if(transition_ready){fit_small=*small_mode;fit_large=*large_mode;center=*c;always=*a;}
        if(first||previous_transition!=transition_ready||previous_movie!=movie_ready)
            api.log_info("[RE8Display.Media] revision=media-r2 transition_bindings=%s movie_bindings=%s; transition policy always on, no loading-indicator suppression",
                transition_ready?"ready":"unavailable",movie_ready?"ready":"unavailable");
        return transition_ready||movie_ready;
    }
    static std::optional<Size> size(Method* getter,Object* object) {
        auto r=invoke(getter,object);if(!r)return std::nullopt;
        Size s{};std::memcpy(&s,r->bytes.data(),sizeof(s));return re8_patch::media::valid(s)?std::optional{s}:std::nullopt;
    }
    static bool write_size(Method* getter,Method* setter,Object* object,re8_patch::media::Owned<Size>& owned,Size wanted) {
        auto observed=size(getter,object);if(!observed||!setter)return false;
        // Width and height have independent ownership if the engine changes one.
        if(!owned.active)owned.original=*observed;
        else {
            if(observed->width!=owned.applied.width)owned.original.width=observed->width;
            if(observed->height!=owned.applied.height)owned.original.height=observed->height;
        }
        if(*observed==wanted)return true;
        owned.active=true;owned.applied=wanted;
        auto result=invoke(setter,object,{&wanted});
        auto after=size(getter,object);
        return result&&after&&*after==wanted;
    }
    static bool restore_size(Method* getter,Method* setter,Object* object,re8_patch::media::Owned<Size>& owned) {
        if(!owned.active)return true;
        auto observed=size(getter,object);if(!observed||!setter)return false;
        auto wanted=*observed;
        if(observed->width==owned.applied.width)wanted.width=owned.original.width;
        if(observed->height==owned.applied.height)wanted.height=owned.original.height;
        if(wanted!=*observed) {
            invoke(setter,object,{&wanted});
            auto after=size(getter,object);if(!after||*after!=wanted)return false;
        }
        owned.active=false;
        return true;
    }
    std::optional<Gui> read_gui(Object* view) {
        auto s=invoke(scale_get,view),a=invoke(anchor_get,view),c=invoke(condition_get,view),r=invoke(adjust_get,view);
        if(!s||!a||!c||!r)return std::nullopt;
        return Gui{int32_t(s->dword),int32_t(a->dword),int32_t(c->dword),r->byte!=0};
    }
    static bool same(Gui a,Gui b) {
        return a.scale==b.scale&&a.anchor==b.anchor&&a.condition==b.condition&&a.enabled==b.enabled;
    }
    bool write_gui(Object* view,Gui wanted,Gui observed) {
        if(same(wanted,observed))return true;
        if(wanted.scale!=observed.scale&&!invoke(scale_set,view,{integer(wanted.scale)}))return false;
        if(wanted.anchor!=observed.anchor&&!invoke(anchor_set,view,{integer(wanted.anchor)}))return false;
        if(wanted.condition!=observed.condition&&!invoke(condition_set,view,{integer(wanted.condition)}))return false;
        if(wanted.enabled!=observed.enabled&&!invoke(adjust_set,view,{integer(wanted.enabled?1:0)}))return false;
        auto after=read_gui(view);
        return after&&same(*after,wanted);
    }
    bool collect(Entry& e) {
        auto r=invoke(child_get,e.view);if(!r)return false;
        auto node=static_cast<Object*>(r->ptr);
        std::unordered_set<Object*> visited;
        std::vector<Object*> found;
        // Top-level textures only: never resize subtitles, indicators or UI.
        while(node&&visited.size()<64&&visited.insert(node).second) {
            auto type=node->get_type_definition();
            if(type&&(type->get_full_name()=="via.gui.Texture"||type->is_derived_from("via.gui.Texture"))) {
                if(size(texture_size_get,node))found.push_back(node);
            }
            r=invoke(next_get,node);if(!r)return false;
            node=static_cast<Object*>(r->ptr);
        }
        if(node||found.size()!=1)return false;
        if(e.textures.size()==1&&e.textures.front().object==found.front())return true;
        // Restore replaced children before releasing their retained references.
        bool restored=true;
        for(auto& t:e.textures)if(!restore_size(texture_size_get,texture_size_set,t.object,t.size))restored=false;
        if(!restored)return false;
        for(auto& t:e.textures)t.object->release();
        e.textures.clear();
        for(auto* object:found){object->add_ref();e.textures.push_back({object,{}});}
        return true;
    }
    bool restore(Entry& e) {
        bool restored=true;
        if(e.gui_owned) {
            auto observed=read_gui(e.view);
            if(observed&&write_gui(e.view,e.transform.restore(*observed),*observed))e.gui_owned=false;
            else restored=false;
        }
        if(!e.gui_owned)e.transform={};
        if(!restore_size(screen_get,screen_set,e.view,e.screen))restored=false;
        for(auto& t:e.textures)if(!restore_size(texture_size_get,texture_size_set,t.object,t.size))restored=false;
        e.logged=false;
        return restored;
    }
    static void release(Entry& e) {
        for(auto& t:e.textures)t.object->release();
        if(e.movie)e.movie->release();e.view->release();
    }
};
}
