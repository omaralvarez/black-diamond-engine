//
//  memoryarena.h
//  black-diamond-engine
//
//  Created by Luis Omar Alvarez Mures on 2/14/12.
//  Copyright (c) 2012 UDC. All rights reserved.
//

#ifndef black_diamond_engine_memoryarena_h
#define black_diamond_engine_memoryarena_h

#include <vector>
#include <stdint.h>
#include <cstdlib>

//Memory management for k-d tree creation.
class MemoryArena {
    
    uint32_t cur_block_pos, block_size;
    char *current_block;
    std::vector<char *> used_blocks, available_blocks;
    
public:
    MemoryArena(uint32_t bs = 32768) {
        block_size = bs;
        cur_block_pos = 0;
        current_block = (char *)alloc_aligned(block_size);
    }
    
    void *alloc_aligned(size_t size) {
        //#ifndef L1_CACHE_SIZE   //Cache alignment here! use memalign instead fo malloc?
        //#define L1_CACHE_SIZE 64
        
        return malloc(size);
        
    }
    
    void free_aligned(void *ptr) {
        
        free(ptr);
        
    }
    
    void *c_alloc(uint32_t sz) {
        
        sz = ((sz+7) & (~7));
        if (cur_block_pos + sz > block_size) {
            used_blocks.push_back(current_block);
            if (available_blocks.size() && sz <= block_size) {
                current_block = available_blocks.back();
                available_blocks.pop_back();
            } else current_block = (char *)alloc_aligned(std::max(sz,block_size));
            cur_block_pos = 0;
        }
        
        void *ret = current_block + cur_block_pos;
        cur_block_pos += sz;
        
        return ret;
        
    }
    
    void free_all() {
        
        cur_block_pos = 0;
        while (used_blocks.size()) {
            available_blocks.push_back(used_blocks.back());
            used_blocks.pop_back();
        }
        
    }
    
};

#endif
