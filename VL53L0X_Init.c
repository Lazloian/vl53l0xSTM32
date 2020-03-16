#define SYSRANGE_START                               0x00
#define SYSTEM_THRESH_HIGH                           0x0C
#define SYSTEM_THRESH_LOW                            0x0E
#define SYSTEM_SEQUENCE_CONFIG                       0x01
#define SYSTEM_RANGE_CONFIG                          0x09
#define SYSTEM_INTERMEASUREMENT_PERIOD               0x04
#define SYSTEM_INTERRUPT_CONFIG_GPIO                 0x0A
#define GPIO_HV_MUX_ACTIVE_HIGH                      0x84
#define SYSTEM_INTERRUPT_CLEAR                       0x0B
#define RESULT_INTERRUPT_STATUS                      0x13
#define RESULT_RANGE_STATUS                          0x14
#define RESULT_CORE_AMBIENT_WINDOW_EVENTS_RTN        0xBC
#define RESULT_CORE_RANGING_TOTAL_EVENTS_RTN         0xC0
#define RESULT_CORE_AMBIENT_WINDOW_EVENTS_REF        0xD0
#define RESULT_CORE_RANGING_TOTAL_EVENTS_REF         0xD4
#define RESULT_PEAK_SIGNAL_RATE_REF                  0xB6
#define ALGO_PART_TO_PART_RANGE_OFFSET_MM            0x28
#define I2C_SLAVE_DEVICE_ADDRESS                     0x8A
#define MSRC_CONFIG_CONTROL                          0x60
#define PRE_RANGE_CONFIG_MIN_SNR                     0x27
#define PRE_RANGE_CONFIG_VALID_PHASE_LOW             0x56
#define PRE_RANGE_CONFIG_VALID_PHASE_HIGH            0x57
#define PRE_RANGE_MIN_COUNT_RATE_RTN_LIMIT           0x64
#define FINAL_RANGE_CONFIG_MIN_SNR                   0x67
#define FINAL_RANGE_CONFIG_VALID_PHASE_LOW           0x47
#define FINAL_RANGE_CONFIG_VALID_PHASE_HIGH          0x48
#define FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT  0x44
#define PRE_RANGE_CONFIG_SIGMA_THRESH_HI             0x61
#define PRE_RANGE_CONFIG_SIGMA_THRESH_LO             0x62
#define PRE_RANGE_CONFIG_VCSEL_PERIOD                0x50
#define PRE_RANGE_CONFIG_TIMEOUT_MACROP_HI           0x51
#define PRE_RANGE_CONFIG_TIMEOUT_MACROP_LO           0x52
#define SYSTEM_HISTOGRAM_BIN                         0x81
#define HISTOGRAM_CONFIG_INITIAL_PHASE_SELECT        0x33
#define HISTOGRAM_CONFIG_READOUT_CTRL                0x55
#define FINAL_RANGE_CONFIG_VCSEL_PERIOD              0x70
#define FINAL_RANGE_CONFIG_TIMEOUT_MACROP_HI         0x71
#define FINAL_RANGE_CONFIG_TIMEOUT_MACROP_LO         0x72
#define CROSSTALK_COMPENSATION_PEAK_RATE_MCPS        0x20
#define MSRC_CONFIG_TIMEOUT_MACROP                   0x46
#define SOFT_RESET_GO2_SOFT_RESET_N                  0xBF
#define IDENTIFICATION_MODEL_ID                      0xC0
#define IDENTIFICATION_REVISION_ID                   0xC2
#define OSC_CALIBRATE_VAL                            0xF8
#define GLOBAL_CONFIG_VCSEL_WIDTH                    0x32
#define GLOBAL_CONFIG_SPAD_ENABLES_REF_0             0xB0
#define GLOBAL_CONFIG_SPAD_ENABLES_REF_1             0xB1
#define GLOBAL_CONFIG_SPAD_ENABLES_REF_2             0xB2
#define GLOBAL_CONFIG_SPAD_ENABLES_REF_3             0xB3
#define GLOBAL_CONFIG_SPAD_ENABLES_REF_4             0xB4
#define GLOBAL_CONFIG_SPAD_ENABLES_REF_5             0xB5
#define GLOBAL_CONFIG_REF_EN_START_SELECT            0xB6
#define DYNAMIC_SPAD_NUM_REQUESTED_REF_SPAD          0x4E
#define DYNAMIC_SPAD_REF_EN_START_OFFSET             0x4F
#define POWER_MANAGEMENT_GO1_POWER_FORCE             0x80
#define VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV            0x89
#define ALGO_PHASECAL_LIM                            0x30
#define ALGO_PHASECAL_CONFIG_TIMEOUT                 0x30

#define VL53L0X_DEV_ADD 0x29

HAL_StatusTypeDef i2cStats;

unsigned char initData[1]; // to hold data needed for init
unsigned char stop_variable[1]; // might be needed for something not sure if we need it


// check if the model ID is the same as the datasheet
i2cStats = HAL_I2C_Mem_Read(&hi2c1, VL53L0X_DEV_ADD << 1, IDENTIFICATION_MODEL_ID, I2C_MEMADD_SIZE_8BIT, initData, 1, 100);

if (initData[0] != 0xEE)
{
    return false;
}

//* VL53L0X_DataInit() begin

// set sensor to 3.3v mode
i2cStats = HAL_I2C_Mem_Read(&hi2c1, VL53L0X_DEV_ADD << 1, VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV, I2C_MEMADD_SIZE_8BIT, initData, 1, 100);
i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV, I2C_MEMADD_SIZE_8BIT, initData[0] | 0x01, 1, 100);

// set I2C standard mode
i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0x88, I2C_MEMADD_SIZE_8BIT, 0x00, 1, 100);
i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0x80, I2C_MEMADD_SIZE_8BIT, 0x01, 1, 100);
i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0xFF, I2C_MEMADD_SIZE_8BIT, 0x01, 1, 100);
i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0x00, I2C_MEMADD_SIZE_8BIT, 0x00, 1, 100);

i2cStats = HAL_I2C_Mem_Read(&hi2c1, VL53L0X_DEV_ADD << 1, 0x91, I2C_MEMADD_SIZE_8BIT, stop_variable, 1, 100);

i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0x00, I2C_MEMADD_SIZE_8BIT, 0x01, 1, 100);
i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0xFF, I2C_MEMADD_SIZE_8BIT, 0x00, 1, 100);
i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, 0x80, I2C_MEMADD_SIZE_8BIT, 0x00, 1, 100);

// disable SIGNAL_RATE_MSRC (bit 1) and SIGNAL_RATE_PRE_RANGE (bit 4) limit checks (don't know what this does)
i2cStats = HAL_I2C_Mem_Read(&hi2c1, VL53L0X_DEV_ADD << 1, MSRC_CONFIG_CONTROL, I2C_MEMADD_SIZE_8BIT, initData, 1, 100);
i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, MSRC_CONFIG_CONTROL, I2C_MEMADD_SIZE_8BIT, initData[0] | 0x12, 1, 100);

// set final range signal rate limit to 0.25 MCPS (million counts per second) 
// setSignalRateLimit(0.25); //! Do not know if this function is needed, default may be fine

i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, SYSTEM_SEQUENCE_CONFIG, I2C_MEMADD_SIZE_8BIT, 0xFF, 1, 100);

//* VL53L0X_DataInit() end

//* VL53L0X_StaticInit() begin