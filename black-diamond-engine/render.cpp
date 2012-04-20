//
//  render.cpp
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 1/17/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#include "render.h"
#include "point.h"
#include "scene.h"
#include "image.h"
#include "bdesettings.h"
#include "montecarlo.h"
#include <math.h>
#include <iostream>
#include <vector>

extern BDESettings settings;

//Used for testing the visibility of objects for shadow tracing.
struct VisibilityTester {
    
    void set_segment(bdm::Point p1, float eps1, bdm::Point p2, float eps2, float time) {
        
        float dist = (p1 - p2).length();
        r = Ray(p1, (p2 - p1)/dist, eps1, dist*(1.f - eps2)); //Time missing.
        
    }
    
    void set_ray(bdm::Point p, float eps, bdm::Vector w, float time) {
        
        r = Ray(p, w, eps, INFINITY);
        
    }
    
    //Returns true if the path to the light is clear.
    bool unoccluded(Scene &s) {
        
        return !s.kd_tree->intersect_p(r);
        
    }
    
    Ray r;
    
};

//Transforms world points to camera points.
void Render::trans_world_to_cam() {
    
    #pragma omp parallel for
    for (int i = 0; i < s.cloud.size(); i++) {
                
        bdm::Point new_point = cam.get_camera_point(s.cloud[i]); //Takes care of the scene points.
        
        s.cloud[i].x = new_point.x;
        s.cloud[i].y = new_point.y;
        s.cloud[i].z = new_point.z;
        
    }
    
    for (int i = 0; i < s.lights.size(); i++) {
        
        bdm::Point new_point = cam.get_camera_point(s.lights[i].light_pos); //Takes care of the light position.
        
        s.lights[i].light_pos.x = new_point.x;
        s.lights[i].light_pos.y = new_point.y;
        s.lights[i].light_pos.z = new_point.z;
        
    }
    
}

//Function that filters the scene points so only those that are in the viewing frustum remain.
void Render::filter_frustum() {
    
    std::vector<Surfel> frustum_cloud(0); 
    
    for (int i = 0; i < s.cloud.size(); i++) {
        if ((s.cloud[i].z >= cam.hither && s.cloud[i].z <= cam.yon) && 
            (fabsf(s.cloud[i].x) >=0 && fabsf(s.cloud[i].x) <= fabsf(tanf(cam.angle_x/2)*s.cloud[i].z)) && 
            (fabsf(s.cloud[i].y) >=0 && fabsf(s.cloud[i].y) <= fabsf(tanf(cam.angle_y/2)*s.cloud[i].z))) {
            frustum_cloud.push_back(s.cloud[i]);
        }
    }
    
    s.cloud = frustum_cloud;
    
}

//This function renders the scene without rays. It needs color values for each surfel.
void Render::get_pixel_info_ortho(){
        
    //Transforms the points to orthographic perspective.
    bdm::Transform ortho_trans = ortho_trans.orthographic(cam.hither, cam.yon);
    float world_max_x = fabsf(tanf(cam.angle_x/2)*cam.yon);
    float world_max_y = fabsf(tanf(cam.angle_y/2)*cam.yon);
    //std::vector<std::vector<std::vector<short> > > pix_vec(x_res,std::vector< std::vector<short> > (y_res, std::vector<short>(3,1337)));
    std::vector<std::vector<std::vector<short> > > pix_vec;
    std::vector<Surfel> clones(0);
    
    // Set up sizes. (HEIGHT x WIDTH x CHANNELS)
    pix_vec.resize(x_res);                     //Fors are faster than the direct declaration of the array with constructor.
    for (int i = 0; i < x_res; ++i)
        pix_vec[i].resize(y_res);
    for (int i = 0; i < x_res; ++i) 
        for (int j = 0; j < y_res; j++)
            pix_vec[i][j].resize(3);
    
    
    for (int i = 0; i < x_res; i++) 
        for (int j = 0; j < y_res; j++) 
            pix_vec[i][j][0] = 1337;
    
    for (int i = 0; i < s.cloud.size(); i++) {
        
        bdm::Transform scales = scales.scale(1,1,1);//80
        bdm::Point point = scales(s.cloud[i]); 
        point = ortho_trans(point);
        
        int screen_x = roundf((x_res/(world_max_x*2))*(point.x+world_max_x));
        int screen_y = roundf((y_res/(world_max_y*2))*(point.y+world_max_y));
        
        if (pix_vec[x_res-screen_x][y_res-screen_y][0] == 1337) {
            
            //pix_vec[x_res-screen_x][y_res-screen_y] = point.color;
            pix_vec[x_res-screen_x][y_res-screen_y][0] = s.cloud[i].r;
            pix_vec[x_res-screen_x][y_res-screen_y][1] = s.cloud[i].g;
            pix_vec[x_res-screen_x][y_res-screen_y][2] = s.cloud[i].b;
            
        } else {
            
            //std::cout << point.x << " " << point.y << std::endl;
            //This code checks which surfel is closest to the camera if they have the same x and y coords. 
            int j=0;
            while (j < clones.size() && (clones[j].x != point.x || clones[j].y != point.y)) j++; //Search for point in clones.
            //if (clones.size() != 0) std::cout << "clones x:" << clones[j].x << "point x:" << point.x << "clones y:" << clones[j].y << "point y:" << point.y << std::endl;
            //If there is no point (x,y) in clones yet, we look for the z and add it.
            if (clones.size() == 0 || (clones[j].x != point.x || clones[j].y != point.y)) { 
                
                j=0;
                while (j < i && (s.cloud[j].x != point.x && s.cloud[j].y != point.y)) j++;
                
                if (s.cloud[j].z < point.z) clones.push_back(s.cloud[j]);
                else {
                    
                    //clones.push_back(point);
                    clones.push_back(Surfel(point.x,point.y,point.z,s.cloud[i].r,s.cloud[i].g,s.cloud[i].b,s.cloud[i].radius));
                    //pix_vec[x_res-screen_x][y_res-screen_y] = point.color;
                    pix_vec[x_res-screen_x][y_res-screen_y][0] = s.cloud[i].r;
                    pix_vec[x_res-screen_x][y_res-screen_y][1] = s.cloud[i].g;
                    pix_vec[x_res-screen_x][y_res-screen_y][2] = s.cloud[i].b;
                    
                }
                                
            } else {
                                
                if (clones[j].z > point.z) {
                    
                    //clones[j] = point;
                    clones[j] = Surfel(point.x,point.y,point.z,s.cloud[i].r,s.cloud[i].g,s.cloud[i].b,s.cloud[i].radius);
                    //pix_vec[x_res-screen_x][y_res-screen_y] = point.color;
                    pix_vec[x_res-screen_x][y_res-screen_y][0] = s.cloud[i].r;
                    pix_vec[x_res-screen_x][y_res-screen_y][1] = s.cloud[i].g;
                    pix_vec[x_res-screen_x][y_res-screen_y][2] = s.cloud[i].b;
                    
                }
                
            }
            
        }
        //pix_vec[x_res-screen_x][y_res-screen_y] = point.color;
        //s.cloud[i] = point;
        
    }
    
    /*for (int it=0; it<clones.size(); it++) {
        std::cout << "x: " << clones[it].x << " y: " << clones[it].y << " z: " << clones[it].z << " color: " << clones[it].r << std::endl;
    }*/
    std::cout << clones.size() << std::endl;
    
    char f_name[200] = "/Users/osurfer3/Desktop/test.png";
    
    //Recordar que casi me he quedado sin stack. Y ademas tengo que devolver pixels con puntero.
    Image *im = new Image(f_name,x_res,y_res,pix_vec);
    
    im->write_png_file(f_name);
    
}

//Funtion in charge of creating rays for ray tracing of the scene.
void Render::get_rays() { 
    
    float hither_max_x = fabsf(tanf(cam.angle_x/2)*cam.hither);
    float hither_max_y = fabsf(tanf(cam.angle_y/2)*cam.hither);
        
    rays.resize(x_res);                     //Fors are faster than the direct declaration of the array with constructor.
    for (int i = 0; i < x_res; ++i)
        rays[i].resize(y_res);
    
    #pragma omp parallel for
    for (int i = 0; i < x_res; i++){
        for (int j = 0; j < y_res; j++) {
            
            //Creates one ray through each pixels center.
            bdm::Point origin = bdm::Point(0.f,0.f,0.f);
            float x = (hither_max_x*2 / x_res)*i - hither_max_x;
            float y = (hither_max_y*2 / y_res)*j - hither_max_y;
            float z = cam.hither;
            //std::cout << x << " " << y << " " << z << std::endl;
            bdm::Vector dir = (bdm::Point(x,y,z) - origin).normalize();
            //NORMALIZAR RAYO!!! - Luego la maxt solo necesita hacerlo avanzar yon -hither
            rays[i][j] = Ray(bdm::Point(0,0,0),dir,0.f,cam.yon);
    
        }
    }
}

//This functions renders the scene without using the k-d tree.
//====TODO==== Hay que actualizarlo para que funcione con materiales.
void Render::get_ray_hits() {
    
    for (int i = 0; i < x_res; i++){ 
        #pragma omp parallel for
        for (int j = 0; j < y_res; j++) { 
            for (int k = 0; k < s.cloud.size(); k++) {
                
                //Checks for intersections with a sphere with radius equal to the surfels radius.
                float A = rays[i][j].d.dot(rays[i][j].d);
                float B = (rays[i][j].o - s.cloud[k]).dot(rays[i][j].d);
                float C = (rays[i][j].o - s.cloud[k]).dot(rays[i][j].o - s.cloud[k]) - powf(s.cloud[k].radius,2.f);
                
                float disc = B*B - A*C;
                
                if (disc < 0.f) {
                    //std::cout << "Ray miss" << std::endl;
                } else {
                    
                    float root = sqrtf(disc);
                    float t;
                    
                    if (root >=0 && root < 0.0000001) t = -B/A;
                    else {
                        t = (-B - root)/A; //En principio la t mas pequeña deberia ser siempre esta. Pero hay que comprobarlo.
                        
                        //float t2 = (-B + root)/A; 
                        
                        //std::cout << t1 << " " << t2 << std::endl;
                    }
                    
                    if (t < rays[i][j].t_hit) {
                        rays[i][j].hit = s.cloud[k];
                        rays[i][j].t_hit = t;
                    }
                    
                }
                    
            }
        }
    }
    std::vector<std::vector<std::vector<short> > > pix_vec;
    
    pix_vec.resize(x_res);                     //Fors are faster than the direct declaration of the array with constructor.
    for (int i = 0; i < x_res; ++i)
        pix_vec[i].resize(y_res);
    for (int i = 0; i < x_res; ++i)
        for (int j = 0; j < y_res; j++)
            pix_vec[i][j].resize(3);
    
    
    for (int i = 0; i < x_res; i++) {
        for (int j = 0; j < y_res; j++) {
            pix_vec[x_res-i-1][y_res-j-1][0] = rays[i][j].hit.r; //Cuidao en get pixel res!!! falta el menos 1. Y puede dar bad_Acces en el extremo.
            pix_vec[x_res-i-1][y_res-j-1][1] = rays[i][j].hit.g;
            pix_vec[x_res-i-1][y_res-j-1][2] = rays[i][j].hit.b;
        }   
    }
    
    char f_name[200] = "/Users/osurfer3/Desktop/test2.png";
    
    //Recordar que casi me he quedado sin stack. Y ademas tengo que devolver pixels con puntero.
    Image *im = new Image(f_name,x_res,y_res,pix_vec);
    
    im->write_png_file(f_name);
    
}

//This function takes care of the shading of the material.
void Render::shading(Ray &ray) {
    
    std::vector<float> rgb(3);
    
    MonteCarlo mc = MonteCarlo();
    rgb = mc.integrate(s,ray.hit);
    
    ray.hit.r = rgb[0];
    ray.hit.g = rgb[1];
    ray.hit.b = rgb[2];
    
    //Old illumination model.
    //---------------------------------------------------
    
    /*bdm::Point hit_point = ray(ray.t_hit);
    
    //Ambient contribution.
    ray.hit.r = ray.hit.mat.ambient[0];
    ray.hit.g = ray.hit.mat.ambient[1];
    ray.hit.b = ray.hit.mat.ambient[2];
    
    for (int k = 0; k < s.lights.size(); k++) {
        
        VisibilityTester vis;
        
        vis.set_segment(hit_point, 0.1f, s.lights[k].light_pos, 0.f, ray.t_hit);//Antes era hit_p,0.||hit_p,settings.min_dist
        if (!vis.unoccluded(s)) continue; //If shadow continue.
        
        //Diffuse contribution.
        bdm::Vector v_s = (s.lights[k].light_pos - hit_point).normalize();
        bdm::Vector normal;
        if(!settings.normal_est) normal = (hit_point - ray.hit).normalize(); //Sphere normal.
        else normal = ray.hit.normal; //Estimated normal.
         
        //std::cout << ray.hit.normal.x << " " << ray.hit.normal.y << " " << ray.hit.normal.z << std::endl;
        //std::cout << s.cloud[0].normal.x << " " << s.cloud[0].normal.y << " " << s.cloud[0].normal.z << std::endl;
        
        float m_dot_s = v_s.dot(normal);
        if (m_dot_s > 0.001f) { //Cuidado deberia comparar con 0;
            ray.hit.r = fminf(ray.hit.r + m_dot_s * ray.hit.mat.diffuse[0],255); 
            ray.hit.g = fminf(ray.hit.g + m_dot_s * ray.hit.mat.diffuse[1],255); 
            ray.hit.b = fminf(ray.hit.b + m_dot_s * ray.hit.mat.diffuse[2],255); 
        }
        
        //Specular contribution.
        bdm::Vector h = (v_s + (-(ray.d))).normalize(); //Mirar porque se mete en 0,0,20 deberia bloquear todo.
        float m_dot_h = h.dot(normal);
        if (m_dot_h > 0.001f) { //Cuidado deberia comparar con 0;
            float phong = powf(m_dot_h, ray.hit.mat.exp);
            ray.hit.r = fminf(ray.hit.r + phong * ray.hit.mat.specular[0],255); 
            ray.hit.g = fminf(ray.hit.g + phong * ray.hit.mat.specular[1],255); 
            ray.hit.b = fminf(ray.hit.b + phong * ray.hit.mat.specular[2],255); 
        }*/
        
        //Hit av.
        //float av_r=0,av_g=0,av_b=0;
        //float w_r=0,w_g=0,w_b=0;
        
        /*av_r += ray.hit.r;
        w_r++;
        av_g += ray.hit.g;
        w_g++;
        av_b += ray.hit.b;
        w_b++;
        */
        /*ray.hit.r = 10;
        ray.hit.g = 10;
        ray.hit.b = 10;
        
        for (int i = 0; i < ray.hitlist.size(); i++) {
            float real_dist = (ray.hit - ray(ray.hitlist_t[i])).length();
            if(real_dist > ray.hit.radius) continue;
            
            hit_point = ray(ray.hitlist_t[i]);
            //Diffuse contribution.
            bdm::Vector v_s = (s.lights[k].light_pos - hit_point).normalize();
            bdm::Vector normal;
            if(!settings.normal_est) normal = (hit_point - ray.hitlist[i]).normalize(); //Sphere normal.
            else normal = ray.hitlist[i].normal; //Estimated normal.
            
            //std::cout << ray.hit.normal.x << " " << ray.hit.normal.y << " " << ray.hit.normal.z << std::endl;
            //std::cout << s.cloud[0].normal.x << " " << s.cloud[0].normal.y << " " << s.cloud[0].normal.z << std::endl;
            
            float m_dot_s = v_s.dot(normal);
            if (m_dot_s > 0.001f) { //Cuidado deberia comparar con 0;
                ray.hit.r = fminf(ray.hit.r + m_dot_s * ray.hit.mat.diffuse[0],255); 
                ray.hit.g = fminf(ray.hit.g + m_dot_s * ray.hit.mat.diffuse[1],255); 
                ray.hit.b = fminf(ray.hit.b + m_dot_s * ray.hit.mat.diffuse[2],255); 
            }
            
            //Specular contribution.
            bdm::Vector h = (v_s + (-(ray.d))).normalize(); //Mirar porque se mete en 0,0,20 deberia bloquear todo.
            float m_dot_h = h.dot(normal);
            if (m_dot_h > 0.001f) { //Cuidado deberia comparar con 0;
                float phong = powf(m_dot_h, ray.hit.mat.exp);
                ray.hit.r = fminf(ray.hit.r + phong * ray.hit.mat.specular[0],255); 
                ray.hit.g = fminf(ray.hit.g + phong * ray.hit.mat.specular[1],255); 
                ray.hit.b = fminf(ray.hit.b + phong * ray.hit.mat.specular[2],255); 
            }
            
            float dn = (hit_point - ray.hitlist[i]).length();
            float rn = ray.hitlist[i].radius;
            av_r += ray.hit.r*(1-(dn/rn));
            w_r+=(1-(dn/rn));
            av_g += ray.hit.g*(1-(dn/rn));
            w_g+=(1-(dn/rn));
            av_b += ray.hit.b*(1-(dn/rn));
            w_b+=(1-(dn/rn));
            
            ray.hit.r = 10;
            ray.hit.g = 10;
            ray.hit.b = 10;
            
        }
        //std::cout << av_r << " " << w_r << std::endl;
        ray.hit.r = fminf(av_r/w_r,255); 
        ray.hit.g = fminf(av_g/w_g,255); 
        ray.hit.b = fminf(av_b/w_b,255);
        //------
        */
    //} //Ultimo corchete para old illumination model.

}

//This function renders the scene using the k-d tree.
void Render::get_kd_ray_hits() {
        
    //Calculates where the rays should pass using AA.
    float sub_pix_x = fabsf(rays[0][1].d.x - rays[0][0].d.x) * 2;
    float sub_pix_y = fabsf(rays[1][0].d.y - rays[0][0].d.y) * 2;
        
    for (int i = 0; i < x_res; i++){ 
        //std::cout << i << std::endl;
        #pragma omp parallel for 
        for (int j = 0; j < y_res; j++) {
            
            //AA Supersampling x4
            if (settings.AA) {
                
                //std::cout << i << " " << j << std::endl;
                
                float r=0.f,g=0.f,b=0.f;
                
                Ray aux_ray = rays[i][j];
                
                //First ray on x val + 0.5.
                aux_ray.d.x = aux_ray.d.x + sub_pix_x;
                
                Ray hit = s.kd_tree->intersect(aux_ray);
                
                if (hit.hit.radius != 0.f) {
                    
                    shading(hit);
                    
                    r += 0.25f * hit.hit.r;
                    g += 0.25f * hit.hit.g;
                    b += 0.25f * hit.hit.b;
                    
                }
                    
                //Second ray on x - 0.5.
                aux_ray = rays[i][j];
                
                aux_ray.d.x = aux_ray.d.x - sub_pix_x;
                
                hit = s.kd_tree->intersect(aux_ray);
                
                if (hit.hit.radius != 0.f) {
                    
                    shading(hit);
                    
                    r += 0.25f * hit.hit.r;
                    g += 0.25f * hit.hit.g;
                    b += 0.25f * hit.hit.b;
                    
                }
                
                //Third ray y + 0.5.
                aux_ray = rays[i][j];
                
                aux_ray.d.y = aux_ray.d.y + sub_pix_y;
                
                hit = s.kd_tree->intersect(aux_ray);
                
                if (hit.hit.radius != 0.f) {
                    
                    shading(hit);
                    
                    r += 0.25f * hit.hit.r;
                    g += 0.25f * hit.hit.g;
                    b += 0.25f * hit.hit.b;
                
                }
                
                //Fourth and last ray on y - 0.5.
                aux_ray = rays[i][j];
                
                aux_ray.d.y = aux_ray.d.y - sub_pix_y;
                
                hit = s.kd_tree->intersect(aux_ray);
                
                if (hit.hit.radius != 0.f) {
                    
                    shading(hit);
                    
                    r += 0.25f * hit.hit.r;
                    g += 0.25f * hit.hit.g;
                    b += 0.25f * hit.hit.b;
                
                }
                
                // Then the contribution of each ray is added.
                hit.hit.r = r;
                hit.hit.g = g;
                hit.hit.b = b;
                
                
                rays[i][j] = hit;

            } else {
                
                Ray hit = s.kd_tree->intersect(rays[i][j]);
                
                //Illumination.
                if (hit.hit.radius != 0.f) {
                    //std::cout << i << " " << j << std::endl;
                    shading(hit);
                    /*if (i == x_res-304-1 && j == y_res-222-1) {
                     std::cout << hit.hit.normal.x << " " << hit.hit.normal.y << " " << hit.hit.normal.z << std::endl;
                     std::cout << hit.hit.x << " " << hit.hit.y << " " << hit.hit.z << std::endl;
                     }*/
                }
                
                rays[i][j] = hit;
            }
        }
    }
    std::vector<std::vector<std::vector<short> > > pix_vec;
    
    pix_vec.resize(x_res);                     //Fors are faster than the direct declaration of the array with constructor.
    
    for (int i = 0; i < x_res; ++i)
        pix_vec[i].resize(y_res);
    for (int i = 0; i < x_res; ++i) 
        for (int j = 0; j < y_res; j++)
            pix_vec[i][j].resize(3);
    
    
    for (int i = 0; i < x_res; i++) 
        for (int j = 0; j < y_res; j++) {
            pix_vec[x_res-i-1][y_res-j-1][0] = rays[i][j].hit.r; //Cuidao en get pixel res!!! falta el menos 1. Y puede dar bad_Acces en el extremo.
            pix_vec[x_res-i-1][y_res-j-1][1] = rays[i][j].hit.g;
            pix_vec[x_res-i-1][y_res-j-1][2] = rays[i][j].hit.b;
        }   
    
    
    char f_name[200] = "/Users/osurfer3/Desktop/test2.png";
    
    //Recordar que casi me he quedado sin stack. Y ademas tengo que devolver pixels con puntero.
    Image *im = new Image(f_name,x_res,y_res,pix_vec);
    
    im->write_png_file(f_name);
    
}
