//
//  sampler.h
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 4/18/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#ifndef black_diamond_engine_sampler_h
#define black_diamond_engine_sampler_h

#include <vector>
#include "point.h"

class Sampler {
    
public:
	typedef enum {
		RANDOM_BOX,
		RANDOM_POLAR,
		RANDOM_UNWRAP,
        UNIFORM,
		UNIFORM_POLAR,
		UNIFORM_UNWRAP
	} TSamplingMethod;
    
	Sampler() { _method = RANDOM_POLAR; }
    
	void compute(bdm::Point center, bdm::Vector normal, std::vector<bdm::Vector> &samples);
	inline void setMethod(TSamplingMethod sm) { _method = sm; }
	inline void nextSampler() { 
		if(_method==(TSamplingMethod)5) 
			_method=(TSamplingMethod)0; 
		else 
			_method = (TSamplingMethod)(_method+1); 
	}
	inline TSamplingMethod getMethod() { return _method; }
	std::string getMethodString();
    
private:
    
	void computeRandomBox(bdm::Point center, bdm::Vector normal, std::vector<bdm::Vector> &samples);
	void computeRandomPolar(bdm::Point center, bdm::Vector normal, std::vector<bdm::Vector> &samples);
	void computeRandomUnwrap(bdm::Point center, bdm::Vector normal, std::vector<bdm::Vector> &samples);
	void computeUniformPolar(bdm::Point center, bdm::Vector normal, std::vector<bdm::Vector> &samples);
	void computeUniformUnwrap(bdm::Point center, bdm::Vector normal, std::vector<bdm::Vector> &samples);
    void computeUniform(bdm::Point center, bdm::Vector normal, std::vector<bdm::Vector> &samples);
    
	TSamplingMethod _method;
    std::vector<bdm::Vector> _storedSamples;
};

#endif
