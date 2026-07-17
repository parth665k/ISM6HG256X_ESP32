#include <Wire.h>

extern "C"
{
#include "ism6hg256x_reg.h"
}

#define IMU_ADDRESS 0x6A

int32_t platform_write(void *handle,
                       uint8_t reg,
                       const uint8_t *bufp,
                       uint16_t len)
{
    Wire.beginTransmission(IMU_ADDRESS);
    Wire.write(reg);

    while(len--)
        Wire.write(*bufp++);

    return Wire.endTransmission();
}

int32_t platform_read(void *handle,
                      uint8_t reg,
                      uint8_t *bufp,
                      uint16_t len)
{
    Wire.beginTransmission(IMU_ADDRESS);
    Wire.write(reg);

    if(Wire.endTransmission(false))
        return -1;

    Wire.requestFrom(IMU_ADDRESS, len);

    uint16_t i = 0;

    while(Wire.available())
    {
        bufp[i++] = Wire.read();
    }

    return 0;
}

void platform_delay(uint32_t ms)
{
    delay(ms);
}