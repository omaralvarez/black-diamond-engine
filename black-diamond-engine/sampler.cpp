//
//  sampler.cpp
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 4/18/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#include <iostream>

#include "sampler.h"
#include "transform.h"

//#include <osg/Quat>
//#include <osg/Matrix>

#include <stdlib.h>

#define NORMALIZED_RAND (rand()/(float)RAND_MAX)
#define PI			3.14159265f
#define TWO_PI		6.28318531f
#define HALF_PI		1.57079633f

std::string Sampler::getMethodString() {
	if(_method == RANDOM_BOX) {
		return "Random Box";
	} else if(_method == RANDOM_POLAR) {
		return "Random Polar";
	} else if(_method == RANDOM_UNWRAP) {
		return "Random UnWrap";
	} else if(_method == UNIFORM_POLAR) {
		return "Uniform Polar";
	} else if(_method == UNIFORM_UNWRAP) {
		return "Uniform Unwrap";
	} else if(_method == UNIFORM) {
		return "Uniform";
	} else return "Error";
}

void Sampler::compute(bdm::Point center, bdm::Vector normal, std::vector<bdm::Vector> &samples) {
    
	if(_method == RANDOM_BOX) {
		computeRandomBox(center, normal, samples);
	} else if(_method == RANDOM_POLAR) {
		computeRandomPolar(center, normal, samples);
	} else if(_method == RANDOM_UNWRAP) {
		computeRandomUnwrap(center, normal, samples);
	} else if(_method == UNIFORM_POLAR) {
		computeUniformPolar(center, normal, samples);
	} else if(_method == UNIFORM_UNWRAP) {
		computeUniformUnwrap(center, normal, samples);
	} else if(_method == UNIFORM) {
		computeUniform(center, normal, samples);
	}
}


void Sampler::computeRandomBox(bdm::Point center, bdm::Vector normal, std::vector<bdm::Vector> &samples) {
	bdm::Vector p;
	for(unsigned int i = 0; i < samples.size(); i++) {
		do {
			p.x = NORMALIZED_RAND - 0.5f;
			p.y = NORMALIZED_RAND - 0.5f;
			p.z = NORMALIZED_RAND - 0.5f;
		} while(p.dot(normal) < 0.3f); //Era 0 no 0.3 
		p.normalize();
		samples[i] = p;
	}
}

void Sampler::computeRandomPolar(bdm::Point center, bdm::Vector normal, std::vector<bdm::Vector> &samples) {
	bdm::Vector p;
	float alpha, beta;
	bdm::Transform t = t.rotate(bdm::Vector(0,1,0),normal);
	for(unsigned int i=0; i<samples.size(); i++) {
		alpha = NORMALIZED_RAND * TWO_PI;
		beta = NORMALIZED_RAND * HALF_PI;
		p.x = cosf(alpha);
		p.z = sinf(alpha);
		p.y = sinf(beta);
		samples[i] = t(p);
        //std::cout << "Veredict: " << samples[i].dot(normal) << std::endl;
	}
    //std::cout << "--------------"<<std::endl;
}

void Sampler::computeRandomUnwrap(bdm::Point center, bdm::Vector normal, std::vector<bdm::Vector> &samples) {
    
	bdm::Vector p;
	for(unsigned int i=0; i<samples.size(); i++) {
		p.x=2; p.y=2; p.z=2;
		while( ((p.dot(normal))<0) || (p.length()>1)) {
			p.x = NORMALIZED_RAND*2.0f-1.0f;
			p.y = NORMALIZED_RAND*2.0f-1.0f;
			p.z = NORMALIZED_RAND*2.0f-1.0f;
			p.normalize();
		}
		samples[i] = p;
	}
    
}

void Sampler::computeUniform(bdm::Point center, bdm::Vector normal, std::vector<bdm::Vector> &samples) {
    
	if(_storedSamples.size() != samples.size()) {
        
		_storedSamples.resize(samples.size());
		unsigned int nsquare = (unsigned int)sqrt((float)samples.size());
		unsigned int latitudes = nsquare;
		unsigned int longitudes = (unsigned int)(samples.size()/nsquare);
		unsigned int remainingSamples = (unsigned int)(samples.size() % nsquare);
		bdm::Vector p;
		unsigned int i=0, j=0, k=0;
		float alpha=0, beta=(HALF_PI/(float)latitudes)/2.f;
		while(j<latitudes) {
			bool justOnce = false;
			if((j%3)==0)
				alpha = 0;
			else if((j%3)==1)
				alpha = (TWO_PI/(float)longitudes)/3.f;
			else
				alpha = 2.f*(TWO_PI/(float)longitudes)/3.f;
			i = 0;
			while(i<longitudes) {
				p.x = cosf(alpha) * cosf(beta);
				p.z = sinf(alpha) * cosf(beta);
				p.y = sinf(beta);
				p.normalize();
				_storedSamples[k] = p;
				
				if(j==0) {
					alpha += TWO_PI/(float)(longitudes+remainingSamples);
					if(i==remainingSamples && !justOnce) { i=0; justOnce = true; }
				} else {
					alpha += TWO_PI/(float)longitudes;
				}
                
				i++; k++;
			}
			beta += HALF_PI/(float)latitudes;
			j++;
		}
	}
    
    bdm::Transform t = t.rotate(bdm::Vector(0,1,0),normal);
    
	for(unsigned int i=0; i<samples.size(); i++) {
        //std::cout << "Rotation: " << normal.x << " " << normal.y << " " <<normal.z << std::endl;
        //std::cout << "Before: " << _storedSamples[i].x <<" " << _storedSamples[i].y <<" " <<_storedSamples[i].z << std::endl;
        //std::cout << "Veredict bef: " << _storedSamples[i].dot(bdm::Vector(0,1,0)) << std::endl;
		samples[i] = t(_storedSamples[i]);
        //normal = bdm::Vector(1,1,sqrtf(2));
        //_storedSamples[i] = bdm::Vector(1,1,sqrtf(2));
        //samples[i] = bdm::Vector(normal.x+_storedSamples[i].x,normal.y+_storedSamples[i].y,sinf(acosf(sqrtf(powf(normal.x,2)+powf(normal.y,2)))) * cosf(asinf(sqrtf(powf(_storedSamples[i].x,2)+powf(_storedSamples[i].y,2))) ));
        //std::cout << "Veredict af: " << samples[i].dot(normal) << std::endl;
        //std::cout << "After: " << samples[i].x <<" "<< samples[i].y << " "<<samples[i].z << std::endl;

	}
    //std::cout << "--------------"<<std::endl;
    
    /*if (center.x >= 0.575 && center.x <= 0.576 && center.y <= -0.136 && center.y >= -0.137 && center.z >= 9.559 && center.z <= 9.56){
        
        for(unsigned int i=0; i<samples.size(); i++) {
            std::cout << "Veredict bef: " << _storedSamples[i].dot(bdm::Vector(0,1,0)) << std::endl;
            std::cout << "Veredict af: " << samples[i].dot(normal) << std::endl;
            std::cout << "After: " << samples[i].x <<" "<< samples[i].y << " "<<samples[i].z << std::endl;
        }
        std::cout << "--------------"<<std::endl;
    }*/
    
}

void Sampler::computeUniformPolar(bdm::Point center, bdm::Vector normal, std::vector<bdm::Vector> &samples) {
    
}

void Sampler::computeUniformUnwrap(bdm::Point center, bdm::Vector normal, std::vector<bdm::Vector> &samples) {
    
}