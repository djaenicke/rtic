#include "bmi160.h"

#include "timing.h"

namespace bmi160
{
static const float PI = 3.14159265359f;
static const float G = 9.81;
static const I2C_HandleTypeDef* i2c_handle;

IMU::IMU(const hal::I2CMaster& i2c, const uint8_t dev_addr) : _i2c(i2c)
{
  i2c_handle = _i2c.get_handle();

  // Set the interface mode to I2C and set the device address
  _handle.intf = BMI160_I2C_INTF;
  _handle.id = (dev_addr << 1u);  // left shift here for HAL compatibility

  // Attach the callbacks
  _handle.read = &read_callback;
  _handle.write = &write_callback;
  _handle.delay_ms = &delay_ms;
}

bool IMU::init(const AccelRange accel_range, const GyroRange gyro_range)
{
  if (BMI160_OK != bmi160_init(&_handle))
  {
    return false;
  }

  // Set the accelerometer configuration
  _handle.accel_cfg.power = BMI160_ACCEL_NORMAL_MODE;
  _handle.accel_cfg.odr = BMI160_ACCEL_ODR_50HZ;
  _handle.accel_cfg.bw = BMI160_ACCEL_BW_NORMAL_AVG4;
  switch (accel_range)
  {
    case RANGE_2G:
      _handle.accel_cfg.range = BMI160_ACCEL_RANGE_2G;
      _accel_scaling = 2.0f / 32768.0f * G;
      break;
    case RANGE_4G:
      _handle.accel_cfg.range = BMI160_ACCEL_RANGE_4G;
      _accel_scaling = 4.0f / 32768.0f * G;
      break;
    case RANGE_8G:
      _handle.accel_cfg.range = BMI160_ACCEL_RANGE_8G;
      _accel_scaling = 8.0f / 32768.0f * G;
      break;
    case RANGE_16G:
      _handle.accel_cfg.range = BMI160_ACCEL_RANGE_16G;
      _accel_scaling = 16.0f / 32768.0f * G;
      break;
    default:
      // todo assert
      return false;
  }

  // Set the gyroscope configuration
  _handle.gyro_cfg.power = BMI160_GYRO_NORMAL_MODE;
  _handle.gyro_cfg.odr = BMI160_GYRO_ODR_50HZ;
  _handle.gyro_cfg.bw = BMI160_GYRO_BW_NORMAL_MODE;
  switch (_handle.gyro_cfg.range)
  {
    case RANGE_250_DPS:
      _handle.gyro_cfg.range = BMI160_GYRO_RANGE_250_DPS;
      _gyro_scaling = 250.0 / 32768.0 * (PI / 180);
      break;
    case RANGE_500_DPS:
      _handle.gyro_cfg.range = BMI160_GYRO_RANGE_500_DPS;
      _gyro_scaling = 500.0 / 32768.0 * (PI / 180);
      break;
    case RANGE_1000_DPS:
      _handle.gyro_cfg.range = BMI160_GYRO_RANGE_1000_DPS;
      _gyro_scaling = 1000.0 / 32768.0 * (PI / 180);
      break;
    case RANGE_2000_DPS:
      _handle.gyro_cfg.range = BMI160_GYRO_RANGE_2000_DPS;
      _gyro_scaling = 2000.0 / 32768.0 * (PI / 180);
      break;
    default:
      // todo assert
      return false;
  }

  return (BMI160_OK == bmi160_set_sens_conf(&_handle));
}

bool IMU::perform_foc(void)
{
  bmi160_foc_conf foc_cfg;
  foc_cfg.acc_off_en = BMI160_ENABLE;
  foc_cfg.foc_acc_x = BMI160_FOC_ACCEL_0G;
  foc_cfg.foc_acc_y = BMI160_FOC_ACCEL_0G;
  foc_cfg.foc_acc_z = BMI160_FOC_ACCEL_POSITIVE_G;
  foc_cfg.foc_gyr_en = BMI160_ENABLE;
  foc_cfg.gyro_off_en = BMI160_ENABLE;
  return (BMI160_OK == bmi160_start_foc(&foc_cfg, &_offsets, &_handle));
}

bool IMU::get_sensor_data(SensorData& data)
{
  bmi160_sensor_data accel;
  bmi160_sensor_data gyro;

  if (BMI160_OK ==
      bmi160_get_sensor_data(BMI160_ACCEL_SEL | BMI160_GYRO_SEL, &accel, &gyro, &_handle))
  {
    data.lin_acc_mps2.x = (accel.x * _accel_scaling);
    data.lin_acc_mps2.y = (accel.y * _accel_scaling);
    data.lin_acc_mps2.z = (accel.z * _accel_scaling);
    data.ang_vel_rps.x = (gyro.x * _gyro_scaling);
    data.ang_vel_rps.y = (gyro.y * _gyro_scaling);
    data.ang_vel_rps.z = (gyro.z * _gyro_scaling);
    return true;
  }
  else
  {
    return false;
  }
}

int8_t IMU::read_callback(uint8_t dev_addr, uint8_t reg_addr, uint8_t* read_data, uint16_t len)
{
  if (HAL_OK == HAL_I2C_Mem_Read((I2C_HandleTypeDef*)i2c_handle, dev_addr, reg_addr,
                                 I2C_MEMADD_SIZE_8BIT, read_data, len, HAL_MAX_DELAY))
  {
    return 0;
  }
  else
  {
    return -1;
  }
}

int8_t IMU::write_callback(uint8_t dev_addr, uint8_t reg_addr, uint8_t* data, uint16_t len)
{
  if (HAL_OK == HAL_I2C_Mem_Write((I2C_HandleTypeDef*)i2c_handle, dev_addr, reg_addr,
                                  I2C_MEMADD_SIZE_8BIT, data, len, HAL_MAX_DELAY))
  {
    return 0;
  }
  else
  {
    return -1;
  }
}

}  // namespace bmi160