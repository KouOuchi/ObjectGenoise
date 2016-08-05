#pragma once
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include "og/og_schema.h"
#include "og/og_schema_relation.h"

#include "og/core/session_object.h"

namespace og
{
namespace core
{
class session_object;
}
using namespace boost;
using namespace std;

class og_session_object
{
public:
  OG_COREAPI og_session_object(og::core::session_object_ptr _o_ptr);
  OG_COREAPI virtual ~og_session_object();

  ///////////////////////////////
  // property
  ///////////////////////////////
  OG_COREAPI string get_id();
  OG_COREAPI string get_instance_name();
  OG_COREAPI void set_instance_name(string _name);
  OG_COREAPI string get_comment();
  OG_COREAPI void set_comment(string _comment);
  OG_COREAPI string get_revision();
  OG_COREAPI void set_revision(string _rev);
  OG_COREAPI string get_create_date();
  OG_COREAPI string get_update_date();

  OG_COREAPI string get_schema_object_id();
  OG_COREAPI string get_schema_object_type();
  OG_COREAPI string get_schema_object_name();
  OG_COREAPI string get_schema_object_comment();
  OG_COREAPI string get_schema_object_create_date();
  OG_COREAPI string get_schema_object_update_date();
  // property ends.

  ///////////////////////////////
  // oepration
  ///////////////////////////////
  OG_COREAPI void delete_object(og::core::connection_direction_enum _direction);
  OG_COREAPI void delete_object();
  OG_COREAPI og_session_object_ptr copy_object();
  OG_COREAPI og_session_object_ptr copy_object(og::core::connection_direction_enum
      _direction);

  ///////////////////////////////
  // relation
  ///////////////////////////////
  OG_COREAPI og_session_relation_ptr connect_to(og_session_object_ptr _sesn_obj,
      string _rel_type);
  OG_COREAPI og_session_relation_ptr connect_from(og_session_object_ptr _sesn_obj,
      string _rel_type);
  OG_COREAPI void disconnect(og_session_object_ptr _sesn_obj);

  OG_COREAPI void get_connected_object(list<og_session_object_ptr>*
                                       _sesn_obj_list);
  OG_COREAPI void get_connected_object(list<string>& _rel_type_list,
                                       list<og_session_object_ptr>* _sesn_obj_list);
  OG_COREAPI void get_connected_object_from(list<og_session_object_ptr>*
      _sesn_obj_list);
  OG_COREAPI void get_connected_object_from(list<string>& _rel_type_list,
      list<og_session_object_ptr>* _sesn_obj_list);
  OG_COREAPI void get_connected_object_to(list<og_session_object_ptr>*
                                          _sesn_obj_list);
  OG_COREAPI void get_connected_object_to(list<string>& _rel_type_list,
                                          list<og_session_object_ptr>* _sesn_obj_list);

  OG_COREAPI void get_connected(
    list<boost::tuple<og_session_object_ptr, og_session_relation_ptr>>*
    _sesn_obj_rel_list);
  OG_COREAPI void get_connected(list<string>& _rel_type_list,
                                list<boost::tuple<og_session_object_ptr, og_session_relation_ptr>>*
                                _sesn_obj_rel_list);
  OG_COREAPI void get_connected_from(
    list<boost::tuple<og_session_object_ptr, og_session_relation_ptr>>*
    _sesn_obj_rel_list);
  OG_COREAPI void get_connected_from(list<string>& _rel_type_list,
                                     list<boost::tuple<og_session_object_ptr, og_session_relation_ptr>>*
                                     _sesn_obj_rel_list);
  OG_COREAPI void get_connected_to(
    list<boost::tuple<og_session_object_ptr, og_session_relation_ptr>>*
    _sesn_obj_rel_list);
  OG_COREAPI void get_connected_to(list<string>& _rel_type_list,
                                   list<boost::tuple<og_session_object_ptr, og_session_relation_ptr>>*
                                   _sesn_obj_rel_list);

  //OG_COREAPI og::core::connection_validation_result_enum validate_connect_to(
  //  og_session_object_ptr _sesn_obj_ptr);
  OG_COREAPI og::core::connection_validation_result_enum validate_connect_to(
    og_session_object_ptr _sesn_obj_ptr, string _rel_type);
  //OG_COREAPI og::core::connection_validation_result_enum validate_connect_from(
  //  og_session_object_ptr _sesn_obj_ptr);
  OG_COREAPI og::core::connection_validation_result_enum validate_connect_from(
    og_session_object_ptr _sesn_obj_ptr,
    string _rel_type);
  OG_COREAPI void get_connectable_relation_type_to(list<string>* _rel_type_list);
  OG_COREAPI void get_connectable_relation_type_from(list<string>*
      _rel_type_list);
  OG_COREAPI void get_connectable_to(map<string, list<og_schema_object_ptr> >*
                                     _rel_type_map);
  OG_COREAPI void get_connectable_from(map<string, list<og_schema_object_ptr> >*
                                       _rel_type_map);

  ///////////////////////////////
  // parameters
  ///////////////////////////////
  template <typename T>
  void set_parameter_value(std::string _param_name, T _value)
  {
    session_object_ptr_->set_parameter_value(_param_name, _value);
    session_object_ptr_->sync();
  }
  template <typename T>
  void set_parameter_values(std::string _param_name, list<T>& _values)
  {
    session_object_ptr_->set_parameter_values(_param_name, _values);
    session_object_ptr_->sync();
  }
  template <typename T>
  void get_parameter_value(std::string _param_name, T* _value)
  {
    session_object_ptr_->get_parameter_value(_param_name, _value);
  }
  template <typename T>
  void get_parameter_values(std::string _param_name, list<T>* _values)
  {
    session_object_ptr_->get_parameter_values(_param_name, _values);
  }

  OG_COREAPI boost::optional<og_session_parameter_ptr> get_parameter(
    string _param_name);
  OG_COREAPI void set_parameter(string _param_name,
                                list<og::core::parameter_value_variant>& _values);

  // object parameters end.

  // member variables
  og::core::session_object_ptr session_object_ptr_;
};

}// namespace og
