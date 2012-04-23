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
	}
}


void Sampler::computeRandomBox(bdm::Point center, bdm::Vector normal, std::vector<bdm::Vector> &samples) {
	bdm::Vector p;
	for(unsigned int i = 0; i < samples.size(); i++) {
		do {
			p.x = NORMALIZED_RAND - 0.5f;
			p.y = NORMALIZED_RAND - 0.5f;
			p.z = NORMALIZED_RAND - 0.5f;
		} while(p.dot(normal) < 0);
		p.normalize();
		samples[i] = p;
	}
}

void Sampler::computeRandomPolar(bdm::Point center, bdm::Vector normal, std::vector<bdm::Vector> &samples) {
	/*bdm::Vector p;
	float alpha, beta;
	bdm::Transform m = m.rotate(osg::Vec3(0,0,1), normal);
	for(unsigned int i=0; i<samples.size(); i++) {
		alpha = NORMALIZED_RAND * TWO_PI;
		beta = NORMALIZED_RAND * HALF_PI;
		p.x = cos(alpha);
		p.y = sin(alpha);
		p.z = sin(beta);
		samples[i] = p * m;
	}*/
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


void Sampler::computeUniformPolar(bdm::Point center, bdm::Vector normal, std::vector<bdm::Vector> &samples) {
    
}

void Sampler::computeUniformUnwrap(bdm::Point center, bdm::Vector normal, std::vector<bdm::Vector> &samples) {
    
}