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
    
    JsonConverter::buildLevelFile("resource/level_1_3.json", "target/world_1/level_3.json", 1, 3);
    
    return 0;
}
