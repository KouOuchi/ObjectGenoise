#pragma once
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>

#include "og/og_schema.h"

#include "og/core/schema.h"
#include "og/core/base.h"


namespace og
{
namespace core
{

}
using namespace boost;
using namespace std;

class og_schema_relation
{
public:
  og_schema_relation(og::core::schema_relation_ptr _rel_ptr);
  og_schema_relation(og::core::schema_relation_ptr _rel_ptr, bool _auto_sync);
  virtual ~og_schema_relation();

  ///////////////////////////////
  // property
  ///////////////////////////////
  string get_id();
  string get_type();
  string get_name();
  void set_name(string _name);
  string get_comment();
  void set_comment(string _comment);
  string get_revision();
  string get_create_date();
  string get_update_date();
  void revision_up();

  string get_from_id();
  string get_to_id();

  ///////////////////////////////
  // multiplicity(TODO)
  ///////////////////////////////
  og::core::multiplicity& get_from_multiplicity(){}
  og::core::multiplicity& get_to_multiplicity(){}
  void set_from_multiplicity(og::core::multiplicity& _mul){}
  void set_get_to_multiplicity(og::core::multiplicity& _mul){}
  // multiplicity ends.

  ///////////////////////////////
  // parameters
  ///////////////////////////////
  void get_parameters(
    list<boost::tuple<string, og_schema_parameter_ptr>>*
    _param_name_types);

  og::core::parameter_basetype_enum get_parameter_basetype_enum_by_param_name(
    string _param_name);

  void add_parameter_definition(string _param_name,
                                const og_schema_parameter_ptr _schm_par);

  template <typename P>
  void get_parameter_definition(string _param_name, P* _param_basetype) {}


  // member variables
  og::core::schema_relation_ptr schema_relation_ptr_;
};

}// namespace og
