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
}
using namespace boost;
using namespace std;


class og_schema_object
{
public:
  og_schema_object(og::core::schema_object_ptr _o_ptr);
  og_schema_object(og::core::schema_object_ptr _o_ptr, bool _auto_sync);
  virtual ~og_schema_object();

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

  // property ends.

  ///////////////////////////////
  // relation
  ///////////////////////////////
  og_schema_relation_ptr connect_from(og_schema_object_ptr _from, string _oname);
  og_schema_relation_ptr connect_to(og_schema_object_ptr _to, string _otype);
  //void disconnect();
  //void disconnect(og_schema_object_ptr _object);

  ///////////////////////////////
  // object
  ///////////////////////////////
  og_schema_object_ptr copy_object();
  void delete_object();

  void get_connected_object(list<og_schema_object_ptr>* _schm_object_list);
  void get_connected_object(list<string>& _rel_type_list,
                            list<og_schema_object_ptr>* _schm_object_list);

  void get_connected_object_to(list<og_schema_object_ptr>* _schm_object_list);
  void get_connected_object_to(list<string>& _rel_type_list,
                               list<og_schema_object_ptr>* _schm_object_list);

  void get_connected_object_from(list<og_schema_object_ptr>* _schm_object_list);
  void get_connected_object_from(list<string>& _rel_type_list,
                                 list<og_schema_object_ptr>* _schm_object_list);

  ///////////////////////////////
  // parameters
  ///////////////////////////////
  void get_parameters(
    list<boost::tuple<string, og_schema_parameter_ptr>>*
    _param_name_types);

  og::core::parameter_basetype_enum  get_parameter_basetype_enum_by_param_name(
    string _param_name);

  void add_parameter_definition(string _param_name,
                                const og_schema_parameter_ptr _schm_par);

  template <typename P>
  void get_parameter_definition(string _param_name, P* _param_basetype) {}

  // member variables
  og::core::schema_object_ptr schema_object_ptr_;
};

}// namespace og
