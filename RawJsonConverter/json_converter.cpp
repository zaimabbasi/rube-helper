//
//  json_converter.cpp
//  RawJsonConverter
//
//  Created by Zaim Abbasi on 28/08/2017.
//  Copyright © 2017 Zaim Abbasi. All rights reserved.
//

#include "json_converter.h"
#include <fstream>

using namespace std;

string STRUCTURE_STRING_LITERAL = "structure";
string ROPE_STRING_LITERAL = "rope";
string HINGE_STRING_LITERAL = "hinge";
string BALL_STRING_LITERAL = "ball";
string PLATFORM_STRING_LITERAL = "platform";
string BLOCK_STRING_LITERAL = "block";

bool JsonConverter::buildLevelFile(std::string resource_path,
                                   std::string target_path,
                                   int world_index,
                                   int level_index)
{
    ifstream inputFile;
    inputFile.open(resource_path);
    
    if (inputFile.is_open())
    {
        // reading source file
        char readChar;
        string readString;
        inputFile >> readChar;
        while (!inputFile.eof())
        {
            readString.append(&readChar, 1);
            inputFile >> readChar;
        }
        
        inputFile.close();
        
        rapidjson::Document inputDocument;
        inputDocument.Parse<0>(readString.c_str());
        
        // manipulating JSON document
        int bodyCount = inputDocument["body"].Size();
        int imageCount = inputDocument["image"].Size();
        
        // document is the root of a json message
        rapidjson::Document outputDocument;
        
        // define the document as an object rather than an array
        outputDocument.SetObject();
        
        // must pass an allocator when the object may need to allocate memory
        rapidjson::Document::AllocatorType& allocator = outputDocument.GetAllocator();
        
        // insert top level members
        outputDocument.AddMember("world_index", world_index, allocator);
        outputDocument.AddMember("level_index", level_index, allocator);
        
        rapidjson::Value ropeStructureArray(rapidjson::kArrayType);
        rapidjson::Value blockArray(rapidjson::kArrayType);
        rapidjson::Value platformArray(rapidjson::kArrayType);
        
        // FIRST PASS
        for (int i = 0; i < bodyCount; i++)
        {
            string bodyName = inputDocument["body"][i]["name"].GetString();
            
            // check if its rope_structure, platform, block or star
            if (bodyName.find(STRUCTURE_STRING_LITERAL) != string::npos)
            {
                if (bodyName.find(BALL_STRING_LITERAL) != string::npos)
                {
                    // its a ball body
                    rapidjson::Value ball(rapidjson::kObjectType);
                    ropeStructureArray.PushBack(ball, allocator);
                }
            }
            else if (bodyName.find(BLOCK_STRING_LITERAL) != string::npos)
            {
                rapidjson::Value block(rapidjson::kObjectType);
                rapidjson::Value position(rapidjson::kObjectType);
                rapidjson::Value fixtureArray(rapidjson::kArrayType);
                rapidjson::Value imageArray(rapidjson::kArrayType);
                
                block.AddMember("global_index", i, allocator);
                
                position.AddMember("x", inputDocument["body"][i]["position"]["x"].GetFloat(), allocator);
                position.AddMember("y", inputDocument["body"][i]["position"]["y"].GetFloat(), allocator);
                block.AddMember("position", position, allocator);
                block.AddMember("type", inputDocument["body"][i]["type"].GetInt(), allocator);
                block.AddMember("angle", inputDocument["body"][i]["angle"].GetFloat(), allocator);
                
                for (int j = 0; j < inputDocument["body"][i]["fixture"].Size(); j++)
                {
                    fixtureArray.PushBack(inputDocument["body"][i]["fixture"][j], allocator);
                    fixtureArray[j].RemoveMember("name");
                }
                block.AddMember("fixture", fixtureArray, allocator);
                
                int imageCount = inputDocument["image"].Size();
                for (int j = 0; j < imageCount; j++)
                {
                    if (inputDocument["image"][j]["body"].GetInt() == i)
                    {
                        rapidjson::Value image(rapidjson::kObjectType);
                        rapidjson::Value res(getResName(inputDocument["image"][j]["file"].GetString()), allocator);
                        
                        image.AddMember("res", res, allocator);
                        image.AddMember("aspect_scale", inputDocument["image"][j]["aspectScale"].GetFloat(), allocator);
                        image.AddMember("scale", inputDocument["image"][j]["scale"].GetFloat(), allocator);
                        image.AddMember("center", inputDocument["image"][j]["center"], allocator);
                        //                        image.AddMember("corners", inputDocument["image"][j]["corners"], allocator);
                        
                        if (inputDocument["image"][j].HasMember("angle"))
                            image.AddMember("angle", inputDocument["image"][j]["angle"].GetFloat(), allocator);
                        else
                            image.AddMember("angle", 0.0, allocator);
                        
                        
                        imageArray.PushBack(image, allocator);
                    }
                }
                
                block.AddMember("image", imageArray, allocator);
                blockArray.PushBack(block, allocator);
            }
            else if (bodyName.find(PLATFORM_STRING_LITERAL) != string::npos)
            {
                rapidjson::Value position(rapidjson::kObjectType);
                rapidjson::Value platform(rapidjson::kObjectType);
                rapidjson::Value fixtureArray(rapidjson::kArrayType);
                rapidjson::Value imageArray(rapidjson::kArrayType);
                
                platform.AddMember("global_index", i, allocator);
                
                position.AddMember("x", inputDocument["body"][i]["position"]["x"].GetFloat(), allocator);
                position.AddMember("y", inputDocument["body"][i]["position"]["y"].GetFloat(), allocator);
                platform.AddMember("position", position, allocator);
                platform.AddMember("type", inputDocument["body"][i]["type"].GetInt(), allocator);
                platform.AddMember("angle", inputDocument["body"][i]["angle"].GetFloat(), allocator);
                
                int fixtureCount = inputDocument["body"][i]["fixture"].Size();
                for (int j = 0; j < fixtureCount; j++)
                {
                    fixtureArray.PushBack(inputDocument["body"][i]["fixture"][j], allocator);
                    fixtureArray[j].RemoveMember("name");
                }
                platform.AddMember("fixture", fixtureArray, allocator);
                
                int imageCount = inputDocument["image"].Size();
                for (int j = 0; j < imageCount; j++)
                {
                    if (inputDocument["image"][j]["body"].GetInt() == i)
                    {
                        rapidjson::Value image(rapidjson::kObjectType);
                        rapidjson::Value res(getResName(inputDocument["image"][j]["file"].GetString()), allocator);
                        
                        image.AddMember("res", res, allocator);
                        image.AddMember("aspect_scale", inputDocument["image"][j]["aspectScale"].GetFloat(), allocator);
                        image.AddMember("scale", inputDocument["image"][j]["scale"].GetFloat(), allocator);
                        image.AddMember("center", inputDocument["image"][j]["center"], allocator);
//                        image.AddMember("corners", inputDocument["image"][j]["corners"], allocator);                        

                        if (inputDocument["image"][j].HasMember("angle"))
                            image.AddMember("angle", inputDocument["image"][j]["angle"].GetFloat(), allocator);
                        else
                            image.AddMember("angle", 0.0, allocator);

                        
                        imageArray.PushBack(image, allocator);
                    }
                }
                
                platform.AddMember("image", imageArray, allocator);
                platformArray.PushBack(platform, allocator);
            }
        }
        // FIRST PASS END
        
        
        // SECOND PASS
        for (int i = 0; i < bodyCount; i++)
        {
            string bodyName = inputDocument["body"][i]["name"].GetString();
            
            if (bodyName.find(STRUCTURE_STRING_LITERAL) != string::npos)
            {
                if (bodyName.find(BALL_STRING_LITERAL) != string::npos)
                {
                    // its a ball body
                    rapidjson::Value ballBody(rapidjson::kObjectType);
                    rapidjson::Value fixtureArray(rapidjson::kArrayType);
                    rapidjson::Value imageArray(rapidjson::kArrayType);
                    
                    ballBody.AddMember("global_index", i, allocator);
                    
                    rapidjson::Value position(rapidjson::kObjectType);
                    position.AddMember("x", inputDocument["body"][i]["position"]["x"].GetFloat(), allocator);
                    position.AddMember("y", inputDocument["body"][i]["position"]["y"].GetFloat(), allocator);
                    ballBody.AddMember("position", position, allocator);
                    ballBody.AddMember("type", inputDocument["body"][i]["type"].GetInt(), allocator);
                    ballBody.AddMember("angle", inputDocument["body"][i]["angle"].GetFloat(), allocator);
                    
                    int fixtureCount = inputDocument["body"][i]["fixture"].Size();
                    for (int j = 0; j < fixtureCount; j++)
                    {
                        fixtureArray.PushBack(inputDocument["body"][i]["fixture"][j], allocator);
                        fixtureArray[j].RemoveMember("name");
                    }
                    ballBody.AddMember("fixture", fixtureArray, allocator);
                    
                    int imageCount = inputDocument["image"].Size();
                    for (int j = 0; j < imageCount; j++)
                    {
                        if (inputDocument["image"][j]["body"].GetInt() == i)
                        {
                            rapidjson::Value image(rapidjson::kObjectType);
                            rapidjson::Value res(getResName(inputDocument["image"][j]["file"].GetString()), allocator);
                            
                            image.AddMember("res", res, allocator);
                            image.AddMember("aspect_scale", inputDocument["image"][j]["aspectScale"].GetFloat(), allocator);
                            image.AddMember("scale", inputDocument["image"][j]["scale"].GetFloat(), allocator);
                            image.AddMember("center", inputDocument["image"][j]["center"], allocator);
                            //                            image.AddMember("corners", inputDocument["image"][j]["corners"], allocator);
                            
                            if (inputDocument["image"][j].HasMember("angle"))
                                image.AddMember("angle", inputDocument["image"][j]["angle"].GetFloat(), allocator);
                            else
                                image.AddMember("angle", 0.0, allocator);
                            
                            
                            imageArray.PushBack(image, allocator);
                        }
                    }
                    
                    ballBody.AddMember("image", imageArray, allocator);
                    
                    ropeStructureArray[getRopeStructureIndex(bodyName)].AddMember("ball", ballBody, allocator);
                }
            }
        }
        // SECOND PASS END
        
        
        // THIRD PASS
        for (int i = 0; i < ropeStructureArray.Size(); i++)
        {
            rapidjson::Value rope(rapidjson::kArrayType);
            rapidjson::Value hinge(rapidjson::kArrayType);
            
            ropeStructureArray[i].AddMember("rope", rope, allocator);
            ropeStructureArray[i].AddMember("hinge", hinge, allocator);

            for (int j = 0; j < 2; j++)
            {
                rapidjson::Value hinge(rapidjson::kObjectType);
                ropeStructureArray[i]["hinge"].PushBack(hinge, allocator);
            }
        }
        
        // for rope 0
        for (int i = 0; i < ropeStructureArray.Size(); i++)
        {
            rapidjson::Value ropeArray(rapidjson::kArrayType);
            rapidjson::Value rope(rapidjson::kObjectType);
            
            for (int j = 0; j < bodyCount; j++)
            {
                string bodyName = inputDocument["body"][j]["name"].GetString();
                
                if (bodyName.find(STRUCTURE_STRING_LITERAL) != string::npos)
                {
                    
                    if (bodyName.find(ROPE_STRING_LITERAL) != string::npos)
                    {
                        // find ropestructure index
                        int ropeStructureIndex = getRopeStructureIndex(bodyName);
                        
                        if (ropeStructureIndex == i)
                        {
                            // for rope index = 0
                            if (getRopeIndex(bodyName) == 0)
                            {
                                rapidjson::Value ropeBody(rapidjson::kObjectType);
                                rapidjson::Value fixtureArray(rapidjson::kArrayType);
                                rapidjson::Value imageArray(rapidjson::kArrayType);
                                
                                ropeBody.AddMember("global_index", j, allocator);
                                ropeBody.AddMember("local_index", getRopeBodyIndex(bodyName), allocator);
                                
                                rapidjson::Value position(rapidjson::kObjectType);
                                position.AddMember("x", inputDocument["body"][j]["position"]["x"].GetFloat(), allocator);
                                position.AddMember("y", inputDocument["body"][i]["position"]["y"].GetFloat(), allocator);
                                ropeBody.AddMember("position", position, allocator);
                                
                                ropeBody.AddMember("type", inputDocument["body"][j]["type"].GetInt(), allocator);
                                ropeBody.AddMember("angle", inputDocument["body"][j]["angle"].GetFloat(), allocator);
                                
                                int fixtureCount = inputDocument["body"][j]["fixture"].Size();
                                for (int k = 0; k < fixtureCount; k++)
                                {
                                    fixtureArray.PushBack(inputDocument["body"][j]["fixture"][k], allocator);
                                    fixtureArray[k].RemoveMember("name");
                                }
                                ropeBody.AddMember("fixture", fixtureArray, allocator);
                                
                                for (int k = 0; k < imageCount; k++)
                                {
                                    if (inputDocument["image"][k]["body"].GetInt() == j)
                                    {
                                        rapidjson::Value image(rapidjson::kObjectType);
                                        rapidjson::Value res(getResName(inputDocument["image"][k]["file"].GetString()), allocator);
                                        
                                        image.AddMember("res", res, allocator);
                                        image.AddMember("aspect_scale", inputDocument["image"][k]["aspectScale"].GetFloat(), allocator);
                                        image.AddMember("scale", inputDocument["image"][k]["scale"].GetFloat(), allocator);
                                        image.AddMember("center", inputDocument["image"][k]["center"], allocator);
                                        //                            image.AddMember("corners", inputDocument["image"][j]["corners"], allocator);
                                        
                                        if (inputDocument["image"][k].HasMember("angle"))
                                            image.AddMember("angle", inputDocument["image"][k]["angle"].GetFloat(), allocator);
                                        else
                                            image.AddMember("angle", 0.0, allocator);
                                        
                                        
                                        imageArray.PushBack(image, allocator);
                                    }
                                }
                                
                                ropeBody.AddMember("image", imageArray, allocator);
                                
                                ropeArray.PushBack(ropeBody, allocator);
                                
                            }
                        }
                    }
                }
            }
            
            ropeStructureArray[i]["rope"].PushBack(ropeArray, allocator);
        }
        
        // for rope 1
        for (int i = 0; i < ropeStructureArray.Size(); i++)
        {
            rapidjson::Value ropeArray(rapidjson::kArrayType);
            rapidjson::Value rope(rapidjson::kObjectType);
            
            for (int j = 0; j < bodyCount; j++)
            {
                string bodyName = inputDocument["body"][j]["name"].GetString();
                
                if (bodyName.find(STRUCTURE_STRING_LITERAL) != string::npos)
                {
                    
                    if (bodyName.find(ROPE_STRING_LITERAL) != string::npos)
                    {
                        // find ropestructure index
                        int ropeStructureIndex = getRopeStructureIndex(bodyName);
                        
                        if (ropeStructureIndex == i)
                        {
                            // for rope index = 1
                            if (getRopeIndex(bodyName) == 1)
                            {
                                rapidjson::Value ropeBody(rapidjson::kObjectType);
                                rapidjson::Value fixtureArray(rapidjson::kArrayType);
                                rapidjson::Value imageArray(rapidjson::kArrayType);
                                
                                ropeBody.AddMember("global_index", j, allocator);
                                ropeBody.AddMember("local_index", getRopeBodyIndex(bodyName), allocator);
                                
                                rapidjson::Value position(rapidjson::kObjectType);
                                position.AddMember("x", inputDocument["body"][j]["position"]["x"].GetFloat(), allocator);
                                position.AddMember("y", inputDocument["body"][i]["position"]["y"].GetFloat(), allocator);
                                ropeBody.AddMember("position", position, allocator);
                                
                                ropeBody.AddMember("type", inputDocument["body"][j]["type"].GetInt(), allocator);
                                ropeBody.AddMember("angle", inputDocument["body"][j]["angle"].GetFloat(), allocator);
                                
                                int fixtureCount = inputDocument["body"][j]["fixture"].Size();
                                for (int k = 0; k < fixtureCount; k++)
                                {
                                    fixtureArray.PushBack(inputDocument["body"][j]["fixture"][k], allocator);
                                    fixtureArray[k].RemoveMember("name");
                                }
                                ropeBody.AddMember("fixture", fixtureArray, allocator);
                                
                                for (int k = 0; k < imageCount; k++)
                                {
                                    if (inputDocument["image"][k]["body"].GetInt() == j)
                                    {
                                        rapidjson::Value image(rapidjson::kObjectType);
                                        rapidjson::Value res(getResName(inputDocument["image"][k]["file"].GetString()), allocator);
                                        
                                        image.AddMember("res", res, allocator);
                                        image.AddMember("aspect_scale", inputDocument["image"][k]["aspectScale"].GetFloat(), allocator);
                                        image.AddMember("scale", inputDocument["image"][k]["scale"].GetFloat(), allocator);
                                        image.AddMember("center", inputDocument["image"][k]["center"], allocator);
                                        //                            image.AddMember("corners", inputDocument["image"][j]["corners"], allocator);
                                        
                                        if (inputDocument["image"][k].HasMember("angle"))
                                            image.AddMember("angle", inputDocument["image"][k]["angle"].GetFloat(), allocator);
                                        else
                                            image.AddMember("angle", 0.0, allocator);
                                        
                                        imageArray.PushBack(image, allocator);
                                    }
                                }
                                
                                ropeBody.AddMember("image", imageArray, allocator);
                                
                                ropeArray.PushBack(ropeBody, allocator);
                            }
                        }
                    }
                }
            }
            
            ropeStructureArray[i]["rope"].PushBack(ropeArray, allocator);
        }
        
        // for hinge
        for (int i = 0; i < bodyCount; i++)
        {
            string bodyName = inputDocument["body"][i]["name"].GetString();
            
            if (bodyName.find(HINGE_STRING_LITERAL) != string::npos)
            {
                // add hinge
                rapidjson::Value hingeBody(rapidjson::kObjectType);
                rapidjson::Value fixtureArray(rapidjson::kArrayType);
                rapidjson::Value imageArray(rapidjson::kArrayType);
                
                hingeBody.AddMember("global_index", i, allocator);
                
                rapidjson::Value position(rapidjson::kObjectType);
                position.AddMember("x", inputDocument["body"][i]["position"]["x"].GetFloat(), allocator);
                position.AddMember("y", inputDocument["body"][i]["position"]["y"].GetFloat(), allocator);
                hingeBody.AddMember("position", position, allocator);
                hingeBody.AddMember("type", inputDocument["body"][i]["type"].GetInt(), allocator);
                hingeBody.AddMember("angle", inputDocument["body"][i]["angle"].GetFloat(), allocator);
                
                int fixtureCount = inputDocument["body"][i]["fixture"].Size();
                for (int j = 0; j < fixtureCount; j++)
                {
                    fixtureArray.PushBack(inputDocument["body"][i]["fixture"][j], allocator);
                    fixtureArray[j].RemoveMember("name");
                }
                hingeBody.AddMember("fixture", fixtureArray, allocator);
                
                int imageCount = inputDocument["image"].Size();
                for (int j = 0; j < imageCount; j++)
                {
                    if (inputDocument["image"][j]["body"].GetInt() == i)
                    {
                        rapidjson::Value image(rapidjson::kObjectType);
                        rapidjson::Value res(getResName(inputDocument["image"][j]["file"].GetString()), allocator);
                        
                        image.AddMember("res", res, allocator);
                        image.AddMember("aspect_scale", inputDocument["image"][j]["aspectScale"].GetFloat(), allocator);
                        image.AddMember("scale", inputDocument["image"][j]["scale"].GetFloat(), allocator);
                        image.AddMember("center", inputDocument["image"][j]["center"], allocator);
                        //                            image.AddMember("corners", inputDocument["image"][j]["corners"], allocator);
                        
                        if (inputDocument["image"][j].HasMember("angle"))
                            image.AddMember("angle", inputDocument["image"][j]["angle"].GetFloat(), allocator);
                        else
                            image.AddMember("angle", 0.0, allocator);
                        
                        imageArray.PushBack(image, allocator);
                    }
                }
                
                hingeBody.AddMember("image", imageArray, allocator);
                
                ropeStructureArray[getRopeStructureIndex(bodyName)]["hinge"][getHingeIndex(bodyName)] = hingeBody;
            }
        }
        // THIRD PASS END
        
        // FOURTH PASS
        for (int i = 0; i < ropeStructureArray.Size(); i++)
        {
            for (int j = 0; j < ropeStructureArray[i]["rope"].Size(); j++)
            {
                for (int k = 0; k < ropeStructureArray[i]["rope"][j].Size(); )
                {
                    int swapperIndex = ropeStructureArray[i]["rope"][j][k]["local_index"].GetInt();
                    
                    if (k == swapperIndex)
                    {
                        k++;
                    }
                    else
                    {
                        rapidjson::Value swapper(rapidjson::kObjectType);
                        
                        swapper = ropeStructureArray[i]["rope"][j][k];
                        ropeStructureArray[i]["rope"][j][k] = ropeStructureArray[i]["rope"][j][swapperIndex];
                        ropeStructureArray[i]["rope"][j][swapperIndex] = swapper;
                    }
                }
                
            }
        }
        // FOURTH PASS END
        
        // FIFTH PASS
        for (int i = 0; i < inputDocument["joint"].Size(); i++)
        {
            rapidjson::Value nameA(inputDocument["body"][inputDocument["joint"][i]["bodyA"].GetInt()]["name"].GetString(), allocator);
            rapidjson::Value nameB(inputDocument["body"][inputDocument["joint"][i]["bodyB"].GetInt()]["name"].GetString(), allocator);
            
            inputDocument["joint"][i].AddMember("nameA", nameA, allocator);
            inputDocument["joint"][i].AddMember("nameB", nameB, allocator);
        }
        // FIFTH PASS END
        
        
        rapidjson::Value fixtureArray(rapidjson::kArrayType);
        
        outputDocument.AddMember("rope_structure", ropeStructureArray, allocator);
        outputDocument.AddMember("platform", platformArray, allocator);
        outputDocument.AddMember("block", blockArray, allocator);
        outputDocument.AddMember("joint", inputDocument["joint"], allocator);
        
        // writing to target file
        rapidjson::StringBuffer strbuf;
        rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
        outputDocument.Accept(writer);
        
        ofstream outputFile;
        outputFile.open(target_path);
        
        outputFile << strbuf.GetString();
        
        outputFile.close();
        
        return true;
    }
    
    return false;
}

int JsonConverter::getRopeStructureIndex(std::string bodyName)
{
    char ropeStructureIndex[3];
    int chItr;
    int chItr2;
    
    chItr = ((int)STRUCTURE_STRING_LITERAL.length()) + 1;
    chItr2 = 0;
    for (; bodyName[chItr] != '_'; chItr++)
    {
        ropeStructureIndex[chItr2++] = bodyName[chItr];
    }
    
    return atoi(ropeStructureIndex);
}

int JsonConverter::getRopeIndex(std::string bodyName)
{
    char ropeStructureIndex[3];
    int chItr;
    int chItr2;
    
    chItr = ((int)STRUCTURE_STRING_LITERAL.length()) + 1;
    chItr2 = 0;
    for (; bodyName[chItr] != '_'; chItr++)
    {
        ropeStructureIndex[chItr2++] = bodyName[chItr];
    }
    
    char ropeIndex[3];
    chItr += ((int)ROPE_STRING_LITERAL.length()) + 2;
    chItr2 = 0;
    
    for (; bodyName[chItr] != '_'; chItr++)
    {
        ropeIndex[chItr2++] = bodyName[chItr];
    }
    
    return atoi(ropeIndex);
}

int JsonConverter::getRopeBodyIndex(std::string bodyName)
{
    // find ropestructure index
    char ropeStructureIndex[3];
    int chItr;
    int chItr2;
    
    chItr = ((int)STRUCTURE_STRING_LITERAL.length()) + 1;
    chItr2 = 0;
    for (; bodyName[chItr] != '_'; chItr++)
    {
        ropeStructureIndex[chItr2++] = bodyName[chItr];
    }
    
    // find rope index
    char ropeIndex[3];
    chItr += ((int)ROPE_STRING_LITERAL.length()) + 2;
    chItr2 = 0;
    
    for (; bodyName[chItr] != '_'; chItr++)
    {
        ropeIndex[chItr2++] = bodyName[chItr];
    }
    
    // find rope body index
    char ropeBodyIndex[3];
    chItr++;
    chItr2 = 0;
    
    for (; chItr < (int)bodyName.length(); chItr++)
    {
        ropeBodyIndex[chItr2++] = bodyName[chItr];
    }
    
    return atoi(ropeBodyIndex);
}

int JsonConverter::getHingeIndex(std::string bodyName)
{
    // find ropestructure index
    char ropeStructureIndex[3];
    int chItr;
    int chItr2;
    
    chItr = (int)STRUCTURE_STRING_LITERAL.length()+1;
    chItr2 = 0;
    for (; bodyName[chItr] != '_'; chItr++)
    {
        ropeStructureIndex[chItr2++] = bodyName[chItr];
    }
    
    // find hinge index
    char hingeIndex[3];
    
    chItr += HINGE_STRING_LITERAL.length()+2;
    chItr2 = 0;
    
    for (; chItr < (int)bodyName.length(); chItr++)
    {
        hingeIndex[chItr2++] = bodyName[chItr];
    }
    
    return atoi(hingeIndex);
}

char * JsonConverter::getResName(std::string resPath)
{
    char *resName;
    int i = ((int)resPath.length())-1;
    int j = 0;
    
    for (; resPath[i] != '/'; i--);
    
    resName = new char[((int)resPath.length())-i];
    
    for (i++; i < ((int)resPath.length()); i++)
        resName[j++] = resPath[i];
    
    resName[j] = '\0';
    
    return resName;
}
