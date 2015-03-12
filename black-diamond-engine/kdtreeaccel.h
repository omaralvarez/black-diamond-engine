/*
 *	kdtreeaccel.h
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

#ifndef black_diamond_engine_kdtreeaccel_h
#define black_diamond_engine_kdtreeaccel_h

#include "aggregate.h"
#include "memoryarena.h"
#include <vector>

struct KdAccelNode;
struct BoundEdge;
struct KdToDo;

class KdTreeAccel : public Aggregate {
    
    int isect_cost, traversal_cost, max_surfels; 
    float empty_bonus;  
    KdAccelNode *nodes;
    int n_alloced_nodes, next_free_node;
    BBox bounds;
    MemoryArena arena;
    mutable int cur_surf_id;
    
public:
    std::vector<Surfel> surfels;
    //KdTreeAccel();
    //Def call: int icost = 80, int scost = 1,  float ebonus = 0.5f, int maxp = 1,int maxDepth = -1
    KdTreeAccel(std::vector<Surfel> &p, int icost, int tcost, float ebonus, int maxs, int max_depth);
    ~KdTreeAccel();
    void build_tree(int node_num, BBox node_bounds, std::vector<BBox> all_surfel_bounds, int *surfel_nums, int n_surfels, int depth, BoundEdge *edges[3], int *surfels0, int *surfels1,int bad_refines);
    void intersect(Ray *ray);
    bool intersect_p(Ray *ray);
    Surfel **get_neighbours(bdm::Point p, float dist, u_int32_t *n_neighbours);
    
};

#endif
