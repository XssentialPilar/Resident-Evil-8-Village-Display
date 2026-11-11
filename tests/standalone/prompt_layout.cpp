#include "RE8PromptLayout.hpp"
#include <initializer_list>
#include <cassert>
#include <limits>
int main(){
    using re8_patch::display::proportional_prompt_y;
    constexpr float canvas=1920.f/1080.f;
    for(float aspect:{4.f/3.f,16.f/10.f,21.f/9.f,32.f/9.f,9.f/16.f})
        for(float animation:{0.5f,1.f,1.3f}) {
            auto y=proportional_prompt_y(animation,animation*aspect/canvas,canvas,aspect);
            assert(y && *y==animation);
        }
    assert(!proportional_prompt_y(1,1,canvas,canvas));
    assert(!proportional_prompt_y(1,0.8f,canvas,1.6f));
    assert(!proportional_prompt_y(1,0,canvas,1.6f));
    assert(!proportional_prompt_y(0,1,canvas,1.6f));
    assert(!proportional_prompt_y(1,1,0,1.6f));
    assert(!proportional_prompt_y(1,std::numeric_limits<float>::quiet_NaN(),canvas,1.6f));
}
