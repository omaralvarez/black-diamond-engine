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

class Parser {
    
public:
    Parser();
    std::vector<Point> parse_data_file(std::string file_name);
    
};

#endif
