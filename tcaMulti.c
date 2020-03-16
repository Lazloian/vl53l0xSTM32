#define TCA_DEV_ADD	0x70

void tcaSelect(unsigned char channels)
{
	unsigned char command[1];
	command[0] = channels;
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, TCA_DEV_ADD << 1, 0x00, I2C_MEMADD_SIZE_8BIT, command, 1, 100);
}

void VL53L0X_Write_Value(unsigned char reg, unsigned char value)
{
	unsigned char command[1];
	command[0] = value;
	i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, reg, I2C_MEMADD_SIZE_8BIT, command, 1, 100);
}