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

int VL53L0X_Init()
{
    HAL_StatusTypeDef i2cStats;

    unsigned char initData[1]; // to hold data needed for init
    unsigned char stop_variable[1]; // might be needed for something not sure if we need it
    unsigned char ref_spad_map[6];
    uint8_t spad_count;
    uint8_t spad_type_is_aperture;
    uint8_t first_spad_to_enable;
    uint8_t spads_enabled = 0;


    // check if the model ID is the same as the datasheet
    i2cStats = HAL_I2C_Mem_Read(&hi2c1, VL53L0X_DEV_ADD << 1, IDENTIFICATION_MODEL_ID, I2C_MEMADD_SIZE_8BIT, initData, 1, 100);

    if (initData[0] != 0xEE)
    {
        return 0;
    }

    //* VL53L0X_DataInit() begin

    // set sensor to 3.3v mode
    i2cStats = HAL_I2C_Mem_Read(&hi2c1, VL53L0X_DEV_ADD << 1, VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV, I2C_MEMADD_SIZE_8BIT, initData, 1, 100);
    i2cStats = VL53L0X_Write_Value(VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV, initData[0] | 0x01);

    // set I2C standard mode
    i2cStats = VL53L0X_Write_Value(0x88, 0x00);
    i2cStats = VL53L0X_Write_Value(0x80, 0x01);
    i2cStats = VL53L0X_Write_Value(0xFF, 0x01);
    i2cStats = VL53L0X_Write_Value(0x00, 0x00);

    i2cStats = HAL_I2C_Mem_Read(&hi2c1, VL53L0X_DEV_ADD << 1, 0x91, I2C_MEMADD_SIZE_8BIT, stop_variable, 1, 100);

    i2cStats = VL53L0X_Write_Value(0x00, 0x01);
    i2cStats = VL53L0X_Write_Value(0xFF, 0x00);
    i2cStats = VL53L0X_Write_Value(0x80, 0x00);

    // disable SIGNAL_RATE_MSRC (bit 1) and SIGNAL_RATE_PRE_RANGE (bit 4) limit checks (don't know what this does)
    i2cStats = HAL_I2C_Mem_Read(&hi2c1, VL53L0X_DEV_ADD << 1, MSRC_CONFIG_CONTROL, I2C_MEMADD_SIZE_8BIT, initData, 1, 100);
    i2cStats = VL53L0X_Write_Value(MSRC_CONFIG_CONTROL, initData[0] | 0x12);

    // set final range signal rate limit to 0.25 MCPS (million counts per second) 
    // setSignalRateLimit(0.25); //! Do not know if this function is needed, default may be fine

    i2cStats = VL53L0X_Write_Value(SYSTEM_SEQUENCE_CONFIG, 0xFF);

    //* VL53L0X_DataInit() end

    //* VL53L0X_StaticInit() begin

    if (!getSpadInfo(&spad_count, &spad_type_is_aperture)) // I do not know what this does
    {
        return 0;
    }

    i2cStats = HAL_I2C_Mem_Read(&hi2c1, VL53L0X_DEV_ADD << 1, GLOBAL_CONFIG_SPAD_ENABLES_REF_0, I2C_MEMADD_SIZE_8BIT, ref_spad_map, 6, 100);

    //* VL53L0X_set_reference_spads() begin

    i2cStats = VL53L0X_Write_Value(0xFF, 0x01);
    i2cStats = VL53L0X_Write_Value(DYNAMIC_SPAD_REF_EN_START_OFFSET, 0x00);
    i2cStats = VL53L0X_Write_Value(DYNAMIC_SPAD_NUM_REQUESTED_REF_SPAD, 0x2C);
    i2cStats = VL53L0X_Write_Value(0xFF, 0x00);
    i2cStats = VL53L0X_Write_Value(GLOBAL_CONFIG_REF_EN_START_SELECT, 0xB4);

    first_spad_to_enable = spad_type_is_aperture ? 12 : 0; // 12 is the first aperture spad
    spads_enabled = 0;

    for (uint8_t i = 0; i < 48; i++)
    {
        if (i < first_spad_to_enable || spads_enabled == spad_count)
        {
            ref_spad_map[i / 8] &= ~(1 << (i % 8));
        }
        else if ((ref_spad_map[i / 8] >> (i % 8)) & 0x1)
        {
            spads_enabled++;
        }
    }

    i2cStats = HAL_I2C_Mem_Write(&hi2c1, VL53L0X_DEV_ADD << 1, GLOBAL_CONFIG_SPAD_ENABLES_REF_0, I2C_MEMADD_SIZE_8BIT, ref_spad_map, 6, 100);

    //* VL53L0X_set_reference_spads() end

    //* VL53L0X_load_tuning_settings() begin
    // settings from vl53l0x_tuning.h

    i2cStats = VL53L0X_Write_Value(0xFF, 0x01);
    i2cStats = VL53L0X_Write_Value(0x00, 0x00);
    i2cStats = VL53L0X_Write_Value(0xFF, 0x00);
    i2cStats = VL53L0X_Write_Value(0x09, 0x00);
    i2cStats = VL53L0X_Write_Value(0x10, 0x00);
    i2cStats = VL53L0X_Write_Value(0x11, 0x00);
    i2cStats = VL53L0X_Write_Value(0x24, 0x01);
    i2cStats = VL53L0X_Write_Value(0x25, 0xFF);
    i2cStats = VL53L0X_Write_Value(0x75, 0x00);
    i2cStats = VL53L0X_Write_Value(0xFF, 0x01);
    i2cStats = VL53L0X_Write_Value(0x4E, 0x2C);
    i2cStats = VL53L0X_Write_Value(0x48, 0x00);
    i2cStats = VL53L0X_Write_Value(0x30, 0x20);
    i2cStats = VL53L0X_Write_Value(0xFF, 0x00);
    i2cStats = VL53L0X_Write_Value(0x30, 0x09);
    i2cStats = VL53L0X_Write_Value(0x54, 0x00);
    i2cStats = VL53L0X_Write_Value(0x31, 0x04);
    i2cStats = VL53L0X_Write_Value(0x32, 0x03);
    i2cStats = VL53L0X_Write_Value(0x40, 0x83);
    i2cStats = VL53L0X_Write_Value(0x46, 0x25);
    i2cStats = VL53L0X_Write_Value(0x60, 0x00);
    i2cStats = VL53L0X_Write_Value(0x27, 0x00);
    i2cStats = VL53L0X_Write_Value(0x50, 0x06);
    i2cStats = VL53L0X_Write_Value(0x51, 0x00);
    i2cStats = VL53L0X_Write_Value(0x52, 0x96);
    i2cStats = VL53L0X_Write_Value(0x56, 0x08);
    i2cStats = VL53L0X_Write_Value(0x57, 0x30);
    i2cStats = VL53L0X_Write_Value(0x61, 0x00);
    i2cStats = VL53L0X_Write_Value(0x62, 0x00);
    i2cStats = VL53L0X_Write_Value(0x64, 0x00);
    i2cStats = VL53L0X_Write_Value(0x65, 0x00);
    i2cStats = VL53L0X_Write_Value(0x66, 0xA0);
    i2cStats = VL53L0X_Write_Value(0xFF, 0x01);
    i2cStats = VL53L0X_Write_Value(0x22, 0x32);
    i2cStats = VL53L0X_Write_Value(0x47, 0x14);
    i2cStats = VL53L0X_Write_Value(0x49, 0xFF);
    i2cStats = VL53L0X_Write_Value(0x4A, 0x00);
    i2cStats = VL53L0X_Write_Value(0xFF, 0x00);
    i2cStats = VL53L0X_Write_Value(0x7A, 0x0A);
    i2cStats = VL53L0X_Write_Value(0x7B, 0x00);
    i2cStats = VL53L0X_Write_Value(0x78, 0x21);
    i2cStats = VL53L0X_Write_Value(0xFF, 0x01);
    i2cStats = VL53L0X_Write_Value(0x23, 0x34);
    i2cStats = VL53L0X_Write_Value(0x42, 0x00);
    i2cStats = VL53L0X_Write_Value(0x44, 0xFF);
    i2cStats = VL53L0X_Write_Value(0x45, 0x26);
    i2cStats = VL53L0X_Write_Value(0x46, 0x05);
    i2cStats = VL53L0X_Write_Value(0x40, 0x40);
    i2cStats = VL53L0X_Write_Value(0x0E, 0x06);
    i2cStats = VL53L0X_Write_Value(0x20, 0x1A);
    i2cStats = VL53L0X_Write_Value(0x43, 0x40);
    i2cStats = VL53L0X_Write_Value(0xFF, 0x00);
    i2cStats = VL53L0X_Write_Value(0x34, 0x03);
    i2cStats = VL53L0X_Write_Value(0x35, 0x44);
    i2cStats = VL53L0X_Write_Value(0xFF, 0x01);
    i2cStats = VL53L0X_Write_Value(0x31, 0x04);
    i2cStats = VL53L0X_Write_Value(0x4B, 0x09);
    i2cStats = VL53L0X_Write_Value(0x4C, 0x05);
    i2cStats = VL53L0X_Write_Value(0x4D, 0x04);
    i2cStats = VL53L0X_Write_Value(0xFF, 0x00);
    i2cStats = VL53L0X_Write_Value(0x44, 0x00);
    i2cStats = VL53L0X_Write_Value(0x45, 0x20);
    i2cStats = VL53L0X_Write_Value(0x47, 0x08);
    i2cStats = VL53L0X_Write_Value(0x48, 0x28);
    i2cStats = VL53L0X_Write_Value(0x67, 0x00);
    i2cStats = VL53L0X_Write_Value(0x70, 0x04);
    i2cStats = VL53L0X_Write_Value(0x71, 0x01);
    i2cStats = VL53L0X_Write_Value(0x72, 0xFE);
    i2cStats = VL53L0X_Write_Value(0x76, 0x00);
    i2cStats = VL53L0X_Write_Value(0x77, 0x00);
    i2cStats = VL53L0X_Write_Value(0xFF, 0x01);
    i2cStats = VL53L0X_Write_Value(0x0D, 0x01);
    i2cStats = VL53L0X_Write_Value(0xFF, 0x00);
    i2cStats = VL53L0X_Write_Value(0x80, 0x01);
    i2cStats = VL53L0X_Write_Value(0x01, 0xF8);
    i2cStats = VL53L0X_Write_Value(0xFF, 0x01);
    i2cStats = VL53L0X_Write_Value(0x8E, 0x01);
    i2cStats = VL53L0X_Write_Value(0x00, 0x01);
    i2cStats = VL53L0X_Write_Value(0xFF, 0x00);
    i2cStats = VL53L0X_Write_Value(0x80, 0x00);

    //* VL53L0X_load_tuning_settings() end

    // "Set interrupt config to new sample ready"
    //* VL53L0X_SetGpioConfig() begin

    i2cStats = VL53L0X_Write_Value(SYSTEM_INTERRUPT_CONFIG_GPIO, 0x04);

    i2cStats = HAL_I2C_Mem_Read(&hi2c1, VL53L0X_DEV_ADD << 1, GPIO_HV_MUX_ACTIVE_HIGH, I2C_MEMADD_SIZE_8BIT, initData, 1, 100);
    i2cStats = VL53L0X_Write_Value(GPIO_HV_MUX_ACTIVE_HIGH, initData[0] & ~0x10); // active low

    i2cStats = VL53L0X_Write_Value(SYSTEM_INTERRUPT_CLEAR, 0x01);

    //* VL53L0X_SetGpioConfig() end

    // measurement_timing_budget_us = getMeasurementTimingBudget(); //! Not sure if this is needed. Need to test if the default provides accurate measurements

    // "Disable MSRC and TCC by default"
    //* VL53L0X_SetSequenceStepEnable() begin

    i2cStats = VL53L0X_Write_Value(SYSTEM_SEQUENCE_CONFIG, 0xE8);

    //* VL53L0X_SetSequenceStepEnable() end

    // setMeasurementTimingBudget(measurement_timing_budget_us); //! Timing budget is recalibrated. Again, not sure if this is needed

    //* VL53L0X_StaticInit() end

    //* VL53L0X_PerformRefCalibration() begin

    //* VL53L0X_perform_vhv_calibration() begin

    i2cStats = VL53L0X_Write_Value(SYSTEM_SEQUENCE_CONFIG, 0x01);

    if (!performSingleRefCalibration(0x40))
    {
        return 0;
    }

    //* VL53L0X_perform_vhv_calibration() end

    //* VL53L0X_perform_phase_calibration() begin

    i2cStats = VL53L0X_Write_Value(SYSTEM_SEQUENCE_CONFIG, 0x02);

    if (!performSingleRefCalibration(0x00))
    {
        return 0;
    }

    //* VL53L0X_perform_phase_calibration() end

    // "restore the previous Sequence Config"
    i2cStats = VL53L0X_Write_Value(SYSTEM_SEQUENCE_CONFIG, 0xE8);

    //* VL53L0X_PerformRefCalibration() end

    return 1;
}