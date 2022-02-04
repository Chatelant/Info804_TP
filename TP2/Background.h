#ifndef TP2_BACKGROUND_H
#define TP2_BACKGROUND_H

#include "Color.h"
#include "PointVector.h"

namespace rt {
    struct Background {
        Background() {}

        virtual ~Background() {}

        virtual Color backgroundColor(const Ray &ray) = 0;
    };

    struct MyBackground : public Background {
        Color backgroundColor(const Ray &ray) {
            const double z = ray.direction[2];
            if (z < 0) {
                Color result;
                Real x = -0.5f * ray.direction[0] / ray.direction[2];
                Real y = -0.5f * ray.direction[1] / ray.direction[2];
                Real d = sqrt(x * x + y * y);
                Real t = std::min(d, 30.0f) / 30.0f;
                x -= floor(x);
                y -= floor(y);
                if (((x >= 0.5f) && (y >= 0.5f)) || ((x < 0.5f) && (y < 0.5f)))
                    result += (1.0f - t) * Color(0.2f, 0.2f, 0.2f) + t * Color(1.0f, 1.0f, 1.0f);
                else
                    result += (1.0f - t) * Color(0.4f, 0.4f, 0.4f) + t * Color(1.0f, 1.0f, 1.0f);
                return result;
            } else if (0 < z && z < 0.5) {
                double t = z * 2;
                return ((1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.0, 0.0, 1.0));;
            } else {
                double t = z * 2 - 1;
                return ((1.0 - t) * Color(0.0, 0.0, 1.0) + t * Color(0.0, 0.0, 0.0)); // mix 001 000
            }
        }
    };
}

#endif //TP2_BACKGROUND_H

