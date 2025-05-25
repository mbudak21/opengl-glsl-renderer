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
        int resetFrame;
    };
    struct Trans {
        SceneObject*   target;
        glm::vec3   pos;
        glm::vec3   speedPerFrame;
        int resetFrame;
    };
    struct Scale {
        SceneObject*   target;
        glm::vec3   scale;
        glm::vec3   speedPerFrame;
        int resetFrame;
    };

    // C++17 inline variables: one shared instance
    inline std::vector<Rot>      rotations;
    inline std::vector<Trans>    translations;
    inline std::vector<Scale>    scalings;

    inline void addRotate(SceneObject* m, const glm::vec3 &spd, int resetframe = INT_MAX) {
        rotations.push_back({m, spd, resetframe});
    }
    inline void addTranslate(SceneObject* m, const glm::vec3 &spd, int resetframe = INT_MAX) {
        translations.push_back({m, m->pos, spd, resetframe});
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
            t.target->pos = t.pos + t.speedPerFrame * static_cast<float>(frameCount % t.resetFrame);
        }
        // scalings
        for (auto &s : scalings) {
            s.target->scale += s.scale + s.speedPerFrame * static_cast<float>(frameCount % s.resetFrame);
        }
    }

}

#endif
