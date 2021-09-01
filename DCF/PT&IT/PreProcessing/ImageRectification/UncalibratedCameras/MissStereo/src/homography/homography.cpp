/* Apply homography to image with anti-aliasing filter.
    Copyright (C) 2010 Pascal Monasse <monasse@imagine.enpc.fr>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "libTransform/map_image.h"
#include "libNumerics/homography.h"
#include "libIO/io_png.h"
#include "libIO/io_tiff.h"
#include "libIO/nan.h"

#include <fstream>
#include <iostream>
#include <cstdlib>

/// Usage: homography image_in H image_out [tiff32_out]|
/// Apply homography given by \f$3\times3\f$ matrix in file @H (Matlab text
/// format) to @image_in (PNG) and output in @image_out (PNG format). If
/// @tiff32_out is given, it outputs the same image but with no quantization as
/// a TIFF float gray-level image. Pixels outside image have value NaN (Not a
/// number).
int main(int argc, char** argv)
{
    if(argc != 4 && argc != 5) {
        std::cerr << "Usage: " <<argv[0] << " image_in H image_out [tiff32_out]"
                  << std::endl;
        return 1;
    }

    size_t nx, ny;
    LWImage<float> in(0,0,0);
    in.data = read_png_f32_rgb(argv[1], &nx, &ny);
    in.w = static_cast<int>(nx); in.h = static_cast<int>(ny); in.comps=3;
    if(! in.data) {
        std::cerr << "Error reading image " << argv[1] << std::endl;
        return 1;
    }

    libNumerics::Homography H;
    std::ifstream f(argv[2]);
    if((f >> H.mat()).fail()) {
        std::cerr << "Error reading homography file " << argv[2] << std::endl;
        return 1;
    }

    LWImage<float> out = alloc_image<float>(in.w, in.h, in.comps);
    map_image(in, H, out, 5, false, true);

    // Write only red channel in float
    if(argc >= 5 &&
       write_tiff_f32(argv[4], out.data, out.w, out.h, 1/*out.comps*/) != 0) {
        std::cerr << "Error writing file " << argv[4] << std::endl;
        return 1;
    }

    // Put in white invalid pixels
    for(int i=out.comps*out.w*out.h-1; i>=0; i--)
        if(! is_number(out.data[i]))
            out.data[i] = 255.0f;
    if(write_png_f32(argv[3], out.data, out.w, out.h, out.comps) != 0) {
        std::cerr << "Error writing file " << argv[3] << std::endl;
        return 1;
    }

    free(in.data);
    free(out.data);

    return 0;
}
