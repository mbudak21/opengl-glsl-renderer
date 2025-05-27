#ifndef _LOC_CACHE_H_
#define _LOC_CACHE_H_
#include <unordered_map>
#include <string>
#include <GL/glew.h>

// std::unordered_map<std::string, GLuint> locCache::locMap;
class locCache {
private:
    // locMap[program][name] = location
    static std::unordered_map<GLuint, std::unordered_map<std::string, GLuint>> locMap;

public:
    static GLuint getUniformLoc(GLuint program, const std::string& name) {
        auto& progMap = locMap[program]; // reference to this program's submap
        auto it = progMap.find(name);
        if (it != progMap.end()) return it->second;

        GLuint loc = glGetUniformLocation(program, name.c_str());
        if(loc < 0) {
            printf("WARNING: couldn't find loc of %s", name);
        }
        progMap[name] = loc;
        return loc;
    }
};
#endif