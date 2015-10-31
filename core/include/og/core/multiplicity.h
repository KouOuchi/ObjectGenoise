#pragma once
namespace og
{
namespace core
{

class multiplicity
{
public:
  multiplicity()
    : max_multiplicity_(-1), min_multiplicity_(0)
  {}

  multiplicity(int _min, int _max)
    : max_multiplicity_(_max), min_multiplicity_(_min)
  {}

  multiplicity(const multiplicity& _mul)
  {
    min_multiplicity_ = _mul.get_min();
    max_multiplicity_ = _mul.get_max();
  }

  virtual ~multiplicity()
  {}

  bool set_multiplicity(int _min, int _max)
  {
    // check
    if (_max != -1 && _max < _min)
    {
		OG_LOG << "unexpected value, _min or _max";
		return false;
    }
    // check
    if (_max != -1 &&_max < 1)
    {
		OG_LOG << "unexpected value, _max";
		return false;

    }
    min_multiplicity_ = _min;
    max_multiplicity_ = _max;
	return true;
  }
  int get_max() const { return max_multiplicity_; }
  int get_min() const { return min_multiplicity_; }

  static int UNLIMITED() { return -1; }

private:
  int max_multiplicity_; // negative means max
  int min_multiplicity_;

};

} //namespace core;
} //namespace og;
