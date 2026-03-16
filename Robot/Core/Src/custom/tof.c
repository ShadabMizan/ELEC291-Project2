#include "tof.h"
#include "main.h"
#include "i2c.h"
#include "vl53l0x.h"

#define VL53L0X_ADDR        0x52

void InitTOF(void) {
    vl53l0x_init();
}

int GetRange_mm(void) {
    uint16_t range = 0;
    if (vl53l0x_read_range_single(&range)) {
        if (range == VL53L0X_OUT_OF_RANGE) {
            return -2;    
        } else {
            return range;
        }
    } else {
        return -1;
    }
}

// Used by vl53l0x.c
unsigned char i2c_read_addr8_data8(unsigned char address, unsigned char * value) {
    HAL_I2C_Mem_Read(&hi2c1, VL53L0X_ADDR, address, I2C_MEMADD_SIZE_8BIT, value, 1, HAL_MAX_DELAY);
    return 1;
}

unsigned char i2c_read_addr8_data16(unsigned char address, unsigned short * value) {
    uint8_t buf[2];
    HAL_I2C_Mem_Read(&hi2c1, VL53L0X_ADDR, address, I2C_MEMADD_SIZE_8BIT, buf, 2, HAL_MAX_DELAY);

    *value = (buf[0] << 8) | buf[1];  // I2C is sent MSB first, so convert back to little endian
    return 1;
}

unsigned char i2c_write_addr8_data8(unsigned char address, unsigned char value) {
    uint8_t val = value;
    HAL_I2C_Mem_Write(&hi2c1, VL53L0X_ADDR, address, I2C_MEMADD_SIZE_8BIT, &val, 1, HAL_MAX_DELAY);
    return 1;
}