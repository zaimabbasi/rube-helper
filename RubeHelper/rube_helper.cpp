#include "rube_helper.h"

string structure = "structure";
string rope_literal = "rope";
string hinge = "hinge";
string jammer = "ball";
string platform = "platform";
string block = "block";
string star = "star";
string background = "background";

bool RubeHelper::parse_level(std::string res_path, std::string tar_path, int world_id, int level_id)
{
    ifstream input_file;
    input_file.open(res_path);
    
    if (input_file.is_open())
    {
        // reading source file
        char read_char;
        string read_string;
        input_file >> read_char;
        while (!input_file.eof())
        {
            read_string.append(&read_char, 1);
            input_file >> read_char;
        }
        
        input_file.close();
        
        rapidjson::Document input_document;
        input_document.Parse<0>(read_string.c_str());
        
        // manipulating JSON document
        int body_count = input_document["body"].Size();
        int image_count = input_document["image"].Size();
        
        // document is the root of a json message
        rapidjson::Document output_document;
        
        // define the document as an object rather than an array
        output_document.SetObject();
        
        // must pass an allocator when the object may need to allocate memory
        rapidjson::Document::AllocatorType& allocator = output_document.GetAllocator();
        
        // insert top level members
        output_document.AddMember("world_id", world_id, allocator);
        output_document.AddMember("level_id", level_id, allocator);
        
        rapidjson::Value rope_structures_array(rapidjson::kArrayType);
        rapidjson::Value blocks_array(rapidjson::kArrayType);
        rapidjson::Value platforms_array(rapidjson::kArrayType);
        rapidjson::Value stars_array(rapidjson::kArrayType);
//        rapidjson::Value bgArray(rapidjson::kArrayType);
        rapidjson::Value world_size(rapidjson::kObjectType);
        
        unsigned int maxX = 0, maxY = 0;
        
        maxX = input_document["body"][0]["position"]["x"].GetFloat();
        maxY = input_document["body"][0]["position"]["y"].GetFloat();
        
        
//        // ZERO PASS
//        int bgCount = 0;
//
//        for (int i = 0; i < image_count; ++i)
//        {
//            string imageName = input_document["image"][i]["name"].GetString();
//
//            if (imageName.find(background) != string::npos)
//            {
//                ++bgCount;
//            }
//        }
        
//        bool found;
//        for (int i = 0; i < bgCount; ++i)
//        {
//            found = false;
//            string imageName;
//            int j = 0;
//
//            for (; j < imageCount && !found; ++j)
//            {
//                imageName = inputDocument["image"][j]["name"].GetString();
//
//                if (imageName.find(background) != string::npos)
//                {
//                    if (background_id(imageName) == i)                                // here
//                    {
//                        found = true;
//                        break;
//                    }
//                }
//            }
//
//            if (found)
//            {
////                rapidjson::Value bgLayer(rapidjson::kObjectType);
//
//                rapidjson::Value res(res_name(inputDocument["image"][j]["file"].GetString()), allocator);
//                rapidjson::Value position(rapidjson::kObjectType);
//                bgLayer.AddMember("res", res, allocator);
//                position.AddMember("x", inputDocument["image"][j]["center"]["x"].GetFloat(), allocator);
//                position.AddMember("y", inputDocument["image"][j]["center"]["y"].GetFloat(), allocator);
//                bgLayer.AddMember("position", position, allocator);
//                bgArray.PushBack(bgLayer, allocator);
//            }
//        }
        
        
        // ZERO PASS
        for (int i = 0; i < body_count; i++)
        {
            string body_name = input_document["body"][i]["name"].GetString();
            
            if (maxX < input_document["body"][i]["position"]["x"].GetFloat())
            {
                maxX = input_document["body"][i]["position"]["x"].GetFloat();
            }
            if (maxY < input_document["body"][i]["position"]["y"].GetFloat())
            {
                maxY = input_document["body"][i]["position"]["y"].GetFloat();
            }
            
            // check if its rope_structure, platform, block or star
            if (body_name.find(structure) != string::npos)
            {
                if (body_name.find(jammer) != string::npos)
                {
                    // its a jammer body
                    rapidjson::Value jammer(rapidjson::kObjectType);
                    rope_structures_array.PushBack(jammer, allocator);
                }
            }
            else if (body_name.find(block) != string::npos)
            {
                rapidjson::Value block(rapidjson::kObjectType);
                rapidjson::Value position(rapidjson::kObjectType);
                rapidjson::Value fixtures_array(rapidjson::kArrayType);
                rapidjson::Value images_array(rapidjson::kArrayType);
                
                block.AddMember("global_index", i, allocator);
                
                position.AddMember("x", input_document["body"][i]["position"]["x"].GetFloat(), allocator);
                position.AddMember("y", input_document["body"][i]["position"]["y"].GetFloat(), allocator);
                block.AddMember("position", position, allocator);
                block.AddMember("type", input_document["body"][i]["type"].GetInt(), allocator);
                block.AddMember("angle", input_document["body"][i]["angle"].GetFloat(), allocator);
                
                for (int j = 0; j < input_document["body"][i]["fixture"].Size(); j++)
                {
                    fixtures_array.PushBack(input_document["body"][i]["fixture"][j], allocator);
                    fixtures_array[j].RemoveMember("name");
                }
                block.AddMember("fixture", fixtures_array, allocator);
                
                int image_count = input_document["image"].Size();
                for (int j = 0; j < image_count; j++)
                {
                    if (input_document["image"][j]["body"].GetInt() == i)
                    {
                        rapidjson::Value image(rapidjson::kObjectType);
                        rapidjson::Value res(res_name(input_document["image"][j]["file"].GetString()), allocator);
                        
                        image.AddMember("res", res, allocator);
                        image.AddMember("aspect_scale", input_document["image"][j]["aspectScale"].GetFloat(), allocator);
                        image.AddMember("scale", input_document["image"][j]["scale"].GetFloat(), allocator);
                        image.AddMember("center", input_document["image"][j]["center"], allocator);
                        //                        image.AddMember("corners", inputDocument["image"][j]["corners"], allocator);
                        
                        if (input_document["image"][j].HasMember("angle"))
                            image.AddMember("angle", input_document["image"][j]["angle"].GetFloat(), allocator);
                        else
                            image.AddMember("angle", 0.0, allocator);
                        
                        
                        images_array.PushBack(image, allocator);
                    }
                }
                
                block.AddMember("image", images_array, allocator);
                blocks_array.PushBack(block, allocator);
            }
            else if (body_name.find(platform) != string::npos)
            {
                rapidjson::Value position(rapidjson::kObjectType);
                rapidjson::Value platform(rapidjson::kObjectType);
                rapidjson::Value fixtures_array(rapidjson::kArrayType);
                rapidjson::Value image_array(rapidjson::kArrayType);
                
                platform.AddMember("global_index", i, allocator);
                
                position.AddMember("x", input_document["body"][i]["position"]["x"].GetFloat(), allocator);
                position.AddMember("y", input_document["body"][i]["position"]["y"].GetFloat(), allocator);
                platform.AddMember("position", position, allocator);
                platform.AddMember("type", input_document["body"][i]["type"].GetInt(), allocator);
                platform.AddMember("angle", input_document["body"][i]["angle"].GetFloat(), allocator);
                
                int fixtureCount = input_document["body"][i]["fixture"].Size();
                for (int j = 0; j < fixtureCount; j++)
                {
                    fixtures_array.PushBack(input_document["body"][i]["fixture"][j], allocator);
                    fixtures_array[j].RemoveMember("name");
                }
                platform.AddMember("fixture", fixtures_array, allocator);
                
                int imageCount = input_document["image"].Size();
                for (int j = 0; j < imageCount; j++)
                {
                    if (input_document["image"][j]["body"].GetInt() == i)
                    {
                        rapidjson::Value image(rapidjson::kObjectType);
                        rapidjson::Value res(res_name(input_document["image"][j]["file"].GetString()), allocator);
                        
                        image.AddMember("res", res, allocator);
                        image.AddMember("aspect_scale", input_document["image"][j]["aspectScale"].GetFloat(), allocator);
                        image.AddMember("scale", input_document["image"][j]["scale"].GetFloat(), allocator);
                        image.AddMember("center", input_document["image"][j]["center"], allocator);
//                        image.AddMember("corners", inputDocument["image"][j]["corners"], allocator);                        

                        if (input_document["image"][j].HasMember("angle"))
                            image.AddMember("angle", input_document["image"][j]["angle"].GetFloat(), allocator);
                        else
                            image.AddMember("angle", 0.0, allocator);

                        
                        image_array.PushBack(image, allocator);
                    }
                }
                
                platform.AddMember("image", image_array, allocator);
                platforms_array.PushBack(platform, allocator);
            }
            else if (body_name.find(star) != string::npos)
            {
                rapidjson::Value position(rapidjson::kObjectType);
                rapidjson::Value star(rapidjson::kObjectType);
                rapidjson::Value fixtures_array(rapidjson::kArrayType);
                rapidjson::Value images_rray(rapidjson::kArrayType);
                
                star.AddMember("global_index", i, allocator);
                
                position.AddMember("x", input_document["body"][i]["position"]["x"].GetFloat(), allocator);
                position.AddMember("y", input_document["body"][i]["position"]["y"].GetFloat(), allocator);
                star.AddMember("position", position, allocator);
                star.AddMember("type", input_document["body"][i]["type"].GetInt(), allocator);
                star.AddMember("angle", input_document["body"][i]["angle"].GetFloat(), allocator);
                
                int fixtures_count = input_document["body"][i]["fixture"].Size();
                for (int j = 0; j < fixtures_count; j++)
                {
                    fixtures_array.PushBack(input_document["body"][i]["fixture"][j], allocator);
                    fixtures_array[j].RemoveMember("name");
                }
                star.AddMember("fixture", fixtures_array, allocator);
                
                int imageCount = input_document["image"].Size();
                for (int j = 0; j < imageCount; j++)
                {
                    if (input_document["image"][j]["body"].GetInt() == i)
                    {
                        rapidjson::Value image(rapidjson::kObjectType);
                        rapidjson::Value res(res_name(input_document["image"][j]["file"].GetString()), allocator);
                        
                        image.AddMember("res", res, allocator);
                        image.AddMember("aspect_scale", input_document["image"][j]["aspectScale"].GetFloat(), allocator);
                        image.AddMember("scale", input_document["image"][j]["scale"].GetFloat(), allocator);
                        image.AddMember("center", input_document["image"][j]["center"], allocator);
                        //                        image.AddMember("corners", inputDocument["image"][j]["corners"], allocator);
                        
                        if (input_document["image"][j].HasMember("angle"))
                            image.AddMember("angle", input_document["image"][j]["angle"].GetFloat(), allocator);
                        else
                            image.AddMember("angle", 0.0, allocator);
                        
                        
                        images_rray.PushBack(image, allocator);
                    }
                }
                
                star.AddMember("image", images_rray, allocator);
                stars_array.PushBack(star, allocator);
            }
        }
        // FIRST PASS END
        
        
        // SECOND PASS
        for (int i = 0; i < body_count; i++)
        {
            string body_name = input_document["body"][i]["name"].GetString();
            
            if (body_name.find(structure) != string::npos)
            {
                if (body_name.find(jammer) != string::npos)
                {
                    // its a jammer body
                    rapidjson::Value jammer_body(rapidjson::kObjectType);
                    rapidjson::Value fixtures_array(rapidjson::kArrayType);
                    rapidjson::Value images_array(rapidjson::kArrayType);
                    
                    jammer_body.AddMember("global_index", i, allocator);
                    
                    rapidjson::Value position(rapidjson::kObjectType);
                    position.AddMember("x", input_document["body"][i]["position"]["x"].GetFloat(), allocator);
                    position.AddMember("y", input_document["body"][i]["position"]["y"].GetFloat(), allocator);
                    jammer_body.AddMember("position", position, allocator);
                    jammer_body.AddMember("type", input_document["body"][i]["type"].GetInt(), allocator);
                    jammer_body.AddMember("angle", input_document["body"][i]["angle"].GetFloat(), allocator);
                    
                    int fixtures_count = input_document["body"][i]["fixture"].Size();
                    for (int j = 0; j < fixtures_count; j++)
                    {
                        fixtures_array.PushBack(input_document["body"][i]["fixture"][j], allocator);
                        fixtures_array[j].RemoveMember("name");
                    }
                    jammer_body.AddMember("fixture", fixtures_array, allocator);
                    
                    int image_count = input_document["image"].Size();
                    for (int j = 0; j < image_count; j++)
                    {
                        if (input_document["image"][j]["body"].GetInt() == i)
                        {
                            rapidjson::Value image(rapidjson::kObjectType);
                            rapidjson::Value res(res_name(input_document["image"][j]["file"].GetString()), allocator);
                            
                            image.AddMember("res", res, allocator);
                            image.AddMember("aspect_scale", input_document["image"][j]["aspectScale"].GetFloat(), allocator);
                            image.AddMember("scale", input_document["image"][j]["scale"].GetFloat(), allocator);
                            image.AddMember("center", input_document["image"][j]["center"], allocator);
                            //                            image.AddMember("corners", inputDocument["image"][j]["corners"], allocator);
                            
                            if (input_document["image"][j].HasMember("angle"))
                                image.AddMember("angle", input_document["image"][j]["angle"].GetFloat(), allocator);
                            else
                                image.AddMember("angle", 0.0, allocator);
                            
                            
                            images_array.PushBack(image, allocator);
                        }
                    }
                    
                    jammer_body.AddMember("image", images_array, allocator);
                    
                    rope_structures_array[rope_structure_id(body_name)].AddMember("jammer", jammer_body, allocator);
                }
            }
        }
        // SECOND PASS END
        
        
        // THIRD PASS
        for (int i = 0; i < rope_structures_array.Size(); i++)
        {
            rapidjson::Value rope(rapidjson::kArrayType);
            rapidjson::Value hinge(rapidjson::kArrayType);
            
            rope_structures_array[i].AddMember("rope", rope, allocator);
            rope_structures_array[i].AddMember("hinge", hinge, allocator);

            for (int j = 0; j < 2; j++)
            {
                rapidjson::Value hinge(rapidjson::kObjectType);
                rope_structures_array[i]["hinge"].PushBack(hinge, allocator);
            }
        }
        
        // for rope 0
        for (int i = 0; i < rope_structures_array.Size(); i++)
        {
            rapidjson::Value ropes_array(rapidjson::kArrayType);
            rapidjson::Value rope(rapidjson::kObjectType);
            
            for (int j = 0; j < body_count; j++)
            {
                string body_name = input_document["body"][j]["name"].GetString();
                
                if (body_name.find(structure) != string::npos)
                {
                    
                    if (body_name.find(rope_literal) != string::npos)
                    {
                        // find ropestructure index
                        int structure_id = rope_structure_id(body_name);
                        
                        if (structure_id == i)
                        {
                            // for rope index = 0
                            if (rope_id(body_name) == 0)
                            {
                                rapidjson::Value rope_body(rapidjson::kObjectType);
                                rapidjson::Value fixtures_array(rapidjson::kArrayType);
                                rapidjson::Value images_array(rapidjson::kArrayType);
                                
                                rope_body.AddMember("global_index", j, allocator);
                                rope_body.AddMember("local_index", rope_body_id(body_name), allocator);
                                
                                rapidjson::Value position(rapidjson::kObjectType);
                                position.AddMember("x", input_document["body"][j]["position"]["x"].GetFloat(), allocator);
                                position.AddMember("y", input_document["body"][j]["position"]["y"].GetFloat(), allocator);
                                rope_body.AddMember("position", position, allocator);
                                
                                rope_body.AddMember("type", input_document["body"][j]["type"].GetInt(), allocator);
                                rope_body.AddMember("angle", input_document["body"][j]["angle"].GetFloat(), allocator);
                                
                                int fixture_count = input_document["body"][j]["fixture"].Size();
                                for (int k = 0; k < fixture_count; k++)
                                {
                                    fixtures_array.PushBack(input_document["body"][j]["fixture"][k], allocator);
                                    fixtures_array[k].RemoveMember("name");
                                }
                                rope_body.AddMember("fixture", fixtures_array, allocator);
                                
                                for (int k = 0; k < image_count; k++)
                                {
                                    if (input_document["image"][k]["body"].GetInt() == j)
                                    {
                                        rapidjson::Value image(rapidjson::kObjectType);
                                        rapidjson::Value res(res_name(input_document["image"][k]["file"].GetString()), allocator);
                                        
                                        image.AddMember("res", res, allocator);
                                        image.AddMember("aspect_scale", input_document["image"][k]["aspectScale"].GetFloat(), allocator);
                                        image.AddMember("scale", input_document["image"][k]["scale"].GetFloat(), allocator);
                                        image.AddMember("center", input_document["image"][k]["center"], allocator);
                                        //                            image.AddMember("corners", inputDocument["image"][j]["corners"], allocator);
                                        
                                        if (input_document["image"][k].HasMember("angle"))
                                            image.AddMember("angle", input_document["image"][k]["angle"].GetFloat(), allocator);
                                        else
                                            image.AddMember("angle", 0.0, allocator);
                                        
                                        
                                        images_array.PushBack(image, allocator);
                                    }
                                }
                                
                                rope_body.AddMember("image", images_array, allocator);
                                
                                ropes_array.PushBack(rope_body, allocator);
                                
                            }
                        }
                    }
                }
            }
            
            rope_structures_array[i]["rope"].PushBack(ropes_array, allocator);
        }
        
        // for rope 1
        for (int i = 0; i < rope_structures_array.Size(); i++)
        {
            rapidjson::Value ropes_array(rapidjson::kArrayType);
            rapidjson::Value rope(rapidjson::kObjectType);
            
            for (int j = 0; j < body_count; j++)
            {
                string body_name = input_document["body"][j]["name"].GetString();
                
                if (body_name.find(structure) != string::npos)
                {
                    if (body_name.find(rope_literal) != string::npos)
                    {
                        // find ropestructure index
                        int structure_id = rope_structure_id(body_name);
                        
                        if (structure_id == i)
                        {
                            // for rope index = 1
                            if (rope_id(body_name) == 1)
                            {
                                rapidjson::Value rope_body(rapidjson::kObjectType);
                                rapidjson::Value fixtures_array(rapidjson::kArrayType);
                                rapidjson::Value images_array(rapidjson::kArrayType);
                                
                                rope_body.AddMember("global_index", j, allocator);
                                rope_body.AddMember("local_index", rope_body_id(body_name), allocator);
                                
                                rapidjson::Value position(rapidjson::kObjectType);
                                position.AddMember("x", input_document["body"][j]["position"]["x"].GetFloat(), allocator);
                                position.AddMember("y", input_document["body"][j]["position"]["y"].GetFloat(), allocator);
                                rope_body.AddMember("position", position, allocator);
                                
                                rope_body.AddMember("type", input_document["body"][j]["type"].GetInt(), allocator);
                                rope_body.AddMember("angle", input_document["body"][j]["angle"].GetFloat(), allocator);
                                
                                int fixtureCount = input_document["body"][j]["fixture"].Size();
                                for (int k = 0; k < fixtureCount; k++)
                                {
                                    fixtures_array.PushBack(input_document["body"][j]["fixture"][k], allocator);
                                    fixtures_array[k].RemoveMember("name");
                                }
                                rope_body.AddMember("fixture", fixtures_array, allocator);
                                
                                for (int k = 0; k < image_count; k++)
                                {
                                    if (input_document["image"][k]["body"].GetInt() == j)
                                    {
                                        rapidjson::Value image(rapidjson::kObjectType);
                                        rapidjson::Value res(res_name(input_document["image"][k]["file"].GetString()), allocator);
                                        
                                        image.AddMember("res", res, allocator);
                                        image.AddMember("aspect_scale", input_document["image"][k]["aspectScale"].GetFloat(), allocator);
                                        image.AddMember("scale", input_document["image"][k]["scale"].GetFloat(), allocator);
                                        image.AddMember("center", input_document["image"][k]["center"], allocator);
                                        //                            image.AddMember("corners", inputDocument["image"][j]["corners"], allocator);
                                        
                                        if (input_document["image"][k].HasMember("angle"))
                                            image.AddMember("angle", input_document["image"][k]["angle"].GetFloat(), allocator);
                                        else
                                            image.AddMember("angle", 0.0, allocator);
                                        
                                        images_array.PushBack(image, allocator);
                                    }
                                }
                                
                                rope_body.AddMember("image", images_array, allocator);
                                
                                ropes_array.PushBack(rope_body, allocator);
                            }
                        }
                    }
                }
            }
            
            rope_structures_array[i]["rope"].PushBack(ropes_array, allocator);
        }
        
        // for hinge
        for (int i = 0; i < body_count; i++)
        {
            string body_name = input_document["body"][i]["name"].GetString();
            
            if (body_name.find(hinge) != string::npos)
            {
                // add hinge
                rapidjson::Value hinge_body(rapidjson::kObjectType);
                rapidjson::Value fixtures_array(rapidjson::kArrayType);
                rapidjson::Value image_array(rapidjson::kArrayType);
                
                hinge_body.AddMember("global_index", i, allocator);
                
                rapidjson::Value position(rapidjson::kObjectType);
                position.AddMember("x", input_document["body"][i]["position"]["x"].GetFloat(), allocator);
                position.AddMember("y", input_document["body"][i]["position"]["y"].GetFloat(), allocator);
                hinge_body.AddMember("position", position, allocator);
                hinge_body.AddMember("type", input_document["body"][i]["type"].GetInt(), allocator);
                hinge_body.AddMember("angle", input_document["body"][i]["angle"].GetFloat(), allocator);
                
                int fixtureCount = input_document["body"][i]["fixture"].Size();
                for (int j = 0; j < fixtureCount; j++)
                {
                    fixtures_array.PushBack(input_document["body"][i]["fixture"][j], allocator);
                    fixtures_array[j].RemoveMember("name");
                }
                hinge_body.AddMember("fixture", fixtures_array, allocator);
                
                int imageCount = input_document["image"].Size();
                for (int j = 0; j < imageCount; j++)
                {
                    if (input_document["image"][j]["body"].GetInt() == i)
                    {
                        rapidjson::Value image(rapidjson::kObjectType);
                        rapidjson::Value res(res_name(input_document["image"][j]["file"].GetString()), allocator);
                        
                        image.AddMember("res", res, allocator);
                        image.AddMember("aspect_scale", input_document["image"][j]["aspectScale"].GetFloat(), allocator);
                        image.AddMember("scale", input_document["image"][j]["scale"].GetFloat(), allocator);
                        image.AddMember("center", input_document["image"][j]["center"], allocator);
                        //                            image.AddMember("corners", inputDocument["image"][j]["corners"], allocator);
                        
                        if (input_document["image"][j].HasMember("angle"))
                            image.AddMember("angle", input_document["image"][j]["angle"].GetFloat(), allocator);
                        else
                            image.AddMember("angle", 0.0, allocator);
                        
                        image_array.PushBack(image, allocator);
                    }
                }
                
                hinge_body.AddMember("image", image_array, allocator);
                
                rope_structures_array[rope_structure_id(body_name)]["hinge"][hinge_id(body_name)] = hinge_body;
            }
        }
        // THIRD PASS END
        
        // FOURTH PASS
        for (int i = 0; i < rope_structures_array.Size(); i++)
        {
            for (int j = 0; j < rope_structures_array[i]["rope"].Size(); j++)
            {
                for (int k = 0; k < rope_structures_array[i]["rope"][j].Size(); )
                {
                    int swapperIndex = rope_structures_array[i]["rope"][j][k]["local_index"].GetInt();
                    
                    if (k == swapperIndex)
                    {
                        k++;
                    }
                    else
                    {
                        rapidjson::Value swapper(rapidjson::kObjectType);
                        
                        swapper = rope_structures_array[i]["rope"][j][k];
                        rope_structures_array[i]["rope"][j][k] = rope_structures_array[i]["rope"][j][swapperIndex];
                        rope_structures_array[i]["rope"][j][swapperIndex] = swapper;
                    }
                }
                
            }
        }
        // FOURTH PASS END
        
        // FIFTH PASS
        for (int i = 0; i < input_document["joint"].Size(); i++)
        {
            rapidjson::Value nameA(input_document["body"][input_document["joint"][i]["bodyA"].GetInt()]["name"].GetString(), allocator);
            rapidjson::Value nameB(input_document["body"][input_document["joint"][i]["bodyB"].GetInt()]["name"].GetString(), allocator);
            
            input_document["joint"][i].AddMember("nameA", nameA, allocator);
            input_document["joint"][i].AddMember("nameB", nameB, allocator);
        }
        // FIFTH PASS END
        
        
        // SIXTH PASS
        for (int i = 0; i < rope_structures_array.Size(); i++)
        {
            for (int j = 0; j < rope_structures_array[i]["rope"].Size(); j++)
            {
                if (rope_structures_array[i]["rope"][j].Size() == 0)
                {
                    rope_structures_array[i]["rope"].PopBack();
                }
            }
            
            for (int j = 0; j < rope_structures_array[i]["hinge"].Size(); j++)
            {
                if (rope_structures_array[i]["hinge"][j].MemberCount() == 0)
                {
                    rope_structures_array[i]["hinge"].PopBack();
                }
            }
        }
        // SIXTH PASS END
        
        
//        rapidjson::Value fixtures_array(rapidjson::kArrayType);
        
//        outputDocument.AddMember("bg_array", bgArray, allocator);
        output_document.AddMember("rope_structure", rope_structures_array, allocator);
        output_document.AddMember("platform", platforms_array, allocator);
        output_document.AddMember("block", blocks_array, allocator);
        output_document.AddMember("star", stars_array, allocator);
        output_document.AddMember("joint", input_document["joint"].GetArray(), allocator);
        
        world_size.AddMember("max_x", maxX, allocator);
        world_size.AddMember("max_y", maxY, allocator);
        output_document.AddMember("world_size", world_size, allocator);
        
        // writing to target file
        rapidjson::StringBuffer strbuf;
        rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
        output_document.Accept(writer);
        
        ofstream outputFile;
        outputFile.open(tar_path);
        
        outputFile << strbuf.GetString();
        
        outputFile.close();
        
        return true;
    }
    
    return false;
}

int RubeHelper::background_id(std::string imageName)
{
    char bgIndex[3];
    int chItr;
    int chItr2;
    
    chItr = ((int)background.length()) + 1;
    chItr2 = 0;
    for (; imageName[chItr] != '_'; chItr++)
    {
        bgIndex[chItr2++] = imageName[chItr];
    }
    
    return atoi(bgIndex);
}

int RubeHelper::rope_structure_id(std::string bodyName)
{
    char rope_structure_id[3];
    int itr;
    int itr_2;
    
    itr = ((int)structure.length()) + 1;
    itr_2 = 0;
    
    for (; bodyName[itr] != '_'; itr++)
    {
        rope_structure_id[itr_2++] = bodyName[itr];
    }
    
    return atoi(rope_structure_id);
}

int RubeHelper::rope_id(std::string body_name)
{
    char rope_structure_id[3];
    int itr;
    int itr_2;
    
    itr = ((int)structure.length()) + 1;
    itr_2 = 0;
    
    for (; body_name[itr] != '_'; itr++)
    {
        rope_structure_id[itr_2++] = body_name[itr];
    }
    
    char rope_id[3];
    itr += ((int)rope_literal.length()) + 2;
    itr_2 = 0;
    
    for (; body_name[itr] != '_'; itr++)
    {
        rope_id[itr_2++] = body_name[itr];
    }
    
    return atoi(rope_id);
}

int RubeHelper::rope_body_id(std::string body_name)
{
    // find ropestructure index
    char rope_structure_id[3];
    int itr;
    int itr_2;
    
    itr = ((int)structure.length()) + 1;
    itr_2 = 0;
    for (; body_name[itr] != '_'; itr++)
    {
        rope_structure_id[itr_2++] = body_name[itr];
    }
    
    // find rope index
    char rope_id[3];
    itr += ((int)rope_literal.length()) + 2;
    itr_2 = 0;
    
    for (; body_name[itr] != '_'; itr++)
    {
        rope_id[itr_2++] = body_name[itr];
    }
    
    // find rope body index
    char rope_body_id[3];
    itr++;
    itr_2 = 0;
    
    for (; itr < (int)body_name.length(); itr++)
    {
        rope_body_id[itr_2++] = body_name[itr];
    }
    
    return atoi(rope_body_id);
}

int RubeHelper::hinge_id(std::string body_name)
{
    // find ropestructure index
    char rope_structure_id[3];
    int itr;
    int itr_2;
    
    itr = (int)structure.length()+1;
    itr_2 = 0;
    for (; body_name[itr] != '_'; itr++)
    {
        rope_structure_id[itr_2++] = body_name[itr];
    }
    
    // find hinge index
    char hinge_id[3];
    
    itr += hinge.length()+2;
    itr_2 = 0;
    
    for (; itr < (int)body_name.length(); itr++)
    {
        hinge_id[itr_2++] = body_name[itr];
    }
    
    return atoi(hinge_id);
}

char * RubeHelper::res_name(std::string res_path)
{
    char * res_name;
    int i = ((int)res_path.length())-1;
    int j = 0;
    
    for (; res_path[i] != '/'; i--);
    
    res_name = new char[((int)res_path.length())-i];
    
    for (i++; i < ((int)res_path.length()); i++)
        res_name[j++] = res_path[i];
    
    res_name[j] = '\0';
    
    return res_name;
}
