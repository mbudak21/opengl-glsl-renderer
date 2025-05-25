#ifndef _ANIMATIONS_H_
#define _ANIMATIONS_H_

#include <vector>
#include <cmath>
#include <glm/vec3.hpp>   // glm::vec3
#include "sceneObj.h"

namespace Anim {
    struct Rot {
        SceneObject*   target;
        glm::vec3   speedPerSec;
    };
    struct Trans {
        SceneObject*   target;
        glm::vec3   speedPerSec;
    };
    struct Scale {
        SceneObject*   target;
        glm::vec3   speedPerSec;
    };

    // C++17 inline variables: one shared instance
    inline std::vector<Rot>      rotations;
    inline std::vector<Trans>    translations;
    inline std::vector<Scale>    scalings;

    inline void addRotate(SceneObject* m, const glm::vec3 &spd) {
        rotations.push_back({m, spd});
    }
    inline void addTranslate(SceneObject* m, const glm::vec3 &spd) {
        translations.push_back({m, spd});
    }
    inline void addScale(SceneObject* m, const glm::vec3 &spd) {
        scalings.push_back({m, spd});
    }

    inline void update(float dt) {
        // rotations
        for (auto &r : rotations) {
            r.target->rot += r.speedPerSec * dt;
        }
        // translations
        for (auto &t : translations) {
            t.target->pos += t.speedPerSec * dt;
        }
        // scalings
        for (auto &s : scalings) {
            s.target->scale += s.speedPerSec * dt;
        }
    }

}

#endif
