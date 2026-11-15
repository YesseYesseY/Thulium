#pragma once

struct FRotator
{
    float Pitch;
    float Yaw;
    float Roll;

    struct FVector Vector() const;
};
