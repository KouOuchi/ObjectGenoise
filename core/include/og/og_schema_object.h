#pragma once
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/optional.hpp>

#include "og/og_schema.h"
#include "og/og_schema_relation.h"

namespace og
{
namespace core
{
class schema_object;
class multiplicity;
}
using namespace boost;
using namespace std;


class og_schema_object
{
public:
  OG_COREAPI og_schema_object(og::core::schema_object_ptr _o_ptr);
  OG_COREAPI og_schema_object(og::core::schema_object_ptr _o_ptr, bool _auto_sync);
  OG_COREAPI virtual ~og_schema_object();

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
  OG_COREAPI void set_revision(string _rev);
  OG_COREAPI string get_create_date();
  OG_COREAPI string get_update_date();
  OG_COREAPI void revision_up();

  // property ends.

  ///////////////////////////////
  // relation
  ///////////////////////////////
  OG_COREAPI og_schema_relation_ptr connect_from(og_schema_object_ptr _from, string _oname);
  OG_COREAPI og_schema_relation_ptr connect_from(og_schema_object_ptr _from, string _oname, og::core::multiplicity _from_mul, og::core::multiplicity _to_mul);
  OG_COREAPI og_schema_relation_ptr connect_to(og_schema_object_ptr _to, string _otype);
  OG_COREAPI og_schema_relation_ptr connect_to(og_schema_object_ptr _to, string _otype, og::core::multiplicity _from_mul, og::core::multiplicity _to_mul);
  //void disconnect();
  //void disconnect(og_schema_object_ptr _object);

  ///////////////////////////////
  // object
  ///////////////////////////////
  OG_COREAPI og_schema_object_ptr copy_object();
  OG_COREAPI void delete_object();
  OG_COREAPI void disconnect();
  OG_COREAPI void disconnect(og_schema_object_ptr _schm_object_ptr);

  OG_COREAPI void get_connected_object(list<og_schema_object_ptr>* _schm_object_list);
  OG_COREAPI void get_connected_object(list<string>& _rel_type_list,
                            list<og_schema_object_ptr>* _schm_object_list);

  OG_COREAPI void get_connected_object_to(list<og_schema_object_ptr>* _schm_object_list);
  OG_COREAPI void get_connected_object_to(list<string>& _rel_type_list,
                               list<og_schema_object_ptr>* _schm_object_list);

  OG_COREAPI void get_connected_object_from(list<og_schema_object_ptr>* _schm_object_list);
  OG_COREAPI void get_connected_object_from(list<string>& _rel_type_list,
                                 list<og_schema_object_ptr>* _schm_object_list);

  ///////////////////////////////
  // parameters
  ///////////////////////////////
  OG_COREAPI void get_parameters(
    list<boost::tuple<string, og_schema_parameter_ptr>>*
    _param_name_types);

  OG_COREAPI og::core::parameter_basetype_enum  get_parameter_basetype_enum_by_param_name(
    string _param_name);

  OG_COREAPI void add_parameter_definition(string _param_name,
                                const og_schema_parameter_ptr _schm_par);

  OG_COREAPI void delete_parameter_definition(string _param_name,
                                const og_schema_parameter_ptr _schm_par);

  template <typename P>
  void get_parameter_definition(string _param_name, P* _param_basetype) {}

  // member variables
  og::core::schema_object_ptr schema_object_ptr_;
};

}// namespace og
