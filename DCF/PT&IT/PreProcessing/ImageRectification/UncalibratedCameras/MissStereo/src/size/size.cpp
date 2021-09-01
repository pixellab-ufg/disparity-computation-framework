/* Display dimensions of PNG image.
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

#include "libIO/io_png.h"
#include <iostream>

/// Usage: size image.png
/// Return the dimensions in pixels of file image.png (PNG format)
int main(int argc, char** argv)
{
    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " image.png" << std::endl;
        return 1;
    }

    size_t w,h,c;
    unsigned char* data = read_png_u8(argv[1], &w,&h,&c);

    if(! data) {
        std::cerr << "Error reading image " << argv[1] << std::endl;
        return 1;
    }
    std::cout << w << " " << h << std::endl;

    return 0;
}
