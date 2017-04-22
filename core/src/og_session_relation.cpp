#include "og/og_session_relation.h"
#include "og/og_session_parameter.h"

#include "og/core/session_relation.h"
#include "og/core/session_parameter.h"

namespace og
{

og_session_relation::og_session_relation(og::core::session_relation_ptr _o) :
  session_relation_ptr_(_o)
{
}

og_session_relation::~og_session_relation()
{}

string og_session_relation::get_id()
{
  return session_relation_ptr_->get_id();
}
string og_session_relation::get_instance_name()
{
  return session_relation_ptr_->get_name();
}
void og_session_relation::set_instance_name(string _name)
{
  session_relation_ptr_->set_name(_name);
  session_relation_ptr_->sync();
}
string og_session_relation::get_comment()
{
  return session_relation_ptr_->get_comment();
}
void og_session_relation::set_comment(string _comment)
{
  session_relation_ptr_->set_comment(_comment);
  session_relation_ptr_->sync();
}
string og_session_relation::get_revision()
{
  return session_relation_ptr_->get_revision();
}
void og_session_relation::set_revision(string _rev)
{
  session_relation_ptr_->set_revision(_rev);
  session_relation_ptr_->sync();
}
string og_session_relation::get_create_date()
{
  return session_relation_ptr_->get_create_date();
}
string og_session_relation::get_update_date()
{
  return session_relation_ptr_->get_update_date();
}

string og_session_relation::get_from_id()
{
  return session_relation_ptr_->get_from_id();
}
string og_session_relation::get_to_id()
{
  return session_relation_ptr_->get_to_id();
}

string og_session_relation::get_schema_relation_id()
{
  return 	session_relation_ptr_->get_schema_relation()->get_id();
}
string og_session_relation::get_schema_relation_type()
{
  return session_relation_ptr_->get_schema_relation()->get_type();
}
string og_session_relation::get_schema_relation_name()
{
  return 	session_relation_ptr_->get_schema_relation()->get_name();
}
string og_session_relation::get_schema_relation_comment()
{
  return 	session_relation_ptr_->get_schema_relation()->get_comment();
}
string og_session_relation::get_schema_relation_create_date()
{
  return 	session_relation_ptr_->get_schema_relation()->get_create_date();
}
string og_session_relation::get_schema_relation_update_date()
{
  return 	session_relation_ptr_->get_schema_relation()->get_update_date();
}

void og_session_relation::set_parameter(string _param_name,
                                        list<og::core::parameter_value_variant>& _values)
{
  session_relation_ptr_->set_parameter(_param_name, _values);
}

boost::optional<og_session_parameter_ptr> og_session_relation::get_parameter(
  std::string _param_name)
{
  boost::optional<og::core::session_parameter_ptr> sp =
    session_relation_ptr_->get_parameter(_param_name);

  if (!sp.is_initialized())
  {
    return boost::optional<og_session_parameter_ptr>();
  }
  else
  {
    return boost::optional<og_session_parameter_ptr>
           (og_session_parameter_ptr
            (new og_session_parameter
             (sp.get()->values_,
              og_schema_parameter_ptr
              (new og_schema_parameter(
                 sp.get()->schema_parameter_)))));
  }
}


} // namespace og;

