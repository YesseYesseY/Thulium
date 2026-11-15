#pragma once

struct FVector
{
    float X;
    float Y;
    float Z;

    FVector() : X(0), Y(0), Z(0) {}
    FVector(float x, float y, float z) : X(x), Y(y), Z(z) {}

    inline FVector operator-(const FVector& O)
    {
        return FVector(X - O.X, Y - O.Y, Z - O.Z);
    }

    inline FVector operator+(const FVector& A)
    {
        return FVector(X + A.X, Y + A.Y, Z + A.Z);
    }

    inline FVector operator*(const FVector& A)
    {
        return FVector(X * A.X, Y * A.Y, Z * A.Z);
    }

    inline FVector operator/(const FVector& A)
    {
        return FVector(X / A.X, Y / A.Y, Z / A.Z);
    }

    inline FVector operator*(float A)
    {
        return FVector(X * A, Y * A, Z * A);
    }
};
