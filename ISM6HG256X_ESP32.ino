/******************************************************************************
 *
 *  ISM6HG256X ESP32 Example
 *  Using Official ST Driver
 *
 ******************************************************************************/

#include <Wire.h>

extern "C"
{
#include "ism6hg256x_reg.h"
}

/************ I2C Configuration ************/
#define SDA_PIN        48 
#define SCL_PIN        46

#define IMU_ADDRESS     0x6B

stmdev_ctx_t dev_ctx;

/************ Function Prototypes ************/
int32_t platform_write(void *handle,
                       uint8_t reg,
                       const uint8_t *bufp,
                       uint16_t len);

int32_t platform_read(void *handle,
                      uint8_t reg,
                      uint8_t *bufp,
                      uint16_t len);

void platform_delay(uint32_t ms);

/************ Setup ************/
void setup()
{
    Serial.begin(115200);

    while(!Serial);

    Serial.println();
    Serial.println("==================================");
    Serial.println("ISM6HG256X Official ST Driver");
    Serial.println("ESP32");
    Serial.println("==================================");

    Wire.begin(SDA_PIN, SCL_PIN);
    Wire.setClock(400000);

    /************ Driver Context ************/
    dev_ctx.write_reg = platform_write;
    dev_ctx.read_reg  = platform_read;
    dev_ctx.mdelay    = platform_delay;
    dev_ctx.handle    = NULL;

    /************ WHO_AM_I ************/
    uint8_t whoamI = 0;

    if (ism6hg256x_device_id_get(&dev_ctx, &whoamI) != 0)
    {
        Serial.println("Communication Failed!");
        while(1);
    }

    Serial.print("WHO_AM_I = 0x");
    Serial.println(whoamI, HEX);

        /************ Reset ************/
        if (ism6hg256x_sh_reset_set(&dev_ctx, PROPERTY_ENABLE) != 0)
        {
                Serial.println("Reset failed!");
                while(1);
        }

        delay(10);

        if (ism6hg256x_sh_reset_set(&dev_ctx, PROPERTY_DISABLE) != 0)
        {
                Serial.println("Reset release failed!");
                while(1);
        }

    /************ Accelerometer ************/
    ism6hg256x_xl_data_rate_set(
            &dev_ctx,
            ISM6HG256X_ODR_HA03_AT_104Hz);

    ism6hg256x_xl_full_scale_set(
            &dev_ctx,
            ISM6HG256X_2g);

    /************ Gyroscope ************/
    ism6hg256x_gy_data_rate_set(
            &dev_ctx,
            ISM6HG256X_ODR_HA03_AT_104Hz);

    ism6hg256x_gy_full_scale_set(
            &dev_ctx,
            ISM6HG256X_250dps);

    Serial.println("Initialization Complete");
}

void loop()
{
    int16_t accel[3];
    int16_t gyro[3];

    ism6hg256x_acceleration_raw_get(
            &dev_ctx,
            accel);

    ism6hg256x_angular_rate_raw_get(
            &dev_ctx,
            gyro);

    Serial.println("---------------------------");

    Serial.print("ACC X : ");
    Serial.println(accel[0]);

    Serial.print("ACC Y : ");
    Serial.println(accel[1]);

    Serial.print("ACC Z : ");
    Serial.println(accel[2]);

    Serial.print("GYR X : ");
    Serial.println(gyro[0]);

    Serial.print("GYR Y : ");
    Serial.println(gyro[1]);

    Serial.print("GYR Z : ");
    Serial.println(gyro[2]);

    delay(500);
}
