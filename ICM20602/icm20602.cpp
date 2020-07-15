#include "icm20602.h"

int ICM20602::init() {
    int status = wiringPiSetup();
    if (status == -1) {
        std::cout << "ERROR: failed to initial wiringPi" << std::endl;
        return -1;
    }
    this->fd = wiringPiI2CSetup(this->device_address);
    if (fd == -1) {
        std::cout << "ERROR: failed to inital I2C" << std::endl;
        return -1;
    }

    if (wiringPiI2CReadReg8(fd, REG_WHO_AM_I) != WHO_AM_I_CONST_VALUE) {
        std::cout << "ERROR: wrong device. This is not a icm20602 sensor." << std::endl;
        return -1;
    }

    // reset
    wiringPiI2CWriteReg8(fd, REG_PWR_MGMT_1, 0x01 << 7);
    delay(500);

    // sample rate
    wiringPiI2CWriteReg8(fd, REG_SMPLRT_DIV, this->sample_rate_div);

    // dlpf config
    wiringPiI2CWriteReg8(fd, REG_CONFIG, this->gyro_dlpf);

    // gyro config
    wiringPiI2CWriteReg8(fd, REG_GYRO_CONFIG, this->gyro_dps << 3);

    // accel config
    wiringPiI2CWriteReg8(fd, REG_ACCEL_CONFIG, this->accel_g << 3);
    wiringPiI2CWriteReg8(fd, REG_ACCEL_CONFIG_2, this->accel_dlpf);

    // disable fifo
    wiringPiI2CWriteReg8(fd, REG_FIFO_EN, 0x00);

    // wake up
    wiringPiI2CWriteReg8(fd, REG_PWR_MGMT_1, 0x01);
    delay(500);

    // add the sensitivity data
    gyro_sensitivity.push_back(131.0);
    gyro_sensitivity.push_back(65.5);
    gyro_sensitivity.push_back(32.8);
    gyro_sensitivity.push_back(16.4);
    accel_sensitivity.push_back(16384.0);
    accel_sensitivity.push_back(8192.0);
    accel_sensitivity.push_back(4096.0);
    accel_sensitivity.push_back(2048.0);
    return 0;
}

int ICM20602::sleep() {
    uint8_t tmp = wiringPiI2CReadReg8(fd, REG_PWR_MGMT_1);
    tmp = tmp | (0x01 << 6);
    wiringPiI2CWriteReg8(fd, REG_PWR_MGMT_1, tmp);
    return 0;
}

int ICM20602::wakeup() {
    uint8_t tmp = wiringPiI2CReadReg8(fd, REG_PWR_MGMT_1);
    tmp = tmp & ~(0x01 << 6);
    wiringPiI2CWriteReg8(fd, REG_PWR_MGMT_1, tmp);
    return 0;
}

std::vector<int> ICM20602::read_accel_raw_data() {
    std::vector<int> accel_raw_data(3, 0);
    accel_raw_data[0] = wiringPiI2CReadReg16(fd, REG_ACCEL_XOUT_H);
    accel_raw_data[1] = wiringPiI2CReadReg16(fd, REG_ACCEL_YOUT_H);
    accel_raw_data[2] = wiringPiI2CReadReg16(fd, REG_ACCEL_ZOUT_H);
    for (int i = 0; i < 3; ++i) {
        if (accel_raw_data[i] > 32768) {
            accel_raw_data[i] -= 65536;
        }
    }
    return accel_raw_data;
}

std::vector<int> ICM20602::read_gyro_raw_data() {
    std::vector<int> gyro_raw_data(3, 0);
    gyro_raw_data[0] = wiringPiI2CReadReg16(fd, REG_GYRO_XOUT_H);
    gyro_raw_data[1] = wiringPiI2CReadReg16(fd, REG_GYRO_YOUT_H);
    gyro_raw_data[2] = wiringPiI2CReadReg16(fd, REG_GYRO_ZOUT_H);
    for (int i = 0; i < 3; ++i) {
        if (gyro_raw_data[i] > 32768) {
            gyro_raw_data[i] -= 65536;
        }
    }
    return gyro_raw_data;
}

int ICM20602::read_temp_raw_data() {
    int temp_raw_data = 0;
    temp_raw_data = wiringPiI2CReadReg16(fd, REG_TEMP_OUT_H);
    if (temp_raw_data > 32768) {
        temp_raw_data -= 65536;
    }
    return temp_raw_data;
}

std::vector<float> ICM20602::read_accel_data() {
    int tmp[3] = {0};
    std::vector<float> accel_data(3, 0.0);
    float sensitivity = accel_sensitivity[accel_g];
    tmp[0] = wiringPiI2CReadReg16(fd, REG_ACCEL_XOUT_H);
    tmp[1] = wiringPiI2CReadReg16(fd, REG_ACCEL_YOUT_H);
    tmp[2] = wiringPiI2CReadReg16(fd, REG_ACCEL_ZOUT_H);
    for (int i = 0; i < 3; ++i) {
        if (tmp[i] > 32768) {
            tmp[i] -= 65536;
        }
        accel_data[i] = tmp[i] / sensitivity;
    }
    return accel_data;    
}

std::vector<float> ICM20602::read_gyro_data() {
    int tmp[3] = {0};
    std::vector<float> gyro_data(3, 0.0);
    float sensitivity = gyro_sensitivity[gyro_dps];
    tmp[0] = wiringPiI2CReadReg16(fd, REG_GYRO_XOUT_H);
    tmp[1] = wiringPiI2CReadReg16(fd, REG_GYRO_YOUT_H);
    tmp[2] = wiringPiI2CReadReg16(fd, REG_GYRO_ZOUT_H);
    for (int i = 0; i < 3; ++i) {
        if (tmp[i] > 32768) {
            tmp[i] -= 65536;
        }
        gyro_data[i] = tmp[i] / sensitivity;
    }
    return gyro_data;
}

float ICM20602::read_temp_data() {
    int temp_data = wiringPiI2CReadReg16(fd, REG_TEMP_OUT_H);
    if (temp_data > 32768) {
        temp_data -= 65535;
    }
    return temp_data / 326.8 + 25;
}