#ifndef FILELOADER_H
#define FILELOADER_H
#include <string>

namespace fileLoader
{
    extern std::string rootPath;
    std::string loadShader(const char* name, bool windows);
    std::string loadData(bool windows);
    std::string loadImg(const char* name, bool windows, const char* fileType);
    std::string loadSkybox(const char* name, bool windows, const char* fileType);
};
#endif