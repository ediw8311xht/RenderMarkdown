#pragma once
#include <iostream>
// #include <opencv2/opencv.hpp>
#include <Magick++.h>
#include <tuple>
#include <vector>
#include <string>

namespace MakeImageNS {
    
    // using namespace std;
    // using namespace cv;
    using _tp_i = std::tuple<int, int>;
    using _s    = std::string;
    namespace Mag = Magick;

    class MakeImage {
        private:
            Mag::Geometry img_geometry;
            Mag::Image img;
        public:
            MakeImage(double x=500, double y=500) : img_geometry(x, y) {}
            void set_image(double r, double g, double b) {
                this->img = Mag::Image(this->img_geometry, Mag::Color(r, g, b));
            }
            void save_image(const _s& filename, const _s& type="jpeg") {
                this->img.magick(type);
                this->img.write(filename);
            }

    };

}
