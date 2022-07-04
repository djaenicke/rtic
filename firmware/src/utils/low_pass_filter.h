#ifndef LP_FILTER_H
#define LP_FILTER_H (1)

template <class T>
T lpFilter(const T x, const T y_n_1, const float alpha)
{
  return ((1.0f - alpha) * y_n_1) + (x * alpha);
}

#endif  // LP_FILTER_H
