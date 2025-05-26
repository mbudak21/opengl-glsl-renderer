#ifndef _ANIMATIONS_H_
#define _ANIMATIONS_H_

#include <vector>
#include <cmath>
#include <glm/vec3.hpp>   // glm::vec3
#include "sceneObj.h"

namespace Anim {
    struct Rot {
        SceneObject*   target;
        glm::vec3   speedPerFrame;
        const int resetFrame;
    };
    struct Trans {
        SceneObject* target;
        glm::vec3 startPos;
        glm::vec3 endPos;
        glm::vec3 speedPerFrame;
    };
    struct Scale {
        SceneObject*   target;
        glm::vec3   scale;
        glm::vec3   speedPerFrame;
        const int resetFrame;
    };

    // C++17 inline variables: one shared instance
    inline std::vector<Rot>      rotations;
    inline std::vector<Trans>    translations;
    inline std::vector<Scale>    scalings;

    inline void addRotate(SceneObject* m, const glm::vec3 &spd, int resetframe = INT_MAX) {
        rotations.push_back({m, spd, resetframe});
    }
    inline void addTranslate(SceneObject* m, const glm::vec3 &spd, const glm::vec3 &start, const glm::vec3 &end) {
        translations.push_back({ m, start, end, spd });
    }
    inline void addScale(SceneObject* m, const glm::vec3 &spd, int resetframe = INT_MAX) {
        scalings.push_back({m, m->scale, spd, resetframe});
    }

    inline void update(int frameCount) {
        // rotations
        for (auto &r : rotations) {
            r.target->rot = r.speedPerFrame * static_cast<float>(frameCount % r.resetFrame);
        }
        // translations
        for (auto &t : translations) {
            t.target->pos += t.speedPerFrame;
            for (int i = 0; i < 3; i++) {
                if ((t.startPos[i] < t.endPos[i]) ) { // Moving in +Axis
                    if ((t.target->pos[i] >= t.endPos[i])) {
                        t.target->pos[i] = t.startPos[i];
                    }
                }
            }
        }
        // scalings
        for (auto &s : scalings) {
            s.target->scale += s.scale + s.speedPerFrame * static_cast<float>(frameCount % s.resetFrame);
        }
    }

}

#endif
