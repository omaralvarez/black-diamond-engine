/*
 *	sampler.h
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
