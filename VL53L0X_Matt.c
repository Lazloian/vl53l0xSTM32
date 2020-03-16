


//        Original
//
//  based on VL53L0X_perform_single_ref_calibration()
// bool VL53L0X::performSingleRefCalibration(uint8_t vhv_init_byte)
// {
//   writeReg(SYSRANGE_START, 0x01 | vhv_init_byte); // VL53L0X_REG_SYSRANGE_MODE_START_STOP

//   startTimeout();
//   while ((readReg(RESULT_INTERRUPT_STATUS) & 0x07) == 0)
//   {
//     if (checkTimeoutExpired()) { return false; }
//   }

//   writeReg(SYSTEM_INTERRUPT_CLEAR, 0x01);

//   writeReg(SYSRANGE_START, 0x00);

//   return true;
// }



//        Replacement:
void performSingleRefCalibration(uint8_t vhv_init_byte)
{
  i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, SYSRANGE_START, I2C_MEMADD_SIZE_8BIT, 0x01 | vhv_init_byte, 1, 100);
  HAL_Delay(20);

  unsigned char tmp[2];
  do
  {
    i2cStats = HAL_I2C_Mem_Read(&hi2c1, VL53L0X_DEV_ADD << 1, RESULT_INTERRUPT_STATUS, I2C_MEMADD_SIZE_8BIT, tmp, 1, 100);
    tmp[0] &= 0x07;
  } while (tmp[0] == 0);
  
  i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, SYSTEM_INTERRUPT_CLEAR, I2C_MEMADD_SIZE_8BIT, 0x01 | vhv_init_byte, 1, 100);
  HAL_Delay(20);

  i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, SYSRANGE_START, I2C_MEMADD_SIZE_8BIT, 0x00, 1, 100);
  HAL_Delay(20);
}


// //  ReadRangeSingle
// //  
// // Performs a single-shot range measurement and returns the reading in
// // millimeters
// // based on VL53L0X_PerformSingleRangingMeasurement()
// uint16_t VL53L0X::readRangeSingleMillimeters(void)
// {
//   writeReg(0x80, 0x01);              //Write 1                
//   writeReg(0xFF, 0x01);              //write 2
//   writeReg(0x00, 0x00);              //Write 3
//   writeReg(0x91, stop_variable);     //Write 4
//   writeReg(0x00, 0x01);              //Write 5
//   writeReg(0xFF, 0x00);              //Write 6
//   writeReg(0x80, 0x00);              //Write 7

//   writeReg(SYSRANGE_START, 0x01);    //Write 8

//   // "Wait until start bit has been cleared"
//   startTimeout();
//   while (readReg(SYSRANGE_START) & 0x01)
//   {
//     if (checkTimeoutExpired())
//     {
//       did_timeout = true;
//       return 65535;
//     }
//   }

//   return readRangeContinuousMillimeters();
// }

//    Replacement Function
// Requires stop_variable it is declared in the original header and initialized in the init function
// Requires SYSRANGE_START to be declared
uint16_t readRangeSingleMil (void)
{

    //Write 1
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0x80, I2C_MEMADD_SIZE_8BIT, 0x01, 1, 100);
    HAL_Delay(20);

    //Write 2 
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0xFF, I2C_MEMADD_SIZE_8BIT, 0x01, 1, 100);
    HAL_Delay(20);
    
    //Write 3
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0x00, I2C_MEMADD_SIZE_8BIT, 0x00, 1, 100);
    HAL_Delay(20);

    //Write 4
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0x91, I2C_MEMADD_SIZE_8BIT, stop_variable, 1, 100);
    HAL_Delay(20);

    //Write 5
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0x00, I2C_MEMADD_SIZE_8BIT, 0x01, 1, 100);
    HAL_Delay(20);

    //Write 6
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0xFF, I2C_MEMADD_SIZE_8BIT, 0x00, 1, 100);
    HAL_Delay(20);

    //Write 7
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0x80, I2C_MEMADD_SIZE_8BIT, 0x00, 1, 100);
    HAL_Delay(20);

    //Write 8
    i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, SYSRANGE_START, I2C_MEMADD_SIZE_8BIT, 0x01, 1, 100);
    HAL_Delay(20);

    // "Wait until start bit has been cleared"
  unsigned char tmp[2];
  do
  {
    i2cStats = HAL_I2C_Mem_Read(&hi2c1, VL53L0X_DEV_ADD << 1, SYSRANGE_START, I2C_MEMADD_SIZE_8BIT, tmp, 1, 100);
    tmp[0] &= 0x01;
  } while (tmp[0] == 0);

    return readRangeContinuousMil()
}

//      readRangeContinuousMillimeters()
//      
//    Orginal:
// // Returns a range reading in millimeters when continuous mode is active
// // (readRangeSingleMillimeters() also calls this function after starting a
// // single-shot range measurement)
// uint16_t VL53L0X::readRangeContinuousMillimeters(void)
// {
//   startTimeout();
//   while ((readReg(RESULT_INTERRUPT_STATUS) & 0x07) == 0)
//   {
//     if (checkTimeoutExpired())
//     {
//       did_timeout = true;
//       return 65535;
//     }
//   }

//   // assumptions: Linearity Corrective Gain is 1000 (default);
//   // fractional ranging is not enabled
//   uint16_t range = readReg16Bit(RESULT_RANGE_STATUS + 10);

//   writeReg(SYSTEM_INTERRUPT_CLEAR, 0x01);

//   return range;
// }

//           Replacement fucntion
//         ------------------------
//
uint16_t readRangeContinousMil(void)
{
  unsigned char tmp[2]

  do
  {
    i2cStats = HAL_I2C_Mem_Read(&hi2c1, VL53L0X_DEV_ADD << 1, RESULT_INTERRUPT_STATUS, I2C_MEMADD_SIZE_8BIT, tmp, 1, 100);
    tmp[0] &= 0x07;
  } while (tmp[0] == 0);
  
  i2cStats = HAL_I2C_Mem_Read(&hi2c1, VL53L0X_DEV_ADD << 1, RESULT_RANGE_STATUS + 10, I2C_MEMADD_SIZE_8BIT, tmp, 2, 100);

  uint16_t range = (range[0] << 8) | range[1];

  i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, SYSTEM_INTERRUPT_CLEAR, I2C_MEMADD_SIZE_8BIT, 0x01, 1, 100);

  return range; 
}



//    Original
//
// Set the return signal rate limit check value in units of MCPS (mega counts
// per second). "This represents the amplitude of the signal reflected from the
// target and detected by the device"; setting this limit presumably determines
// the minimum measurement necessary for the sensor to report a valid reading.
// Setting a lower limit increases the potential range of the sensor but also
// seems to increase the likelihood of getting an inaccurate reading because of
// unwanted reflections from objects other than the intended target.
// Defaults to 0.25 MCPS as initialized by the ST API and this library.
bool VL53L0X::setSignalRateLimit(float limit_Mcps)
{
  if (limit_Mcps < 0 || limit_Mcps > 511.99) { return false; }

  // Q9.7 fixed point format (9 integer bits, 7 fractional bits)
  writeReg16Bit(FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT, limit_Mcps * (1 << 7));
  return true;
}

//    Replacement
//   -------------
void setSignalRateLimit(float limit_Mcps)
{
  //Error checking not added.  Make sure limit_Mcps is between 0 and 511.99

  //Need to verify how HAL handles multiple bits
  i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT, I2C_MEMADD_SIZE_8BIT, limit_Mcps * (1 << 7), 2, 100);
  HAL_Delay(20);
}