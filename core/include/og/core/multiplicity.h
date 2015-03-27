#pragma once
namespace og
{
namespace core
{

class multiplicity
{
public:
  multiplicity()
    : max_multiplicity_(1), min_multiplicity_(0)
  {}

  multiplicity(unsigned int _max, unsigned int _min)
    : max_multiplicity_(_max), min_multiplicity_(_min)
  {}

  virtual ~multiplicity()
  {}

  void set_multiplicity_(unsigned int _min, unsigned int _max)
  {
    // check
    if (_max < _min)
    {
      throw og::core::exception() <<
                            exception_message("unexpected value, _min or _max");
    }
    // check
    if (_max < 1)
    {
      throw og::core::exception() <<
                            exception_message("unexpected value, _max");
    }
    min_multiplicity_ = _min;
    max_multiplicity_ = _max;
  }
  int get_max() { return max_multiplicity_; }
  int get_min() { return min_multiplicity_; }

  static int UNLIMITED() { return -1; }

private:
  int max_multiplicity_; // negative means max
  int min_multiplicity_;

};

} //namespace core;
} //namespace og;
