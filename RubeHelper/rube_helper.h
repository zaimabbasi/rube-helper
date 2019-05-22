#ifndef rube_helper_h
#define rube_helper_h

#include <stdio.h>
#include <iostream>
#include <dirent.h>
#include <fstream>
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
using namespace std;

class RubeHelper
{
public:
    RubeHelper() { };
    ~RubeHelper() { };
    
    static bool parse_level(std::string res_path, std::string tar_path, int world_id, int level_id);
private:
    static int background_id(std::string image_name);
    static int rope_structure_id(std::string body_name);
    static int rope_id(std::string body_name);
    static int rope_body_id(std::string body_name);
    static int hinge_id(std::string body_name);
    static char * res_name(std::string res_path);
};

#endif /* rube_helper_h */
