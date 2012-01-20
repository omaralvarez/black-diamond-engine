//
//  render.h
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 1/17/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#ifndef black_diamond_engine_render_h
#define black_diamond_engine_render_h

#include "scene.h"

class Render {
    
public:
    Render(){}
    Scene trans_world_to_cam(Scene s);
    Scene filter_frustum(Scene s);
    
};

#endif
