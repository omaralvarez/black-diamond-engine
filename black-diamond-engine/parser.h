/*
 *	parser.h
 *	black-diamond-engine
 *
 *	Created by Luis Omar Alvarez Mures on 2/13/12.
 *	Copyright (c) 2012
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
    std::vector<Surfel> parse_data_file_v002(std::string file_name, float radius);
    std::vector<PointLight> parse_lights_file(std::string file_name);
    
};

#endif
