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
    
    JsonConverter::buildLevelFile("resource/level_0_0.json", "target/world_0/level_0.json", 0, 0);
    
    return 0;
}
