#include "RE8MediaLayout.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>

int main() {
    using namespace re8_patch::media;
    assert((source_size({1920,1088},{1920,1080})==Size{1920,1080}));
    assert((source_size({1920,1080},{1920,1080})==Size{1920,1080}));
    assert((source_size({1920,1080},{1920,1200})==Size{1920,1080}));
    assert((source_size({1920,1088},{1280,720})==Size{1920,1088}));
    assert((source_size({1920,1088},{1920,1071})==Size{1920,1088}));
    assert((source_size({800,800},{784,800})==Size{800,800})); // Strictly below 2%.
    assert((source_size({1920,1088},{0,1080})==Size{1920,1088}));
    assert(!source_size({0,1088},{1920,1080}));
    assert(!source_size({std::numeric_limits<float>::infinity(),1088},{1920,1080}));
    assert((source_size({1920,1088},{std::numeric_limits<float>::quiet_NaN(),1080})==Size{1920,1088}));
    const auto covered=fit({1920,1080},{1920,1200},Fit::Cover);
    assert(covered && covered->width>=1920 && covered->height>=1200);
    const auto contained=fit({1920,1080},{1920,1200},Fit::Contain);
    assert(contained && contained->width<=1920 && contained->height<=1200);
    assert(contained->scale==1 && contained->y==60 && contained->x==0);
    const Size sources[]={{1920,1080},{3840,2160},{1440,1080},{1920,800}};
    const Size outputs[]={{1920,1080},{1920,1200},{1440,1080},{3440,1440},{5120,1440},{1080,1920},{7680,4320}};
    for(auto s:sources)for(auto o:outputs)for(auto mode:{Fit::Cover,Fit::Contain}) {
        auto p=fit(s,o,mode); assert(p);
        assert(std::abs(p->width/s.width-p->height/s.height)<0.00001f);
        assert(std::abs(p->x*2+p->width-o.width)<0.01f);
        assert(std::abs(p->y*2+p->height-o.height)<0.01f);
        if(mode==Fit::Cover)assert(p->width+0.01f>=o.width&&p->height+0.01f>=o.height);
        else assert(p->width<=o.width+0.01f&&p->height<=o.height+0.01f);
    }
    assert(!fit({0,1080},{1920,1200},Fit::Cover));
    assert(!fit({1920,1080},{std::numeric_limits<float>::infinity(),1200},Fit::Cover));
    Owned<Size> owned; owned.observe({1920,1080}); owned.active=true;owned.applied={1920,1088};
    assert(owned.restored({1920,1088})==(Size{1920,1080}));
    assert(owned.restored({1280,720})==(Size{1280,720}));
    owned.observe({1280,720}); assert(owned.original==(Size{1280,720}));
    std::cout<<"PASS: 56 proportional placements, decoder padding and ownership restoration\n";
}
