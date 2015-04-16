#pragma once

#include "og/core/base.h"

namespace og
{
namespace core
{

class schema_base : public base
{

public:
  schema_base(schema* _schema);
  virtual ~schema_base();

protected:
  schema* schema_;

  virtual void revison_up() {};

  // covered constructor
  schema_base();
};

} //namespace core;
} //namespace og;
