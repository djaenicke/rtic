#ifndef BMI160_H
#define BMI160_H (1)

#include "../../vendor/bosch/bmi160.h"
#include "i2c.h"

namespace bmi160
{
enum AccelRange
{
  RANGE_2G = 0,
  RANGE_4G,
  RANGE_8G,
  RANGE_16G
};

enum GyroRange
{
  RANGE_250_DPS = 0,
  RANGE_500_DPS,
  RANGE_1000_DPS,
  RANGE_2000_DPS
};

struct Vector3
{
  float x;
  float y;
  float z;
  Vector3(void) : x(0.0f), y(0.0f), z(0.0f) {};
};

struct SensorData
{
  Vector3 lin_acc_mps2;
  Vector3 ang_vel_rps;
  SensorData(void) : lin_acc_mps2(), ang_vel_rps() {};
};

class IMU
{
 public:
  IMU(const hal::I2CMaster& i2c, const uint8_t dev_addr);
  bool init(const AccelRange accel_range, const GyroRange gyro_range);
  bool performFOC(void);
  bool getSensorData(SensorData& data);

 private:
  hal::I2CMaster _i2c;
  bmi160_dev _handle;
  bmi160_offsets _offsets;
  float _accel_scaling;
  float _gyro_scaling;
};
}  // namespace bmi160

#endif  // BMI160_H (1)