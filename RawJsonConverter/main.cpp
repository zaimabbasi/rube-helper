//
//  main.cpp
//  RawJsonConverter
//
//  Created by Zaim Abbasi on 28/08/2017.
//  Copyright © 2017 Zaim Abbasi. All rights reserved.
//

#include <iostream>
#include "json_converter.h"

int main(int argc, const char * argv[]) {
    
    JsonConverter::buildLevelFile("resource/level_0_1.json", "target/world_0/level_1.json", 0, 1);
    
    return 0;
}
