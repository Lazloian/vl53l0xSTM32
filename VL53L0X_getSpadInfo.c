#define VL53L0X_DEV_ADD 0x29

int getSpadInfo(uint8_t* count, uint8_t* type_is_aperture)
{
    unsigned char tmp[1]; // holds data from register
    HAL_StatusTypeDef i2cStats;

    i2cStats = VL53L0X_Write_Value(0x80, 0x01);
    i2cStats = VL53L0X_Write_Value(0xFF, 0x01);
    i2cStats = VL53L0X_Write_Value(0x00, 0x00);
    i2cStats = VL53L0X_Write_Value(0xFF, 0x06);

    i2cStats = HAL_I2C_Mem_Read(&hi2c1, VL53L0X_DEV_ADD << 1, 0x83, I2C_MEMADD_SIZE_8BIT, tmp, 1, 100);
    i2cStats = VL53L0X_Write_Value(0x83, tmp[0] | 0x04);

    i2cStats = VL53L0X_Write_Value(0xFF, 0x07);
    i2cStats = VL53L0X_Write_Value(0x81, 0x01);
    i2cStats = VL53L0X_Write_Value(0x80, 0x01);
    i2cStats = VL53L0X_Write_Value(0x94, 0x6b);
    i2cStats = VL53L0X_Write_Value(0x83, 0x00);

    do {
        i2cStats = HAL_I2C_Mem_Read(&hi2c1, VL53L0X_DEV_ADD << 1, 0x83, I2C_MEMADD_SIZE_8BIT, tmp, 1, 100);
    } while (tmp[0] == 0x00)

    i2cStats = VL53L0X_Write_Value(0x83, 0x01);

    i2cStats = HAL_I2C_Mem_Read(&hi2c1, VL53L0X_DEV_ADD << 1, 0x92, I2C_MEMADD_SIZE_8BIT, tmp, 1, 100);

    *count = tmp[0] & 0x7f;
    *type_is_aperture = (tmp[0] >> 7) & 0x01;

    i2cStats = VL53L0X_Write_Value(0x81, 0x00);
    i2cStats = VL53L0X_Write_Value(0xFF, 0x06);

    i2cStats = HAL_I2C_Mem_Read(&hi2c1, VL53L0X_DEV_ADD << 1, 0x83, I2C_MEMADD_SIZE_8BIT, tmp, 1, 100);
    i2cStats = VL53L0X_Write_Value(0x83, tmp[0] & ~0x04);

    i2cStats = VL53L0X_Write_Value(0xFF, 0x01);
    i2cStats = VL53L0X_Write_Value(0x00, 0x01);
    i2cStats = VL53L0X_Write_Value(0xFF, 0x00);
    i2cStats = VL53L0X_Write_Value(0x80, 0x00);

    return 1;
}