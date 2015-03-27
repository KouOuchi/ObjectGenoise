#include "og/core/schema_base.h"


namespace og
{
namespace core
{

schema_base::schema_base() : base()
{

}

schema_base::schema_base(schema* _schema) : base()
{
  schema_ = _schema;
}

schema_base::~schema_base()
{

}
} //namespace core;
} //namespace og;