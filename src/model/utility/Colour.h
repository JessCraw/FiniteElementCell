#ifndef COLOUR_H
#define COLOUR_H

struct Colour
{
    Colour(float red, float green, float blue, float alpha)
        : red(red),
          green(green),
          blue(blue),
          alpha(alpha)
    {
    }

    float red, green, blue, alpha;
};

#endif  // COLOUR_H
