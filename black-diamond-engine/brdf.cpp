//
//  brdf.cpp
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 5/13/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#include "brdf.h"

//BRDF Factory.
BRDF* BRDF::create(std::string type) {
    if ( type == "BRDF_SIMPLE" ) return new BRDFSimple();
    //if ( type == "BRDF_OTHER_TYPE" ) return new BRDFOtherType();
    return NULL;
}