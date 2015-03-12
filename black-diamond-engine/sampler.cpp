/*
 *	sampler.cpp
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

#include <iostream>
#include "sampler.h"
#include "transform.h"
#include <stdlib.h>

#define NORMALIZED_RAND (rand()/(float)RAND_MAX)
#define PI			3.14159265f
#define TWO_PI		6.28318531f
#define HALF_PI		1.57079633f
#define IMPORTANCE_FACTOR 0.3f

std::string Sampler::getMethodString() {
    
	if(_method == UNIFORM) {
		return "Uniform";
	} else if(_method == RANDOM) {
		return "Random";
	} else if(_method == IMPORTANCE) {
		return "Importance (soon)";
	}
	return "";
    
}

void Sampler::compute(bdm::Point center, bdm::Vector normal, std::vector<bdm::Vector> &samples) {
    
	if(_method == UNIFORM) {
		computeUniform(center, normal, samples);
	} else if(_method == RANDOM) {
		computeRandom(center, normal, samples);
	} else if(_method == IMPORTANCE) {
		computeImportance(center, normal, samples);
        
	}
}

void Sampler::computeRandom(bdm::Point center, bdm::Vector normal, std::vector<bdm::Vector> &samples) {
    
	bdm::Vector p;
	bdm::Transform t = t.rotate(bdm::Vector(0,1,0),normal);
	for(unsigned int i=0; i<samples.size(); i++) {
		p.x=1; p.y=1; p.z=1;
		while(p.length()>1) {
			p.x = NORMALIZED_RAND*2.0f-1.0f;
			p.z = NORMALIZED_RAND*2.0f-1.0f;
			p.y = NORMALIZED_RAND;
			p.normalize();
		}
		samples[i] = t(p);
        
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

		samples[i] = t(_storedSamples[i]);

	}
    
}

void Sampler::computeImportance(bdm::Point center, bdm::Vector normal, std::vector<bdm::Vector> &samples) {
    
    bdm::Vector p;
	//osg::Matrix m = osg::Matrixd::rotate(osg::Vec3(0,0,1), normal);
	float minDist = IMPORTANCE_FACTOR * TWO_PI/sqrt((float)samples.size());
	for(unsigned int i=0; i<samples.size(); i++) {
		p.x=1; p.y=1; p.z=1;
		bool valid = false;
		while(!valid) {
			p.x = NORMALIZED_RAND*2.0f-1.0f;
			p.z = NORMALIZED_RAND*2.0f-1.0f;
			p.y = NORMALIZED_RAND;
			if(p.length()>1) continue;
			p.normalize();
            
			valid = true;
            bdm::Transform t = t.rotate(bdm::Vector(0,1,0),normal);
			p = t(p);
			for(unsigned int j=0; j<i; j++) {
				if((p-samples[j]).length()<minDist) {
					valid = false;
					break;
				}
			}
		}
        
		samples[i] = p;
	}
    
}
