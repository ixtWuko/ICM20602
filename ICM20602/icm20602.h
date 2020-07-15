#ifndef _ICM20602_H_
#define _ICM20602_H_

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>
#include <vector>

/***** registers address *****/
/** Here are the address of all registers of icm20602 */
#define REG_XG_OFFS_TC_H 0x04
#define REG_XG_OFFS_TC_L 0x05
#define REG_YG_OFFS_TC_H 0x07
#define REG_YG_OFFS_TC_L 0x08
#define REG_ZG_OFFS_TC_H 0x0A
#define REG_ZG_OFFS_TC_L 0x0B
#define REG_SELF_TEST_X_ACCEL 0x0D
#define REG_SELF_TEST_Y_ACCEL 0x0E
#define REG_SELF_TEST_Z_ACCEL 0x0F
#define REG_XG_OFFS_USRH 0x13
#define REG_XG_OFFS_USRL 0x14
#define REG_YG_OFFS_USRH 0x15
#define REG_YG_OFFS_USRL 0x16
#define REG_ZG_OFFS_USRH 0x17
#define REG_ZG_OFFS_USRL 0x18

/** sample rate divisor
  * sample rate = 1kHz / (1 + SMPLRT_DIV) */
#define REG_SMPLRT_DIV 0x19

/** config, dlpf config 低通滤波器
  * | - | - | ... | dlpf config[2:0] |
  * sample rate / 2 */
#define REG_CONFIG 0x1A

/** gyro config
  * | xg_set | yg_set | zg_set | full scale select[4:3] | -[2:0] |
  * full scale: 0 pm250, 1 pm500, 2 pm1000, 3 pm2000. */
#define REG_GYRO_CONFIG 0x1B

/** accel config
  * | xa_set | ya_set | za_set | full scale seletc[4:3] | -[2:0] |
  * full scale: 0 pm2g, 1 pm4g, 2 pm8g, 3 pm16g. */
#define REG_ACCEL_CONFIG 0x1C
#define REG_ACCEL_CONFIG_2 0x1D
#define REG_LP_MODE_CFG 0x1E
#define REG_ACCEL_WOM_X_THR 0x20
#define REG_ACCEL_WOM_Y_THR 0x21
#define REG_ACCEL_WOM_Z_THR 0x22

/** fifo enable
  * | gyro | accel | */
#define REG_FIFO_EN 0x23

#define REG_FSYNC_INT 0x36
#define REG_INT_PIN_CFG 0x37
#define REG_INT_ENABLE 0x38 // set all zero to turnoff the interrupt
#define REG_FIFO_WM_INT_STATUS 0x39
#define REG_INT_STATUS 0x3A

/** data output */
#define REG_ACCEL_XOUT_H 0x3B
#define REG_ACCEL_XOUT_L 0x3C
#define REG_ACCEL_YOUT_H 0x3D
#define REG_ACCEL_YOUT_L 0x3E
#define REG_ACCEL_ZOUT_H 0x3F
#define REG_ACCEL_ZOUT_L 0x40
#define REG_TEMP_OUT_H 0x41
#define REG_TEMP_OUT_L 0x42
#define REG_GYRO_XOUT_H 0x43
#define REG_GYRO_XOUT_L 0x44
#define REG_GYRO_YOUT_H 0x45
#define REG_GYRO_YOUT_L 0x46
#define REG_GYRO_ZOUT_H 0x47
#define REG_GYRO_ZOUT_L 0x48

#define REG_SELF_TEST_X_GYRO 0x50
#define REG_SELF_TEST_Y_GYRO 0x51
#define REG_SELF_TEST_Z_GYRO 0x52
#define REG_FIFO_WM_TH1 0x60
#define REG_FIFO_WM_TH2 0x61
#define REG_SIGNAL_PATH_RESET 0x68
#define REG_ACCEL_INTEL_CTRL 0x69
#define REG_USER_CTRL 0x6A

/** power management
  * | reset | sleep | cycle | - | temp_dis | clock[2:0] | */
#define REG_PWR_MGMT_1 0x6B
/** power management 2, save energy
  * set all zero in this driver */
#define REG_PWR_MGMT_2 0x6C
#define REG_I2C_IF 0x70
#define REG_FIFO_COUNTH 0x72
#define REG_FIFO_COUNTL 0x73
#define REG_FIFO_R_W 0x74
#define REG_WHO_AM_I 0x75
#define REG_XA_OFFSET_H 0x77
#define REG_XA_OFFSET_L 0x78
#define REG_YA_OFFSET_H 0x7A
#define REG_YA_OFFSET_L 0x7B
#define REG_ZA_OFFSET_H 0x7D
#define REG_ZA_OFFSET_L 0x7E

#define WHO_AM_I_CONST_VALUE 0x12

/***** Enums *****/

enum icm20602_accel_dlpf {
  ICM20602_ACCEL_DLPF_218_1_HZ = 0,
  ICM20602_ACCEL_DLPF_99_HZ = 2,
  ICM20602_ACCEL_DLPF_44_8_HZ = 3,
  ICM20602_ACCEL_DLPF_21_2_HZ = 4,
  ICM20602_ACCEL_DLPF_10_2_HZ = 5,
  ICM20602_ACCEL_DLPF_5_1_HZ = 6,
  ICM20602_ACCEL_DLPF_420_HZ = 7,
  ICM20602_ACCEL_DLPF_BYPASS_1046_HZ, 
};

enum icm20602_accel_g {
  ICM20602_ACCEL_RANGE_2G = 0,
  ICM20602_ACCEL_RANGE_4G = 1,
  ICM20602_ACCEL_RANGE_8G = 2,
  ICM20602_ACCEL_RANGE_16G = 3,
};


enum icm20602_gyro_dlpf {
  ICM20602_GYRO_DLPF_250_HZ = 0, 
  ICM20602_GYRO_DLPF_176_HZ = 1,
  ICM20602_GYRO_DLPF_92_HZ = 2,
  ICM20602_GYRO_DLPF_41_HZ = 3,
  ICM20602_GYRO_DLPF_20_HZ = 4,
  ICM20602_GYRO_DLPF_10_HZ = 5,
  ICM20602_GYRO_DLPF_5_HZ = 6,
  ICM20602_GYRO_DLPF_3281_HZ = 7,
  ICM20602_GYRO_DLPF_BYPASS_3281_HZ,
  ICM20602_GYRO_DLPF_BYPASS_8173_HZ,
};

enum icm20602_gyro_dps {
  ICM20602_GYRO_RANGE_250_DPS = 0,
  ICM20602_GYRO_RANGE_500_DPS = 1,
  ICM20602_GYRO_RANGE_1000_DPS = 2,
  ICM20602_GYRO_RANGE_2000_DPS = 3,
};

class ICM20602 {
private:
    uint8_t device_address;
    enum icm20602_accel_dlpf accel_dlpf;
    enum icm20602_accel_g accel_g;
    enum icm20602_gyro_dlpf gyro_dlpf;
    enum icm20602_gyro_dps gyro_dps;
    uint8_t sample_rate_div;
    int fd; // using in wiringPi API
    std::vector<float> gyro_sensitivity;
    std::vector<float> accel_sensitivity;

public:
    ICM20602(uint8_t dev_add, enum icm20602_accel_dlpf acc_dlpf, icm20602_accel_g acc_g, enum icm20602_gyro_dlpf gy_dlpf, enum icm20602_gyro_dps gy_dps, uint8_t srd) : 
        device_address(dev_add),
        accel_dlpf(acc_dlpf),
        accel_g(acc_g),
        gyro_dlpf(gy_dlpf),
        gyro_dps(gy_dps),
        sample_rate_div(srd) {}

    int init(); //初始化
    int sleep(); //
    int wakeup(); 
    std::vector<int> read_accel_raw_data();
    std::vector<int> read_gyro_raw_data();
    int read_temp_raw_data();

    std::vector<float> read_accel_data();
    std::vector<float> read_gyro_data();
    float read_temp_data();
};

#endif // _ICM20602_H_