//
//  kdtreeaccel.cpp
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 2/13/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#include <iostream>
#include <vector>
#include "kdtreeaccel.h"
#include "surfel.h"
#include "memoryarena.h"//Possible optimization problem.

struct KdAccelNode {
    
    //Init leaf node.
    void init_leaf(int *surfel_nums,int ns, std::vector<Surfel> s, MemoryArena &arena) { 
        
        n_surfels = ns << 2;
        flags |= 3;
        
        if (ns == 0) one_surfel = NULL;
        else if (ns == 1) one_surfel = &s[0];
        else {
            
            m_surfels = (Surfel **) arena.c_alloc(int(ns * sizeof(Surfel *))); //Problema, coincidencia de nombres con vector clase.
            
            for (int i = 0; i < ns; ++i) {
                //std::cout << "surfel_nums: " << surfel_nums[i] << " i: " << i << std::endl;
                m_surfels[i] = &s[surfel_nums[i]];//Possible problem. 
                //std::cout << "Surfel num: " << surfel_nums[i] << std::endl;
                //std::cout << "Surfel info vector: " << s[surfel_nums[i]].x << " " << s[surfel_nums[i]].y << " " << s[surfel_nums[i]].z << std::endl;
                //std::cout << "Surfel info pointer: " << m_surfels[i]->x << " " << m_surfels[i]->y << " " << m_surfels[i]->z << std::endl;
                
            }
            
        }
        
    }
    
    //Init interior node.
    void init_interior(int axis, float s) {
        
        split = s;
        flags &= ~3;
        flags |= axis;
        
    }
    
    float split_pos() const { return split; }
    float num_surfels() const { return n_surfels >> 2; }
    float split_axis() const { return flags & 3; }
    float is_leaf() const { return (flags & 3) == 3; }
    
    union {
        u_int32_t flags;
        float split;
        u_int32_t n_surfels;
    };
    
    union {
        u_int32_t above_child;
        Surfel *one_surfel;//Ask pointer size ¿32bits?
        Surfel **m_surfels;
    };
    
};

struct BoundEdge {
    
    BoundEdge() {}
    
    BoundEdge(float tt, int sn, bool starting) {
        t = tt;
        surfel_num = sn;
        type = starting ? START : END;
    }
    
    bool operator<(const BoundEdge &e) const {
        if (t == e.t) return (int)type < (int)e.type;
        else return t < e.t;
    }
    
    float t;
    int surfel_num;
    enum { START, END } type;
    
};

struct KdToDo {
    
    const KdAccelNode *node;
    float tmin,tmax;
    
};

KdTreeAccel::KdTreeAccel(std::vector<Surfel> p, int icost, int tcost, float ebonus, int maxs, int max_depth) {
    
    isect_cost = icost; traversal_cost = tcost; empty_bonus = ebonus; max_surfels = maxs; 
    surfels=p;
    
    next_free_node = n_alloced_nodes = 0;
    cur_surf_id = 0; 
    
    if (max_depth <= 0){
        float v = float(surfels.size());
        int log = ((*(int *) &v) >> 23) - 127;
        max_depth = 8 + 1.3f * log;             //Recommended max depth of the tree.
    }
    
    //std::cout << "Bounds x,y,z: " << bounds.p_min.x << std::endl;
    
    //Compute point bounds.
    std::vector<BBox> surfel_bounds;
    surfel_bounds.reserve(surfels.size());
    for (u_int32_t i = 0; i < surfels.size(); ++i) {
        BBox b = BBox(surfels[i], surfels[i].radius);
        bounds = bounds.uni(b);
        surfel_bounds.push_back(b);
    }
    
    //Allocate work mem here.
    BoundEdge *edges[3];
    for (int i = 0; i < 3; ++i) {
        edges[i] = new BoundEdge[2*surfels.size()];
    }
    
    int *surfels0 = new int[surfels.size()];
    int *surfels1 = new int[(max_depth+1) * surfels.size()];
    
    //All surfels are in the first node, because it covers the whole world.
    int *surfel_nums = new int[surfels.size()]; 
    for (u_int32_t i = 0; i < surfels.size(); ++i) surfel_nums[i] = i;
    
    //Build tree recursively.
    build_tree(0,bounds,surfel_bounds,surfel_nums,int(surfels.size()),max_depth,edges,surfels0,surfels1,0); //Careful bad refinements.
    
    //std::cout << "Tree size: " << nodes[0].is_leaf() << std::endl;
    
    // Free working memory for kd-tree construction
    delete[] surfel_nums;
    for (int i = 0; i < 3; ++i)
        delete[] edges[i];
    delete[] surfels0;
    delete[] surfels1;
    
}

KdTreeAccel::~KdTreeAccel() {
    free(nodes);//Possible issue calling from arena. Should be without arena look at memory.h
}

void KdTreeAccel::build_tree(int node_num, BBox node_bounds, std::vector<BBox> all_surfel_bounds, int *surfel_nums, int n_surfels, int depth, BoundEdge *edges[3], int *surfels0, int *surfels1,int bad_refines) {
    
    //Get initial block of tree nodes.
    if (next_free_node == n_alloced_nodes){ 
        
        int n_alloc = std::max(2 * n_alloced_nodes, 512);
        KdAccelNode *n = (KdAccelNode *)malloc(n_alloc * sizeof(KdAccelNode));
        
        if (n_alloced_nodes > 0) {
            memcpy(n, nodes, n_alloced_nodes * sizeof(KdAccelNode));
            free(nodes);
        }
        
        nodes = n;
        n_alloced_nodes = n_alloc;
    }
    
    ++next_free_node;
    //std::cout << "Node num: " << node_num << std::endl;
    //Init leaf node if termination criteria met. Either sufficiently small number of surfels in the region o max depth reached.
    //std::cout << "n_surfels,max_surfels: " << n_surfels <<"," <<max_surfels <<","<<depth << std::endl;
    if (n_surfels <= max_surfels || depth == 0) {   //We create a leaf if we reach max depth or small number of surfels in the region.
        nodes[node_num].init_leaf(surfel_nums, n_surfels, surfels,arena);
        //std::cout << "Leaf surf 1: " << n_surfels << std::endl;
        /*if (n_surfels == 1) {
            Surfel *ms = nodes[node_num].one_surfel;
            std::cout << "Surfel info 1: " << ms->x << " " << ms->y << " " << ms->z << std::endl;
        } else {
            Surfel **m_surfels = nodes[node_num].m_surfels;
            for (u_int32_t i = 0; i < n_surfels; ++i) {
                Surfel *ms = m_surfels[i];
                //std::cout<< "Loop value: " << i << std::endl;
                std::cout << "Surfel info 2: " << ms->x << " " << ms->y << " " << ms->z << std::endl;//Same ray always. ERROR
                
            }
        }*/
        return;
    }
    
    //Init interior node and continue recursion.
    
    //Choose split axis position for inferior node.
    int best_axis = -1, best_offset = -1;
    float best_cost = INFINITY;
    float old_cost = isect_cost * float(n_surfels);
    bdm::Vector d = node_bounds.p_max - node_bounds.p_min;
    float total_sa = (2.f * (d.x*d.y + d.x*d.z + d.y*d.z));
    float inv_total_sa = 1.f/total_sa;
    
    //Choose axis to split along.
    int axis;
    if (d.x > d.y && d.x > d.z) axis = 0;
    else axis = (d.y > d.z) ? 1 : 2;
    
    //std::cout << "Axis: " << axis << std::endl;
    
    int retries = 0;
retry_split:
    
    //Init edges for axis.
    for (int i = 0; i < n_surfels; ++i) {
        int sn = surfel_nums[i];
        const BBox &bbox = all_surfel_bounds[sn]; //Possible issue.
        edges[axis][2*i] = BoundEdge(bbox.p_min[axis],sn,true);
        edges[axis][2*i+1] = BoundEdge(bbox.p_max[axis], sn, false);
    }
    
    std::sort(&edges[axis][0],&edges[axis][2*n_surfels]);
    
    //Compute cost of all splits for axis to find best.
    int n_below = 0, n_above = n_surfels;
    for (int i = 0; i < 2*n_surfels; ++i) {
        if (edges[axis][i].type == BoundEdge::END) --n_above;
        float edget = edges[axis][i].t;
        //std::cout << "edget,node_bounds_min,node_bounds_max,i: " << edget<<"," << node_bounds.p_min[axis] << "," << node_bounds.p_max[axis] <<"," <<i << std::endl;//<----- Resume bug correct here.
        if (edget > node_bounds.p_min[axis] && edget < node_bounds.p_max[axis]) {
            
            //Compute cost for a split in the ith edge.
            int other_axis[3][2] = { {1,2}, {0,2}, {0,1} };
            int other_axis0 = other_axis[axis][0];
            int other_axis1 = other_axis[axis][1];
            
            float below_sa = 2 * (d[other_axis0] * d[other_axis1] + 
                                  (edget - node_bounds.p_min[axis]) * 
                                  (d[other_axis0] + d[other_axis1]));
            float above_sa = 2 * (d[other_axis0] * d[other_axis1] + 
                                  (node_bounds.p_max[axis] - edget) * 
                                  (d[other_axis0] + d[other_axis1]));
            
            float s_below = below_sa * inv_total_sa;
            float s_above = above_sa * inv_total_sa;
            
            float eb = (n_above == 0 || n_below == 0) ? empty_bonus : 0.f;
            float cost = traversal_cost + isect_cost * (1.f - eb) * (s_below * n_below + s_above * n_above);
            //std::cout << "cost: " << cost << std::endl;
            //Update best split if is the lowest cost so far.
            if (cost < best_cost) {
                best_cost = cost;
                best_axis = axis;
                best_offset = i;
            }
            
        }
        if (edges[axis][i].type == BoundEdge::START) --n_below;
    }
    
    //Create leaf if no good splits were found.
    //std::cout << "Best_axis,retries: " << best_axis << "," << retries << std::endl;
    if (best_axis == -1 && retries < 2) {
        ++retries;
        axis = (axis+1) % 3;
        goto retry_split; //Ask if goto is allowed...
    }
    
    if (best_cost > old_cost) ++bad_refines;
    if ((best_cost > 4.f * old_cost && n_surfels < 16) || best_axis == -1 || bad_refines == 3) {
        nodes[node_num].init_leaf(surfel_nums, n_surfels, surfels, arena);
        //std::cout << "Leaf surf 2: " << n_surfels << std::endl;
        return;
    }
    
    //Classify primitives with respect to split.
    int n0 = 0, n1 = 0;
    
    for (int i = 0; i < best_offset; ++i) 
        if (edges[best_axis][i].type == BoundEdge::START) 
            surfels0[n0++] = edges[best_axis][i].surfel_num;
    
    for (int i = best_offset + 1; i < 2*n_surfels; ++i) 
        if (edges[best_axis][i].type == BoundEdge::END) 
            surfels1[n1++] = edges[best_axis][i].surfel_num;
    
    //Recursively initialize children nodes.
    float tsplit = edges[best_axis][best_offset].t;
    nodes[node_num].init_interior(best_axis, tsplit);
    //std::cout << nodes[node_num].is_leaf() << std::endl;
    BBox bounds0 = node_bounds, bounds1 = node_bounds;
    bounds0.p_max[best_axis] = bounds1.p_min[best_axis] = tsplit;
    build_tree(node_num + 1, bounds0, all_surfel_bounds, surfels0, n0, depth - 1, edges, surfels0, surfels1 + n_surfels, bad_refines);
    nodes[node_num].above_child = next_free_node;
    build_tree(nodes[node_num].above_child, bounds1, all_surfel_bounds, surfels1, n1, depth - 1, edges, surfels0, surfels1+n_surfels, bad_refines);
    
}

bool KdTreeAccel::intersect(Ray ray) {
    
    //Compute initial parametric range of ray inside kd-tree extent.
    float tmin, tmax;
    
    if (!bounds.intersect_p(ray, &tmin, &tmax)) return false;
    
    //Prepare to traverse the kd-tree for ray.
    int ray_id = cur_surf_id++;
    bdm::Vector inv_dir(1.f/ray.d.x, 1.f/ray.d.y, 1.f/ray.d.z); //Why invert?
    
    #define MAX_TODO 64
    KdToDo todo[MAX_TODO];
    int todo_pos = 0;
    
    //Traverse nodes in order for ray.
    bool hit = false;
    const KdAccelNode *node = &nodes[0];
    
    while (node != NULL) {
        
        //Bail out if we found a hit closer than the current node.
        if (ray.maxt < tmin) break; //Recap ray creation being careful with tmin and tmax.
        if (!node->is_leaf()) {
            //Process interior node.
            //Compute parametric distance along ray to split plane.
            int axis = node->split_axis();
            float tplane = (node->split_pos() - ray.o[axis]) * inv_dir[axis];
            
            //Get node children pointers for ray.
            const KdAccelNode *first_child, *second_child;
            int below_first = ray.o[axis] <= node->split_pos();
            if (below_first) {
                first_child = node + 1;
                second_child = &nodes[node->above_child];
            } else {
                first_child = &nodes[node->above_child];
                second_child = node + 1;
            }
            
            //Advance to next child node, possibly enqueue other child.
            if (tplane > tmax || tplane < 0) node = first_child;
            else if (tplane < tmin) node = second_child;
            else {
                
                //Enqueue second child in todo list.
                todo[todo_pos].node = second_child;
                todo[todo_pos].tmin = tplane;
                todo[todo_pos].tmax = tmax;
                ++todo_pos;
                
                node = first_child;
                tmax = tplane;
                
            }
            
        } else {
            
            //Check for intersections inside leaf node.
            u_int32_t n_surfels = node->num_surfels();
            //std::cout<< "Surfels in each node: " << n_surfels << std::endl;
            if (n_surfels == 1) {
                Surfel *ms = node->one_surfel;
                
                //Check one surfel inside leaf.
                //std::cout << "Surfel info1: " << ms->x << " " << ms->y << " " << ms->z << std::endl;
                if (ms->intersect(ray)) hit = true;
                
            } else {
                Surfel **m_surfels = node->m_surfels;
                for (u_int32_t i = 0; i < n_surfels; ++i) {
                    Surfel *ms = m_surfels[i];
                    //std::cout<< "Loop value: " << i << std::endl;
                    //std::cout << "Surfel info2: " << ms->x << " " << ms->y << " " << ms->z << std::endl;//Same ray always. ERROR
                    //Check one primitive inside leaf node.
                    if (ms->intersect(ray)) hit = true; 
                    
                }
            }
            
            //Grab next node to process from todo list.
            if (todo_pos > 0) {
                
                --todo_pos;
                node = todo[todo_pos].node;
                tmin = todo[todo_pos].tmin;
                tmax = todo[todo_pos].tmax;
                
            } else break;
            
        }
    }
    
    return hit;
    
}

