#include <iostream>
#include "rube_helper.h"

#define BUILD_ALL   0

#define LEVEL_COUNT 15

int main(int argc, const char * argv[]) {
    
    std::string res_path;
    std::string tar_path;
    
    if (!BUILD_ALL)
    {
        int level_id = 1;
        
        res_path = "res/level_"+std::to_string(level_id)+".json";
        tar_path = "tar/level_"+std::to_string(level_id)+".json";
        
        RubeHelper::parse_level(res_path, tar_path, level_id);
    }
    else
    {
        for (int i = 0; i < LEVEL_COUNT; ++i)
        {
            res_path = "res/level_"+std::to_string(i)+".json";
            tar_path = "tar/level_"+std::to_string(i)+".json";
            
            RubeHelper::parse_level(res_path, tar_path, i);
        }
    }
    
    return 0;
}
