





//I have no clue what this is supposed to do
void performSingleRefCalibration
{
    LSMwrite[0] = 0b01010000; //CTRL1_XL 2g, 400Hz, normal mode
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, LSM6DS33_DEV_ADD << 1, 0x10, I2C_MEMADD_SIZE_8BIT, LSMwrite, 1, 100);
    HAL_Delay(20);
}


//  ReadRangeSingle
//  
// Performs a single-shot range measurement and returns the reading in
// millimeters
// based on VL53L0X_PerformSingleRangingMeasurement()
uint16_t VL53L0X::readRangeSingleMillimeters(void)
{
  writeReg(0x80, 0x01);              //Write 1                
  writeReg(0xFF, 0x01);              //write 2
  writeReg(0x00, 0x00);              //Write 3
  writeReg(0x91, stop_variable);     //Write 4
  writeReg(0x00, 0x01);              //Write 5
  writeReg(0xFF, 0x00);              //Write 6
  writeReg(0x80, 0x00);              //Write 7

  writeReg(SYSRANGE_START, 0x01);    //Write 8

  // "Wait until start bit has been cleared"
  startTimeout();
  while (readReg(SYSRANGE_START) & 0x01)
  {
    if (checkTimeoutExpired())
    {
      did_timeout = true;
      return 65535;
    }
  }

  return readRangeContinuousMillimeters();
}

//    Replacement Function
// Requires stop_variable it is declared in the original header and initialized in the init function
// Requires SYSRANGE_START to be declared
uint16_t readRangeSingleMil (void)
{
    //Write 1
    LSMwrite[0] = 0x01; 
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, LSM6DS33_DEV_ADD << 1, 0x80, I2C_MEMADD_SIZE_8BIT, LSMwrite, 1, 100);
    HAL_Delay(20);

    //Write 2
    LSMwrite[0] = 0x01; 
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, LSM6DS33_DEV_ADD << 1, 0xFF, I2C_MEMADD_SIZE_8BIT, LSMwrite, 1, 100);
    HAL_Delay(20);
    
    //Write 3
    LSMwrite[0] = 0x00; 
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, LSM6DS33_DEV_ADD << 1, 0x00, I2C_MEMADD_SIZE_8BIT, LSMwrite, 1, 100);
    HAL_Delay(20);

    //Write 4
    LSMwrite[0] = stop_variable; 
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, LSM6DS33_DEV_ADD << 1, 0x91, I2C_MEMADD_SIZE_8BIT, LSMwrite, 1, 100);
    HAL_Delay(20);

    //Write 5
    LSMwrite[0] = 0x01; 
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, LSM6DS33_DEV_ADD << 1, 0x00, I2C_MEMADD_SIZE_8BIT, LSMwrite, 1, 100);
    HAL_Delay(20);

    //Write 6
    LSMwrite[0] = 0x00; 
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, LSM6DS33_DEV_ADD << 1, 0xFF, I2C_MEMADD_SIZE_8BIT, LSMwrite, 1, 100);
    HAL_Delay(20);

    //Write 7
    LSMwrite[0] = 0x00; 
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, LSM6DS33_DEV_ADD << 1, 0x80, I2C_MEMADD_SIZE_8BIT, LSMwrite, 1, 100);
    HAL_Delay(20);

    //Write 8
    LSMwrite[0] = 0x01; 
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, LSM6DS33_DEV_ADD << 1, SYSRANGE_START, I2C_MEMADD_SIZE_8BIT, LSMwrite, 1, 100);
    HAL_Delay(20);
}