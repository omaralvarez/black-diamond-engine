//
//  montecarlo.cpp
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 4/18/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#include <iostream>
#include "montecarlo.h"
#include "ray.h"
//Cambiar escena a puntero. Surfel puntero tambien.
std::vector<float> MonteCarlo::integrate(Scene *s, Ray *view, int level) {
    
    std::vector<float> rgb(3,0);
    std::vector<float> sum_rgb(3,0);
    std::vector<bdm::Vector> samples(n_samples);
    float inv_samples = 1.f/n_samples;
    //Calculate samples for Monte Carlo integration.
    static Sampler sampler;
    sampler.compute(view->hit, view->hit.normal, samples);
    //BRDF *b = Material::get_brdf("BRDF_DIF");
    //delete b;
        
    for (int i = 0; i < n_samples; i++) {
        
        //For each sample check for intersections.
        Ray ray = Ray(view->hit,samples[i],0.5f,INFINITY);
        Ray hit = ray; //Cuando la luz este en el kd_tree sobra hit.
        
        s->kd_tree->intersect(&hit);
        
        //Check if the sample intersects with any light.
        //for (int j = 0; j < s->lights.size(); j++) s->lights[j].intersect(&ray);
        //std::cout << level <<"-------" << std::endl;
        //If the ray intersected with other surfels and they are closer than light intersection if any.
        if (hit.t_hit != INFINITY && hit.t_hit < ray.t_hit) {
            //std::cout << "Inter." << std::endl;
            
            //If it intersects with other surfels, calculate MC again.
            if (hit.hit.mat.emit) {
                
                //b->brdf(hit.d, view->d, &view->hit, &hit.hit);
                float cos_t = hit.d.dot(view->hit.normal);
                rgb[0] = hit.hit.mat.diffuse[0] * cos_t * hit.hit.mat.emit;
                rgb[1] = hit.hit.mat.diffuse[1] * cos_t * hit.hit.mat.emit;
                rgb[2] = hit.hit.mat.diffuse[2] * cos_t * hit.hit.mat.emit;
                
                /*bdm::Vector h = (hit.d + (-(view->d))).normalize();
                float cos_h = h.dot(view->hit.normal);
                float phong = powf(cos_h, ray.hit.mat.exp);
                rgb[0] += hit.hit.mat.specular[0] * phong * hit.hit.mat.emit;
                rgb[1] += hit.hit.mat.specular[1] * phong * hit.hit.mat.emit;
                rgb[2] += hit.hit.mat.specular[2] * phong * hit.hit.mat.emit;*/
                
            } else if (level < 1) {
                
                MonteCarlo new_mc = MonteCarlo();
                //todo.push_back(&new_mc);
                rgb = new_mc.integrate(s, &hit,level+1);
                
            } /*else {
                //Check if you can see a light from the sample.
                //for (int j = 0; j < s->lights.size(); j++) s->lights[j].intersect(&ray);
            }*/
            
        } 
        
        //If the ray intersected with a light and is closer than the other intersections.
        if (ray.t_hit != INFINITY && hit.t_hit >= ray.t_hit) {
            //std::cout << "Light." << std::endl;
            rgb[0] = view->hit.mat.diffuse[0];
            rgb[1] = view->hit.mat.diffuse[1];
            rgb[2] = view->hit.mat.diffuse[2];
            
        }
        
        //If the ray doesn't intersect with anything ambient contribution.
        if (ray.t_hit == INFINITY && hit.t_hit == INFINITY) {
            //std::cout << "Nothing." << std::endl;
            rgb[0] = view->hit.mat.ambient[0];
            rgb[1] = view->hit.mat.ambient[1];
            rgb[2] = view->hit.mat.ambient[2];
            
        }
        //std::cout << level <<"*********" << std::endl;
        sum_rgb[0] += rgb[0];
        sum_rgb[1] += rgb[1];
        sum_rgb[2] += rgb[2];
        
    }
    
    //Return avg rgb values from all samples.
    sum_rgb[0] *= inv_samples;
    sum_rgb[1] *= inv_samples;
    sum_rgb[2] *= inv_samples;
    
    sum_rgb[0] *= view->hit.mat.diffuse[0];
    sum_rgb[1] *= view->hit.mat.diffuse[1];
    sum_rgb[2] *= view->hit.mat.diffuse[2];
    
    /*sum_rgb[0] = fminf(sum_rgb[0], 1.f);
    sum_rgb[1] = fminf(sum_rgb[1], 1.f);
    sum_rgb[2] = fminf(sum_rgb[2], 1.f);*/
    
   // delete b;
    
    return sum_rgb;
    
}