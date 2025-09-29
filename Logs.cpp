#include "Logs.h"

double Logs::elapsedMilliseconds() const
{
  auto now = std::chrono::steady_clock::now();
  return std::chrono::duration<double, std::milli>(now - start_).count();
}

void Logs::startTimer()
{
  start_ = std::chrono::steady_clock::now();
}

void Logs::add(int x)
{
  ++siz_;

  double delta = x - mean_;
  mean_ += delta / siz_;
  double delta2 = x - mean_;
  varianceAcc_ += delta * delta2;

  if (x > maximum_)
  {
    int shift = x - maximum_;
    if (shift >= Constants::HISTOGRAM_SIZE)
    {
      histogram.fill(0);
    }
    else
    {
      for (int i = shift; i < Constants::HISTOGRAM_SIZE; ++i) histogram[i - shift] = histogram[i];
      for (int i = Constants::HISTOGRAM_SIZE - shift; i < Constants::HISTOGRAM_SIZE; ++i)
        histogram[i] = 0;
    }
    maximum_ = x;
  }

  int idx = Constants::HISTOGRAM_SIZE - 1 - (maximum_ - x);
  if (idx >= 0) ++histogram[idx];
}

double Logs::variance() const
{
  return (siz_ > 0) ? varianceAcc_ / (siz_) : 0.0;
}

void Logs::print(std::ostream& out) const
{
  out << "Z(" << m_ << ", " << n_ << ", " << s_ << ", " << t_ << "): ";
  out << "Max: " << maximum_ << "; ";
  out << "Mean: " << mean_ << "; ";
  out << "Variance: " << variance() << "; ";
  out << "Size: " << siz_ << "; ";
  out << "Time: " << elapsedMilliseconds() << " ms; ";
  out << "Top" << Constants::HISTOGRAM_SIZE << " histogram(relative to max) : ";
  for (auto count : histogram) out << count << " ";
  out << ";";
}
