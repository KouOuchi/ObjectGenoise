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
  OG_COREAPI og_schema_relation(og::core::schema_relation_ptr _rel_ptr);
  OG_COREAPI og_schema_relation(og::core::schema_relation_ptr _rel_ptr, bool _auto_sync);
  OG_COREAPI virtual ~og_schema_relation();

  ///////////////////////////////
  // property
  ///////////////////////////////
  OG_COREAPI string get_id();
  OG_COREAPI string get_type();
  OG_COREAPI string get_name();
  OG_COREAPI void set_name(string _name);
  OG_COREAPI string get_comment();
  OG_COREAPI void set_comment(string _comment);
  OG_COREAPI string get_revision();
  OG_COREAPI string get_create_date();
  OG_COREAPI string get_update_date();
  OG_COREAPI void revision_up();

  OG_COREAPI string get_from_id();
  OG_COREAPI string get_to_id();

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
  OG_COREAPI void get_parameters(
    list<boost::tuple<string, og_schema_parameter_ptr>>*
    _param_name_types);

  OG_COREAPI og::core::parameter_basetype_enum get_parameter_basetype_enum_by_param_name(
    string _param_name);

  OG_COREAPI void add_parameter_definition(string _param_name,
                                const og_schema_parameter_ptr _schm_par);

  template <typename P>
  void get_parameter_definition(string _param_name, P* _param_basetype) {}


  // member variables
  og::core::schema_relation_ptr schema_relation_ptr_;
};

}// namespace og
