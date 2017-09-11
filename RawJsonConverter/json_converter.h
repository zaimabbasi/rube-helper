//
//  json_converter.h
//  RawJsonConverter
//
//  Created by Zaim Abbasi on 28/08/2017.
//  Copyright © 2017 Zaim Abbasi. All rights reserved.
//

#ifndef json_converter_h
#define json_converter_h

#include <stdio.h>
#include <iostream>
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

class JsonConverter
{
public:
    JsonConverter() { };
    ~JsonConverter() { };
    
    static bool buildLevelFile(std::string resource_path,
                               std::string target_path,
                               int world_index,
                               int level_index);
private:
    static int getRopeStructureIndex(std::string bodyName);
    static int getRopeIndex(std::string bodyName);
    static int getRopeBodyIndex(std::string bodyName);
    static int getHingeIndex(std::string bodyName);
    static char * getResName(std::string resPath);
};

#endif /* json_converter_h */
