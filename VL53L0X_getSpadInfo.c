#define VL53L0X_DEV_ADD 0x29

int getSpadInfo(uint8_t* count, uint8_t* type_is_aperture)
{
    unsigned char tmp[1]; // holds data from register
    HAL_StatusTypeDef i2cStats;

    i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0x80, I2C_MEMADD_SIZE_8BIT, 0x01, 1, 100);
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0xFF, I2C_MEMADD_SIZE_8BIT, 0x01, 1, 100);
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0x00, I2C_MEMADD_SIZE_8BIT, 0x00, 1, 100);

    i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0xFF, I2C_MEMADD_SIZE_8BIT, 0x06, 1, 100);

    i2cStats = HAL_I2C_Mem_Read(&hi2c1, VL53L0X_DEV_ADD << 1, 0x83, I2C_MEMADD_SIZE_8BIT, tmp, 1, 100);
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0x83, I2C_MEMADD_SIZE_8BIT, tmp[0] | 0x04, 1, 100);

    i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0xFF, I2C_MEMADD_SIZE_8BIT, 0x07, 1, 100);
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0x81, I2C_MEMADD_SIZE_8BIT, 0x01, 1, 100);
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0x80, I2C_MEMADD_SIZE_8BIT, 0x01, 1, 100);
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0x94, I2C_MEMADD_SIZE_8BIT, 0x6b, 1, 100);
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0x83, I2C_MEMADD_SIZE_8BIT, 0x00, 1, 100);

    do {
        i2cStats = HAL_I2C_Mem_Read(&hi2c1, VL53L0X_DEV_ADD << 1, 0x83, I2C_MEMADD_SIZE_8BIT, tmp, 1, 100);
    } while (tmp[0] == 0x00)

    i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0x83, I2C_MEMADD_SIZE_8BIT, 0x01, 1, 100);

    i2cStats = HAL_I2C_Mem_Read(&hi2c1, VL53L0X_DEV_ADD << 1, 0x92, I2C_MEMADD_SIZE_8BIT, tmp, 1, 100);

    *count = tmp[0] & 0x7f;
    *type_is_aperture = (tmp[0] >> 7) & 0x01;

    i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0x81, I2C_MEMADD_SIZE_8BIT, 0x00, 1, 100);
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0xFF, I2C_MEMADD_SIZE_8BIT, 0x06, 1, 100);

    i2cStats = HAL_I2C_Mem_Read(&hi2c1, VL53L0X_DEV_ADD << 1, 0x83, I2C_MEMADD_SIZE_8BIT, tmp, 1, 100);
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0x83, I2C_MEMADD_SIZE_8BIT, tmp[0] & ~0x04, 1, 100);

    i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0xFF, I2C_MEMADD_SIZE_8BIT, 0x01, 1, 100);
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0x00, I2C_MEMADD_SIZE_8BIT, 0x01, 1, 100);
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0xFF, I2C_MEMADD_SIZE_8BIT, 0x00, 1, 100);
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0x80, I2C_MEMADD_SIZE_8BIT, 0x00, 1, 100);

    return 1;
}