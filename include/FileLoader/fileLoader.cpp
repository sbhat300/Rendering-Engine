#include "FileLoader/fileLoader.h"
#include <iostream>

std::string fileLoader::loadShader(const char* name, bool windows)
{
    if(windows) return rootPath + "\\shaders\\" + name + ".glsl";
    else return rootPath + "/shaders/" + name + ".glsl"; 
}
std::string fileLoader::loadImg(const char* name, bool windows, const char* fileType)
{
    if(windows) return rootPath + "\\Textures\\" + name + "." + fileType;
    else return rootPath + "/Textures/" + name + "." + fileType; 
}
std::string fileLoader::loadSkybox(const char* name, bool windows, const char* fileType)
{
    if(windows) return rootPath + "\\Textures\\Skybox\\" + name + "." + fileType;
    else return rootPath + "/Textures/Skybox/" + name + "." + fileType; 
}