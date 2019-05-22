#include <iostream>
#include "rube_helper.h"

#define BUILD_ALL   1

#define WORLD_COUNT 3
#define LEVEL_COUNT 5

int main(int argc, const char * argv[]) {
    
    std::string res_path;
    std::string tar_path;
    
    if (!BUILD_ALL)
    {
        int world_id = 2;
        int level_id = 4;
        
        res_path = "res/level_"+std::to_string(world_id)+"_"+std::to_string(level_id)+".json";
        tar_path = "tar/level_"+std::to_string(world_id)+"_"+std::to_string(level_id)+".json";
        
        RubeHelper::parse_level(res_path, tar_path, world_id, level_id);
    }
    else
    {
        for (int i = 0; i < WORLD_COUNT; ++i)
        {
            for (int j = 0; j < LEVEL_COUNT; ++j)
            {
                res_path = "res/level_"+std::to_string(i)+"_"+std::to_string(j)+".json";
                tar_path = "tar/level_"+std::to_string(i)+"_"+std::to_string(j)+".json";
                
                RubeHelper::parse_level(res_path, tar_path, i, j);
            }
        }
    }
    
    return 0;
}
