//
// Created by floodd on 23/03/2022.
//
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
{

}
void Image::AdditionalFunction3()
{

}
void Image::AdditionalFunction1()
{

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