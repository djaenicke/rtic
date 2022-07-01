#ifndef PID_H
#define PID_H (1)

namespace pid
{
struct Gains
{
  float kp;
  float ki;
  float kd;
  Gains(const float kp, const float ki, const float kd) : kp(kp), ki(ki), kd(kd) {};
};

class PID
{
 private:
  float kp_;
  float ki_;
  float kd_;
  float tol_; // Tolerance
  float last_e_;
  float integral_;

 public:
  PID(const float tol = 0.0f);
  float step(const float sp, const float fb, const float dt, const float max, const float min);
  void reset(const Gains& gains);
};

}  // namespace pid

#endif  // PID_H
