//
//  kdtreeaccel.cpp
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 2/13/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#include <iostream>
#include <vector>
#include <set>
#include "kdtreeaccel.h"
#include "surfel.h"
#include "memoryarena.h"//Possible optimization problem.

struct KdAccelNode {
    
    //Init leaf node.
    void init_leaf(int *surfel_nums,int ns, std::vector<Surfel> &s, MemoryArena &arena) { 
        
        n_surfels = ns << 2;
        flags |= 3;
        
        if (ns == 0) one_surfel = NULL;
        else if (ns == 1) one_surfel = &s[surfel_nums[0]];
        else {
            
            m_surfels = (Surfel **) arena.c_alloc(int(ns * sizeof(Surfel *)));
            //#pragma omp parallel for //Why not thread safe?
            for (int i = 0; i < ns; ++i) {
        
                m_surfels[i] = &s[surfel_nums[i]];
                
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
        Surfel *one_surfel;     
        Surfel **m_surfels;
    };
    
};

//Useful for determining which points are on each side of a space division.
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

/*KdTreeAccel::KdTreeAccel() {
    isect_cost = 0; traversal_cost = 0; empty_bonus = 0; max_surfels = 0; 
    
    next_free_node = n_alloced_nodes = 0;
}*/

KdTreeAccel::KdTreeAccel(std::vector<Surfel> &p, int icost, int tcost, float ebonus, int maxs, int max_depth) {
    
    isect_cost = icost; traversal_cost = tcost; empty_bonus = ebonus; max_surfels = maxs; 
    surfels=p;
    
    next_free_node = n_alloced_nodes = 0;
    cur_surf_id = 0; 
    
    //If no max depth is given.
    if (max_depth <= 0){
        float v = float(surfels.size());
        int log = ((*(int *) &v) >> 23) - 127;
        max_depth = 8 + 1.3f * log;             //Recommended max depth of the tree.
    }
        
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
    for (int i = 0; i < 3; ++i) {   //For each axis 0, 1 and 2. (4.11,209)
        edges[i] = new BoundEdge[2*surfels.size()]; //Each tree node needs 2*number of surfel, bbox faces (left and right) per axis.
    }
    
    int *surfels0 = new int[surfels.size()];
    int *surfels1 = new int[(max_depth+1) * surfels.size()];
    
    //All surfels are in the first node, because it covers the whole world.
    int *surfel_nums = new int[surfels.size()];  //Create an array with surfel numbers to indicate which surfels overlap the node.
    for (u_int32_t i = 0; i < surfels.size(); ++i) surfel_nums[i] = i;
    
    //Build tree recursively.
    //Responsible for deciding if is interior node or leaf updating everything that needs to be updated.
    build_tree(0,bounds,surfel_bounds,surfel_nums,int(surfels.size()),max_depth,edges,surfels0,surfels1,0); //Careful bad refinements.
   
    // Free working memory for kd-tree construction
    delete[] surfel_nums;
    for (int i = 0; i < 3; ++i)
        delete[] edges[i];
    delete[] surfels0;
    delete[] surfels1;
    
}

KdTreeAccel::~KdTreeAccel() {
    if(n_alloced_nodes){
        free(nodes); 
    }
}

void KdTreeAccel::build_tree(int node_num, BBox node_bounds, std::vector<BBox> all_surfel_bounds, int *surfel_nums, int n_surfels, int depth, BoundEdge *edges[3], int *surfels0, int *surfels1,int bad_refines) {
    
    //Get initial block of tree nodes.
    if (next_free_node == n_alloced_nodes){ 
        
        int n_alloc = std::max(2 * n_alloced_nodes, 512); //If everything is used allocate memory with twice as many entries.
        KdAccelNode *n = (KdAccelNode *)malloc(n_alloc * sizeof(KdAccelNode));
        
        if (n_alloced_nodes > 0) {
            memcpy(n, nodes, n_alloced_nodes * sizeof(KdAccelNode));
            free(nodes);
        }
        
        nodes = n;
        n_alloced_nodes = n_alloc;
    }
    
    ++next_free_node;
    
    //Init leaf node if termination criteria met. Either sufficiently small number of surfels in the region o max depth reached.
    if (n_surfels <= max_surfels || depth == 0) {   //We create a leaf if we reach max depth or small number of surfels in the region.

        nodes[node_num].init_leaf(surfel_nums, n_surfels, surfels,arena);
        return;
        
    }
    
    //Init interior node and continue recursion.
    
    //Choose split axis position for inferior node.
    int best_axis = -1, best_offset = -1; //Record which axis and bounding box that gave the best cost so far.
    float best_cost = INFINITY;     //Best cost so far.
    float old_cost = isect_cost * float(n_surfels);
    bdm::Vector d = node_bounds.p_max - node_bounds.p_min;
    float total_sa = (2.f * (d.x*d.y + d.x*d.z + d.y*d.z)); //Superficie cubo grande(no es cubo). Node surface area.
    float inv_total_sa = 1.f/total_sa;
    
    //Choose axis to split along.
    int axis;
    if (d.x > d.y && d.x > d.z) axis = 0; //Choose axis with largest spatial extent, hepls getting square regions of space.
    else axis = (d.y > d.z) ? 1 : 2;    //Coge la arista mas grande ya sea x, y o z. Si no le gusta la que escogio luego prueba el resto.
        
    int retries = 0;
retry_split:
    
    //Init edges for axis.
    #pragma omp parallel for
    for (int i = 0; i < n_surfels; ++i) {
        int sn = surfel_nums[i];    //Choose numbers of surfels inside of the node.
        const BBox &bbox = all_surfel_bounds[sn]; //Get the bounding boxes.
        edges[axis][2*i] = BoundEdge(bbox.p_min[axis],sn,true); //Create one edge in the left face of the bounding box.
        edges[axis][2*i+1] = BoundEdge(bbox.p_max[axis], sn, false); //Create another to the right of the bounding box.
    }
    
    std::sort(&edges[axis][0],&edges[axis][2*n_surfels]); //Sort the bboxes to sweep them from first in the axis to last.
    
    //Compute cost of all splits for axis to find best.
    int n_below = 0, n_above = n_surfels; //Number of primitives on each side of the split.
    for (int i = 0; i < 2*n_surfels; ++i) { //From 0 to total splits(2*number of surfels in node).
        if (edges[axis][i].type == BoundEdge::END) --n_above; //Si se hace el split en este punto la que hace el split ya no pertenece.
        float edget = edges[axis][i].t;
        if (edget > node_bounds.p_min[axis] && edget < node_bounds.p_max[axis]) { //Si el punto de corte esta dentro de la BBox del nodo.
            
            //Compute cost for a split in the ith edge.
            int other_axis[3][2] = { {1,2}, {0,2}, {0,1} }; //Herramienta para calcular las SA. Tiene los indices de las otras dos ejes.
            int other_axis0 = other_axis[axis][0];
            int other_axis1 = other_axis[axis][1];
            
            float below_sa = 2 * (d[other_axis0] * d[other_axis1] + 
                                  (edget - node_bounds.p_min[axis]) * 
                                  (d[other_axis0] + d[other_axis1])); //Area de cubo a un lado.
            float above_sa = 2 * (d[other_axis0] * d[other_axis1] + 
                                  (node_bounds.p_max[axis] - edget) * 
                                  (d[other_axis0] + d[other_axis1])); //Area del otro lado.
            
            float s_below = below_sa * inv_total_sa; //Probabilidad de un lado.
            float s_above = above_sa * inv_total_sa; //Probabilidad de otro.
            
            float eb = (n_above == 0 || n_below == 0) ? empty_bonus : 0.f; //Si en uno de los lados no quedan puntos bonificacion.
            float cost = traversal_cost + isect_cost * (1.f - eb) * (s_below * n_below + s_above * n_above); //Formula coste total.
            
            //Update best split if is the lowest cost so far.
            if (cost < best_cost) {
                best_cost = cost;
                best_axis = axis;
                best_offset = i;
            }
            
        }
        if (edges[axis][i].type == BoundEdge::START) ++n_below; //Si al contrario que arriba se hace en el punto de la izda. si pert.
    }
    
    //Create leaf if no good splits were found. (4.12,213)
    if (best_axis == -1 && retries < 2) {
        ++retries;
        axis = (axis+1) % 3; //Siguiente eje a probar.
        goto retry_split; //Ask if goto is allowed...
    }
    
    if (best_cost > old_cost) ++bad_refines; //Si nuevo coste dividiendo es peor que el coste 
                                            //sin se aumenta bad refines que lleva la cuenta de cuantos splits malos se han hecho.
    if ((best_cost > 4.f * old_cost && n_surfels < 16) || best_axis == -1 || bad_refines == 3) {
        nodes[node_num].init_leaf(surfel_nums, n_surfels, surfels, arena);
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
    float tsplit = edges[best_axis][best_offset].t; //Coge el punto de split con mejores resultados en el mejor eje.
    nodes[node_num].init_interior(best_axis, tsplit); //Crea nodo interno.
    BBox bounds0 = node_bounds, bounds1 = node_bounds;
    bounds0.p_max[best_axis] = bounds1.p_min[best_axis] = tsplit;
    build_tree(node_num + 1, bounds0, all_surfel_bounds, surfels0, n0, depth - 1, edges, surfels0, surfels1 + n_surfels, bad_refines);
    nodes[node_num].above_child = next_free_node;
    build_tree(nodes[node_num].above_child, bounds1, all_surfel_bounds, surfels1, n1, depth - 1, edges, surfels0, surfels1+n_surfels, bad_refines);
    
}

Ray KdTreeAccel::intersect(Ray ray) {
    
    //Compute initial parametric range of ray inside kd-tree extent.
    float tmin, tmax;
    
    if (!bounds.intersect_p(ray, &tmin, &tmax)) return ray; //Obtenemos tmin y tmax que son donde el rayo corta la bounding box de la sc.
    
    //Prepare to traverse the kd-tree for ray.
    bdm::Vector inv_dir(1.f/ray.d.x, 1.f/ray.d.y, 1.f/ray.d.z); //Se hace la inversion para poder multiplicar en vez de dividir y ahorrar.
    
    #define MAX_TODO 64
    KdToDo todo[MAX_TODO]; //Define array de recorrido de arbol, con maximo de profundidad como tamaño de array. 64 deberia sobrar.
    int todo_pos = 0;
    
    //Traverse nodes in order for ray.
    Ray hit = ray; //Inicialmente no corta el rayo.
    const KdAccelNode *node = &nodes[0];
    
    while (node != NULL) {
        
        //Bail out if we found a hit closer than the current node.
        if (ray.maxt < tmin) break; //Recap ray creation being careful with tmin and tmax. //======== TODO ========= tema del yon 40.
        if (!node->is_leaf()) { //No es hoja?
            //Process interior node.
            //Compute parametric distance along ray to split plane.
            int axis = node->split_axis(); //Saca eje en el que divide el nodo.
            float tplane = (node->split_pos() - ray.o[axis]) * inv_dir[axis]; //Saca la distancia parametrica al plano de corte.
            
            //Get node children pointers for ray.
            const KdAccelNode *first_child, *second_child; //Init hijos.
            int below_first = ray.o[axis] <= node->split_pos(); //Orden en que recorre los hijos.
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
            //std::cout << " n_surfels: " << n_surfels << std::endl;
            
            if (n_surfels == 1) {
                Surfel *ms = node->one_surfel;
                
                //Check one surfel inside leaf.
                //std::cout << "Surfel info1: " << ms->x << " " << ms->y << " " << ms->z << std::endl;
                if (ms->intersect(&ray)) hit = ray;
                
            } else {
                //std::cout << n_surfels << "------------" << std::endl;
                Surfel **m_surfels = node->m_surfels;
                for (u_int32_t i = 0; i < n_surfels; ++i) {
                    Surfel *ms = m_surfels[i];
                    //std::cout<< "Loop value: " << i << std::endl;
                    //std::cout << "Surfel info2: " << ms->x << " " << ms->y << " " << ms->z << std::endl;//Same ray always. ERROR
                    //Check one surfel inside leaf node.
                    if (ms->intersect(&ray)) hit = ray; 
                    
                }
                //if(ray.hitlist.size()>1) std::cout <<ray.hitlist.size()<< std::endl;
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
    //std::cout << "---------------------" << std::endl;
    return ray; //====WARNING===== Cambie hit por ray y no he comprobado.
    
}

//Used in shadow rays, no hit info is needed so it only returns true if it intersects.
bool KdTreeAccel::intersect_p(Ray ray) {
    
    //Compute initial parametric range of ray inside kd-tree extent.
    float tmin, tmax;
    
    if (!bounds.intersect_p(ray, &tmin, &tmax)) return false; //Obtenemos tmin y tmax que son donde el rayo corta la bounding box de la sc.
    
    //Prepare to traverse the kd-tree for ray.
    bdm::Vector inv_dir(1.f/ray.d.x, 1.f/ray.d.y, 1.f/ray.d.z); //Se hace la inversion para poder multiplicar en vez de dividir y ahorrar.
    
#define MAX_TODO 64
    KdToDo todo[MAX_TODO]; //Define array de recorrido de arbol, con maximo de profundidad como tamaño de array. 64 deberia sobrar.
    int todo_pos = 0;
    
    //Traverse nodes in order for ray.
    bool hit = false; //Inicialmente no corta el rayo.
    const KdAccelNode *node = &nodes[0];
    
    while (node != NULL) {
        
        //Bail out if we found a hit closer than the current node.
        if (ray.maxt < tmin) break; //Recap ray creation being careful with tmin and tmax. //======== TODO ========= tema del yon 40.
        if (!node->is_leaf()) { //No es hoja?
            //Process interior node.
            //Compute parametric distance along ray to split plane.
            int axis = node->split_axis(); //Saca eje en el que divide el nodo.
            float tplane = (node->split_pos() - ray.o[axis]) * inv_dir[axis]; //Saca la distancia parametrica al plano de corte.
            
            //Get node children pointers for ray.
            const KdAccelNode *first_child, *second_child; //Init hijos.
            int below_first = ray.o[axis] <= node->split_pos(); //Orden en que recorre los hijos.
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
            //std::cout << " n_surfels: " << n_surfels << std::endl;
            
            if (n_surfels == 1) {
                Surfel *ms = node->one_surfel;
                
                //Check one surfel inside leaf.
                //std::cout << "Surfel info1: " << ms->x << " " << ms->y << " " << ms->z << std::endl;
                if (ms->intersect(&ray)) return true;
                
            } else {
                Surfel **m_surfels = node->m_surfels;
                for (u_int32_t i = 0; i < n_surfels; ++i) {
                    Surfel *ms = m_surfels[i];
                    //std::cout<< "Loop value: " << i << std::endl;
                    //std::cout << "Surfel info2: " << ms->x << " " << ms->y << " " << ms->z << std::endl;//Same ray always. ERROR
                    //Check one surfel inside leaf node.
                    if (ms->intersect(&ray)) return true; 
                    
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
    //std::cout << "---------------------" << std::endl;
    return hit;
    
}

//Funcion que mete en un array de *Surfels los vecinos de un punto.
//3 rayos, 1 por cada eje en 2 direcciones osea 6 rayos por punto en total.
//Posible evolucion diagonales por cada dos ejes a mayores 12 rayos total.

Surfel **KdTreeAccel::get_neighbours(bdm::Point p, float dist, u_int32_t *n_neighbours) {
    
    //Calcular rayos a partir de punto y dist.
    std::set<intptr_t> done;
    std::vector<Ray> rays(6);
    Surfel **neighbours = NULL;
    
    rays[0] = Ray(p,bdm::Vector(1,0,0),0,dist);
    rays[1] = Ray(p,bdm::Vector(-1,0,0),0,dist);
    rays[2] = Ray(p,bdm::Vector(0,1,0),0,dist);
    rays[3] = Ray(p,bdm::Vector(0,-1,0),0,dist);
    rays[4] = Ray(p,bdm::Vector(0,0,1),0,dist);
    rays[5] = Ray(p,bdm::Vector(0,0,-1),0,dist);
    
    for (int it = 0; it < 6; it++) {
        
        Ray ray = rays[it];
        
        //Compute initial parametric range of ray inside kd-tree extent.
        float tmin, tmax;
        
        if (!bounds.intersect_p(ray, &tmin, &tmax)) continue; //Obtenemos tmin y tmax que son donde el rayo corta la bounding box de la sc.
        
        //Prepare to traverse the kd-tree for ray.
        bdm::Vector inv_dir(1.f/ray.d.x, 1.f/ray.d.y, 1.f/ray.d.z); //Se hace la inversion para poder multiplicar en vez de dividir y ahorrar.
        
#define MAX_TODO 64
        KdToDo todo[MAX_TODO]; //Define array de recorrido de arbol, con maximo de profundidad como tamaño de array. 64 deberia sobrar.
        int todo_pos = 0;
        
        //Traverse nodes in order for ray.
        //bool hit = false; //Inicialmente no corta el rayo.
        const KdAccelNode *node = &nodes[0];
        
        while (node != NULL) {
            
            //Bail out if we found a hit closer than the current node.
            if (ray.maxt < tmin) break; //Recap ray creation being careful with tmin and tmax. //======== TODO ========= tema del yon 40.
            if (!node->is_leaf()) { //No es hoja?
                //Process interior node.
                //Compute parametric distance along ray to split plane.
                int axis = node->split_axis(); //Saca eje en el que divide el nodo.
                float tplane = (node->split_pos() - ray.o[axis]) * inv_dir[axis]; //Saca la distancia parametrica al plano de corte.
                
                //Get node children pointers for ray.
                const KdAccelNode *first_child, *second_child; //Init hijos.
                int below_first = ray.o[axis] <= node->split_pos(); //Orden en que recorre los hijos.
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
            
                //std::cout << "Ray: " << it << std::endl;
                
                if (done.count((intptr_t)node) == 0) {
                    
                    done.insert((intptr_t)node);
                    
                    //Check for intersections inside leaf node.
                    u_int32_t n_surfels = node->num_surfels();
                    //std::cout << "Ptr: " << (intptr_t)node << " " << n_surfels << std::endl;
                    //std::cout << " n_surfels: " << n_surfels << std::endl;
                    
                    if (n_surfels == 1) {
                        Surfel *ms = node->one_surfel;
                        if (fabsf(p.x - ms->x) > dist || fabsf(p.y - ms->y) > dist || fabsf(p.z - ms->z) > dist) continue;
                        neighbours = (Surfel **) realloc(neighbours, (*n_neighbours + 1)*sizeof(Surfel *));
                        neighbours[*n_neighbours] = ms;
                        (*n_neighbours)++;
                        
                        //Check one surfel inside leaf.
                        //std::cout << "Surfel info1: " << ms->x << " " << ms->y << " " << ms->z << std::endl;
                        //if (ms->intersect(&ray)) return true;
                        
                    } else {
                        
                        u_int32_t size_surf = *n_neighbours + n_surfels;
                        //std::cout << size_surf << std::endl;
                        Surfel **m_surfels = node->m_surfels;
                        neighbours = (Surfel **) realloc(neighbours, size_surf*sizeof(Surfel *));
                        for (u_int32_t i = 0; i < n_surfels; ++i) {
                            Surfel *ms = m_surfels[i];
                            if (fabsf(p.x - ms->x) > dist || fabsf(p.y - ms->y) > dist || fabsf(p.z - ms->z) > dist) continue;
                            neighbours[*n_neighbours] = ms;
                            (*n_neighbours)++;
                            
                            //std::cout<<*n_neighbours-1 << " Point_orig: " << ms->x << " " << ms->y << " " << ms->z << std::endl;
                            //std::cout<< "Point_neig: " << neighbours[*n_neighbours - 1]->x << " " << neighbours[*n_neighbours -1]->y << " " << neighbours[*n_neighbours - 1]->z << std::endl;
                            //std::cout << "Surfel info2: " << ms->x << " " << ms->y << " " << ms->z << std::endl;//Same ray always. ERROR
                            //Check one surfel inside leaf node.
                            //if (ms->intersect(&ray)) return true; 
                            
                        }
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
        //std::cout << "---------------------" << std::endl;
        //return hit;

        
    }
    
    return neighbours;
    
        /*u_int32_t n_surfels = nodes[node].num_surfels();
        //std::cout << " n_surfels: " << n_surfels << std::endl;
                
        Surfel **m_surfels = nodes[node].m_surfels;
        
        for (u_int32_t i = 0; i < n_surfels; i++) {
            Surfel *p_i = m_surfels[i];
            
                //Surfel p_i = cloud[i];
                //if (p_i.x == 0 && p_i.y <= -0.8883 && p_i.y >= -0.8884 && p_i.z >= 10.315 && p_i.z <= 10.316) flag = true;
                
        }*/
    
}

