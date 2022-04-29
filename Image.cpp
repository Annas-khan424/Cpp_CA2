
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include "Image.h"
#include <cmath>



bool Image::load(string filename)
{
    std::ifstream ifs;
    ifs.open(filename, std::ios::binary);
    try {
        if (ifs.fail()) {
            throw("Can't open input file");
        }
        std::string header;
        int w, h, b;
        ifs >> header;
        if (strcmp(header.c_str(), "P6") != 0) throw("Can't read input file");
        ifs >> w >> h >> b;
        this->w = w;
        this->h = h;
        this->pixels = new Rgb[w * h]; // this is throw an exception if bad_alloc
        ifs.ignore(256, '\n'); // skip empty lines in necessary until we get to the binary data
        unsigned char pix[3]; // read each pixel one by one and convert bytes to floats
        for (int i = 0; i < w * h; ++i) {
            ifs.read(reinterpret_cast<char *>(pix), 3);
            this->pixels[i].r = pix[0];
            this->pixels[i].g = pix[1];
            this->pixels[i].b = pix[2];
        }
        ifs.close();
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ifs.close();
        return false;
    }
    return true;


}
bool Image::loadRaw(string filename)
{
    ifstream in(filename);
    if(in.good())
    {
        in >> w;
        in >> h;

        for(int i = 0; i < w*h; i++)
        {
            float r, g, b;
            in >> r >>g>>b;
            this->pixels[i].r = (unsigned char)(std::max(0.f, std::min(255.f, powf(r, 1/2.2) * 255 + 0.5f)));
            this->pixels[i].g = (unsigned char)(std::max(0.f, std::min(255.f, powf(g, 1/2.2) * 255 + 0.5f)));
            this->pixels[i].b = (unsigned char)(std::max(0.f, std::min(255.f, powf(b, 1/2.2) * 255 + 0.5f)));
        }
        in.close();
        return true;
    }
    return false;

}


bool Image::savePPM(string filename)
{
    if (this->w == 0 || this->h == 0) { fprintf(stderr, "Can't save an empty image\n"); return false; }
    std::ofstream ofs;
    try {
        ofs.open(filename, std::ios::binary); // need to spec. binary mode for Windows users
        if (ofs.fail())
        {
            throw("Can't open output file");
            return false;
        }
        ofs << "P6\n" << this->w << " " << this->h << "\n255\n";
        unsigned char r, g, b;
        // loop over each pixel in the image, clamp and convert to byte format
        for (int i = 0; i < this->w * this->h; ++i) {
            r = this->pixels[i].r;
            g = this->pixels[i].g;
            b = this->pixels[i].b;
            ofs << r << g << b;
        }
        ofs.close();
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ofs.close();
        return false;
    }
    return true;
}


void Image::filterRed()
{
    for(int i = 0; i < h*w; i++)
    {
        this->pixels[i].g = 0;
        this->pixels[i].b = 0;
    }
}
void Image::filterGreen()
{
    for(int i = 0; i < h*w; i++)
    {
        this->pixels[i].r = 0;
        this->pixels[i].b = 0;
    }

}
void Image::filterBlue()
{
    for(int i = 0; i < h*w; i++)
    {
        this->pixels[i].r = 0;
        this->pixels[i].g = 0;
    }

}
void Image::greyScale()
{
    for(int i = 0; i < h*w; i++)
    {
        int r = this->pixels[i].r;
        int g = this->pixels[i].g;
        int b = this->pixels[i].b;

        double greyed_unrounded = (double)(r + g + b) / 3;
        int greyed_rounded = round(greyed_unrounded);

        this->pixels[i].r = greyed_rounded;
        this->pixels[i].g = greyed_rounded;
        this->pixels[i].b = greyed_rounded;
    }
}
void Image::flipHorizontal()
{

    int pixels[3];
    unsigned int temp1;
    unsigned int temp2;
    for(int y= 0; y < h; ++y)
    {
        for(int x =0; x <w/2; ++x)
        {
            temp1 = x + y * w;
            temp2 = (w - 1 - x) + y * w;

            pixels[0] = this->pixels[temp1].r;
            pixels[1] = this->pixels[temp1].g;
            pixels[2] = this->pixels[temp1].b;

            this->pixels[temp1] = this->pixels[temp2];

            this->pixels[temp2].r = pixels[0];
            this->pixels[temp2].g = pixels[1];
            this->pixels[temp2].b = pixels[2];
        }
    }

}
void Image::flipVertically()
{
    int pixels[3];
    unsigned int temp1;
    unsigned int temp2;
    for(int x= 0; x < w; ++x)
    {
        for(int y =0; y <h/2; ++y)
        {
            temp1 = x + y * w;
            temp2 = x + (h - 1 - y) * w;

            pixels[0] = this->pixels[temp1].r;
            pixels[1] = this->pixels[temp1].g;
            pixels[2] = this->pixels[temp1].b;

            this->pixels[temp1] = this->pixels[temp2];

            this->pixels[temp2].r = pixels[0];
            this->pixels[temp2].g = pixels[1];
            this->pixels[temp2].b = pixels[2];
        }
    }


}
void Image::AdditionalFunction2()
//rotate by 90 degree
{
    Image *rotImage = new Image(h,w);

    for(int x = 0;  x< w;x++)    //x axis
    {
        for(int y = 0; y < h;y++)  //y axis
        {
            int offset = h * x + y;
            rotImage->pixels[offset] = this->pixels[w * (h - 1 - y) + x];
        }
    }
    swap(w,h);
    delete[] this->pixels;
    this->pixels = rotImage->pixels;
    rotImage = nullptr;
}
void Image::AdditionalFunction3()
{
    // negative
    for (int i = 0; i < this->w * this->h; i++)
    {
        this->pixels[i].r = 255 - this->pixels[i].r;
        this->pixels[i].g = 255 - this->pixels[i].g;
        this->pixels[i].b = 255 - this->pixels[i].b;
    }
}
void Image::AdditionalFunction1() {
//  increase the image contrast to gray with 9 level of shades

    for (int i = 0; i < h * w; i++) {
        int r = this->pixels[i].r;
        int g = this->pixels[i].g;
        int b = this->pixels[i].b;

        double grey_unrounded = (double) (r + g + b) / 4;
        int grey_rounded = round(grey_unrounded);


        if (grey_rounded > 223) {
            this->pixels[i].r = 255;
            this->pixels[i].g = 255;
            this->pixels[i].b = 255;
        } else if (grey_rounded > 190) {
            this->pixels[i].r = 223;
            this->pixels[i].g = 223;
            this->pixels[i].b = 223;
        } else if (grey_rounded > 149) {
            this->pixels[i].r = 190;
            this->pixels[i].g = 190;
            this->pixels[i].b = 190;
        } else if (grey_rounded > 117) {
            this->pixels[i].r = 149;
            this->pixels[i].g = 149;
            this->pixels[i].b = 149;
        } else if (grey_rounded > 85) {
            this->pixels[i].r = 117;
            this->pixels[i].g = 117;
            this->pixels[i].b = 117;
        } else if (grey_rounded > 63) {
            this->pixels[i].r = 85;
            this->pixels[i].g = 85;
            this->pixels[i].b = 85;
        } else if (grey_rounded > 31) {
            this->pixels[i].r = 63;
            this->pixels[i].g = 63;
            this->pixels[i].b = 63;
        }else if (grey_rounded > 3) {
            this->pixels[i].r = 31;
            this->pixels[i].g = 31;
            this->pixels[i].b = 31;
        } else {
            this->pixels[i].r = 3;
            this->pixels[i].g = 3;
            this->pixels[i].b = 3;
        }
    }
}
    void Image::gamma()
    {
        float gamma=1/2.0f;
        for (int i = 0; i < w * h; i++)
        {
            pixels[i].r = pow(pixels[i].r / 255.0f, gamma) * 255;
            pixels[i].g = pow(pixels[i].g / 255.0f, gamma) * 255;
            pixels[i].b = pow(pixels[i].b / 255.0f, gamma) * 255;
        }
    }
 void Image::AdditionalFunction4()
{
    // edge detection by using roberts cross method
    // https://www.youtube.com/watch?v=0lOtl9JmLU4
    // https://homepages.inf.ed.ac.uk/rbf/HIPR2/roberts.htm#:~:text=The%20Roberts%20Cross%20operator%20performs,image%2C%20as%20is%20the%20output.
    for(int x = 0; x < h; ++x)
    {
        for(int y = 0; y < w; ++y)
        {
            int red = 0;
            int green = 0;
            int blue = 0;
            if(x + 1 < h && y + 1 < w)
            {
                red = abs(pixels[(x + 1) * w + (y + 1)].r - pixels[(x + 1) * w + (y)].r) +
                      abs(pixels[(x + 1) * w + (y + 1)].r - pixels[(x) * w + (y + 1)].r);

                green = abs(pixels[(x + 1) * w + (y + 1)].g - pixels[(x + 1) * w + (y)].g) +
                        abs(pixels[(x + 1) * w + (y + 1)].g - pixels[(x) * w + (y + 1)].g);

                blue = abs(pixels[(x + 1) * w + (y + 1)].b - pixels[(x + 1) * w + (y)].b) +
                       abs(pixels[(x + 1) * w + (y + 1)].b - pixels[(x) * w + (y + 1)].b);
            }
            pixels[x * w + y].r = red;
            pixels[x * w + y].g = green;
            pixels[x * w + y].b = blue;
        }
    }

}
/* Functions used by the GUI - DO NOT MODIFY */
int Image::getWidth()
{
    return w;
}

int Image::getHeight()
{
    return h;
}

Rgb* Image::getImage()
{
    return pixels;
}