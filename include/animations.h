#ifndef _ANIMATIONS_H_
#define _ANIMATIONS_H_

#include <vector>
#include <cmath>
#include <glm/vec3.hpp>   // glm::vec3
#include "mesh.h"

namespace Anim {
    struct Rot {
        Mesh*   target;
        float   speedDegSec;
    };
    struct Trans {
        Mesh*   target;
        glm::vec3   speedPerSec;
    };
    struct Scale {
        Mesh*   target;
        glm::vec3   speedPerSec;
    };

    // C++17 inline variables: one shared instance
    inline std::vector<Rot>      rotations;
    inline std::vector<Trans>    translations;
    inline std::vector<Scale>    scalings;

    inline void addRotate(Mesh* m, float speedDegPerSec) {
        rotations.push_back({m, speedDegPerSec});
    }
    inline void addTranslate(Mesh* m, const glm::vec3 &spd) {
        translations.push_back({m, spd});
    }
    inline void addScale(Mesh* m, const glm::vec3 &spd) {
        scalings.push_back({m, spd});
    }

    inline void update(float dt) {
        // rotations
        for (auto &r : rotations) {
            float v = r.target->rot.y + r.speedDegSec * dt;
            r.target->rot.y = std::fmod(v, 360.f);
            if (r.target->rot.y < 0.f) r.target->rot.y += 360.f;
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
