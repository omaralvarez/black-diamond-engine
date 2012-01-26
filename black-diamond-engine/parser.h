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
//#include "scene.h"
#include "render.h"
#include "tinyxml.h"

class Parser {
    
public:
    Parser();
    const char * getIndent( unsigned int numIndents );
    const char * getIndentAlt( unsigned int numIndents );
    int dump_attribs_to_stdout(TiXmlElement* pElement, unsigned int indent);
    void dump_to_stdout( TiXmlNode* pParent, unsigned int indent = 0 );
    void dump_to_stdout(const char* pFilename);
    Render parse_config(char* config_path);
    std::vector<Point> parse_data_file(std::string file_name);
    
};

#endif
