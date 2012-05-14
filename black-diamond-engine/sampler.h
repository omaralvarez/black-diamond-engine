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
		UNIFORM,
		RANDOM,
		IMPORTANCE
        
	} TSamplingMethod;
    
	Sampler() { _method = RANDOM; }
    
	void compute(bdm::Point center, bdm::Vector normal, std::vector<bdm::Vector> &samples);
	inline void setMethod(TSamplingMethod sm) { _method = sm; _storedSamples.resize(0); }
	inline void nextSampler() { 
		if(_method==(TSamplingMethod)2) 
			setMethod((TSamplingMethod)0); 
		else 
			setMethod((TSamplingMethod)(_method+1)); 
        
	}
	inline TSamplingMethod getMethod() { return _method; }
	std::string getMethodString();
    
private:
    
	void computeRandom(bdm::Point center, bdm::Vector normal, std::vector<bdm::Vector> &samples);
    void computeUniform(bdm::Point center, bdm::Vector normal, std::vector<bdm::Vector> &samples);
    void computeImportance(bdm::Point center, bdm::Vector normal, std::vector<bdm::Vector> &samples);
    
	TSamplingMethod _method;
    std::vector<bdm::Vector> _storedSamples;
};

#endif
