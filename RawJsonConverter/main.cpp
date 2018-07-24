//
//  main.cpp
//  RawJsonConverter
//
//  Created by Zaim Abbasi on 28/08/2017.
//  Copyright © 2017 Zaim Abbasi. All rights reserved.
//

#include <iostream>
#include "json_converter.h"

#define BUILD_ALL   0

#define WORLD_COUNT 3
#define LEVEL_COUNT 5

int main(int argc, const char * argv[]) {
    
    std::string resource;
    std::string target;
    
    if (!BUILD_ALL)
    {
        int world = 2;
        int level = 4;
        
        resource = "resource/level_"+std::to_string(world)+"_"+std::to_string(level)+".json";
        target = "target/world_"+std::to_string(world)+"/level_"+std::to_string(level)+".json";
        
        JsonConverter::buildLevelFile(resource, target, world, level);
    }
    else
    {
        for (int i = 2; i < WORLD_COUNT; ++i)
        {
            for (int j = 0; j < LEVEL_COUNT; ++j)
            {
                resource = "resource/level_"+std::to_string(i)+"_"+std::to_string(j)+".json";
                target = "target/world_"+std::to_string(i)+"/level_"+std::to_string(j)+".json";
                
                JsonConverter::buildLevelFile(resource, target, i, j);
            }
        }
    }
    
    return 0;
}
