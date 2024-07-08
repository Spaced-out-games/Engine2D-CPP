#pragma once

union Number
{
    int asInt;
    float asFloat;
};

struct floatPair
{
    float x;
    float y;
    floatPair(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
};

struct intPair
{
    int x;
    int y;
    intPair(int x = 0, int y = 0) : x(x), y(y) {}
};

// A number that can function as both an integer and a float, depending on how you ask
struct pair2D
{
    pair2D() : x({ 0 }), y({ 0 }), representation(1) {} // Initialize to (0,0) with float representation
    pair2D(int x, int y);
    pair2D(float x, float y);
    ~pair2D() = default;

    // Casts to an int (3.14 -> 3). This changes the representation
    void to_int();

    // Casts to a float (0 -> 0.0). This changes the representation
    void to_float();

    // Gets the abstract number representation as a float pair. This means returning the floats if in float representation,
    // and casting to floats if in int representation
    floatPair as_float() const;

    // Gets the abstract number representation as an int pair. This means returning the ints if in int representation,
    // and casting to ints if in float representation
    intPair as_int() const;

    // If this is a pixel coordinate, this normalizes it to a 0:1 range, given a window size
    floatPair NormalSpace(int width, int height) const;

    // Converts normalized coordinates to pixel coordinates based on the given window size
    intPair PixelSpace(int width, int height) const;

    void setPosition(int x, int y);
    void setPosition(float x, float y);

private:
    Number x;
    Number y;
    bool representation; // 0 for int, 1 for float
};

struct rect
{
    pair2D location;
    pair2D dimensions;
    rect(int x, int y, int w, int h) : location(x, y), dimensions(w, h) {}
    rect(float x, float y, float w, float h) : location(x, y), dimensions(w, h) {}
};

pair2D::pair2D(int x, int y) : representation(0)
{
    this->x.asInt = x;
    this->y.asInt = y;
}

pair2D::pair2D(float x, float y) : representation(1)
{
    this->x.asFloat = x;
    this->y.asFloat = y;
}

void pair2D::to_int()
{
    if (representation)
    {
        x.asInt = static_cast<int>(x.asFloat);
        y.asInt = static_cast<int>(y.asFloat);
        representation = 0;
    }
}

void pair2D::to_float()
{
    if (!representation)
    {
        x.asFloat = static_cast<float>(x.asInt);
        y.asFloat = static_cast<float>(y.asInt);
        representation = 1;
    }
}

floatPair pair2D::as_float() const
{
    if (representation)
    {
        return floatPair(x.asFloat, y.asFloat);
    }
    return floatPair(static_cast<float>(x.asInt), static_cast<float>(y.asInt));
}

intPair pair2D::as_int() const
{
    if (representation)
    {
        return intPair(static_cast<int>(x.asFloat), static_cast<int>(y.asFloat));
    }
    return intPair(x.asInt, y.asInt);
}

floatPair pair2D::NormalSpace(int width, int height) const
{
    float normalizedX = representation ? x.asFloat / static_cast<float>(width) : static_cast<float>(x.asInt) / static_cast<float>(width);
    float normalizedY = representation ? y.asFloat / static_cast<float>(height) : static_cast<float>(y.asInt) / static_cast<float>(height);
    return floatPair(normalizedX, normalizedY);
}

intPair pair2D::PixelSpace(int width, int height) const
{
    int pixelX = representation ? static_cast<int>(x.asFloat * width) : x.asInt * width;
    int pixelY = representation ? static_cast<int>(y.asFloat * height) : y.asInt * height;
    return intPair(pixelX, pixelY);
}

void pair2D::setPosition(int x, int y)
{
    this->x.asInt = x;
    this->y.asInt = y;
    representation = 0;
}

void pair2D::setPosition(float x, float y)
{
    this->x.asFloat = x;
    this->y.asFloat = y;
    representation = 1;
}