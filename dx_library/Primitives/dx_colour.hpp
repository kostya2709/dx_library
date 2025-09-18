
#ifndef DX_COLOUR
#define DX_COLOUR

#include <inttypes.h>
#include <iostream>

struct dx_colour
{
    uint8_t red   = 0;
    uint8_t green = 0;
    uint8_t blue  = 0;
    uint8_t alpha = 255;

    dx_colour (void) {}

    dx_colour (uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255) : 
    red (red), green (green), blue (blue), alpha (alpha) {}

    dx_colour operator * (const double coef) const noexcept
    {
        return dx_colour (red * coef, green * coef, blue * coef, alpha);
    }

    dx_colour& operator = (const dx_colour& new_colour)
    {
        this->red = new_colour.red;
        this->green = new_colour.green;
        this->blue = new_colour.blue;
        this->alpha = new_colour.alpha;

        return *this;
    }

    friend std::ostream& operator << (std::ostream& ost, const dx_colour& colour)
    {
        ost << "\nRed: " << (int)colour.red << ", green: " << (int)colour.green << ", blue: " << (int)colour.blue;
        if (colour.alpha != 255)
            ost << ", alpha: " << (int)colour.alpha;
        ost << "\n";
        return ost;
    }
};


namespace colour
{
    const dx_colour RED (255, 0, 0);
    const dx_colour GREEN (0, 255, 0);
    const dx_colour BLUE (0, 0, 255);
    const dx_colour LIGHT_BLUE (230, 230, 255);
    const dx_colour SKY_BLUE (200, 223, 255);

    const dx_colour WHITE (255, 255, 255);
    const dx_colour BLACK (0, 0, 0);

    const dx_colour GRAY (128, 128, 128);
    const dx_colour SILVER (192, 192, 192);

    const dx_colour YELLOW (255, 255, 0);
    const dx_colour MAGENTA (255, 0, 255);
    const dx_colour AQUA (0, 255, 255);

    const dx_colour DEFAULT (127, 127, 127);
    const dx_colour TEST (100, 255, 255);

    const dx_colour GREY_BACKGROUND (212, 221, 236);

    const dx_colour BURGUNDY (144, 0, 32);
    const dx_colour CARROT (237, 145, 33);
    const dx_colour DARK_GREEN (23, 114, 69);
};

#endif                  // #define DX_COLOUR