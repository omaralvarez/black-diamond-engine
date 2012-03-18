//
//  parser.h
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 1/17/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#ifndef black_diamond_engine_parser_h
#define black_diamond_engine_parser_h

#include <string>
#include <vector>
#include "point.h"
#include "render.h"
#include "tinyxml.h"
#include "surfel.h"
#include "bdesettings.h"

extern BDESettings settings;

class Parser {
    
public:
    Parser();
    Render parse_config(char* config_path);
    std::vector<Surfel> parse_data_file(std::string file_name, float radius);
    std::vector<PointLight> parse_lights_file(std::string file_name);
    
};

#endif
