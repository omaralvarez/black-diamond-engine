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
#include "brdf.h"

//Cambiar escena a puntero. Surfel puntero tambien.
std::vector<float> MonteCarlo::integrate(Scene *s, Ray *view, int level) {
    
    std::vector<float> rgb(3,0);
    std::vector<float> sum_rgb(3,0);
    std::vector<bdm::Vector> samples(n_samples);
    float inv_samples = 1.f/n_samples;
    //Calculate samples for Monte Carlo integration.
    static Sampler sampler;
    sampler.compute(view->hit, view->hit.normal, samples);
    BRDF *brdf = BRDF::create("BRDF_SIMPLE");
    
    /*brdf->diffuse(bdm::Vector());
    brdf->specular(bdm::Vector(),bdm::Vector());*/
        
    for (int i = 0; i < n_samples; i++) {
        
        //For each sample check for intersections.
        Ray ray = Ray(view->hit,samples[i],0.5f,INFINITY);
        
        s->kd_tree->intersect(&ray);
        
        //If the ray intersected with other surfels and they are closer than light intersection if any.
        if (ray.t_hit != INFINITY) {
            
            //If it intersects with other surfels, calculate MC again.
            if (ray.hit.mat.emit) {
                                
                float dif_ref = brdf->diffuse(ray.d, view->hit.normal);
                
                rgb[0] = ray.hit.mat.diffuse[0] * dif_ref * ray.hit.mat.emit;
                rgb[1] = ray.hit.mat.diffuse[1] * dif_ref * ray.hit.mat.emit;
                rgb[2] = ray.hit.mat.diffuse[2] * dif_ref * ray.hit.mat.emit;
                
                
                float phong = brdf->specular(ray.d, view->d, view->hit.normal);
                 
                rgb[0] += ray.hit.mat.specular[0] * phong * ray.hit.mat.emit;
                rgb[1] += ray.hit.mat.specular[1] * phong * ray.hit.mat.emit;
                rgb[2] += ray.hit.mat.specular[2] * phong * ray.hit.mat.emit;
                
            } else if (level < 1) {
                
                MonteCarlo new_mc = MonteCarlo();
                //todo.push_back(&new_mc);
                rgb = new_mc.integrate(s, &ray,level+1);
                
            }
            
        } else {

        //If the ray doesn't intersect with anything ambient contribution.
            //std::cout << "Nothing." << std::endl;
            rgb[0] = view->hit.mat.ambient[0];
            rgb[1] = view->hit.mat.ambient[1];
            rgb[2] = view->hit.mat.ambient[2];
        
        }
        
        //Sum the contribution of the sample.
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
    
    delete brdf;
    
    return sum_rgb;
    
}