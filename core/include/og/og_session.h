#pragma once
#include <string>

#include <boost/scoped_ptr.hpp>
#include <boost/optional.hpp>

#include "og/core/og.h"
#include "og/og_schema.h"
#include "og/og_session_object.h"

namespace og
{
namespace core
{
class session;
}
using namespace boost;
using namespace std;


class og_session
{
public:
  OG_COREAPI og_session();
  OG_COREAPI virtual ~og_session();

  ///////////////////////////////
  // schema
  ///////////////////////////////
  OG_COREAPI og_schema* schema();

  ///////////////////////////////
  // file io and management
  ///////////////////////////////
  OG_COREAPI bool import_from_file(string _path);
  OG_COREAPI void export_to_file(string _path);
  OG_COREAPI bool catchup_schema(string _path);
  OG_COREAPI bool reload_schema(string _path, string _sesn_path);
  OG_COREAPI boost::optional<og_session_object_ptr> import_object_from_file(string _path);

  OG_COREAPI void open(string _connection_string);
  OG_COREAPI void close();
  OG_COREAPI void purge();
  OG_COREAPI void build();
  // file io and management ends.

  ///////////////////////////////
  // object
  ///////////////////////////////
  OG_COREAPI og_session_object_ptr create_object(og_schema_object_ptr _schm_obj);
  OG_COREAPI void delete_object(string _id);

  OG_COREAPI optional<og_session_object_ptr> get_object(string _id);
  OG_COREAPI void get_object(list<og_session_object_ptr>* _sesn_obj_list);
  OG_COREAPI void get_object(list<string>& _oid_list,
                             list<og_session_object_ptr>* _sesn_obj_list);

  OG_COREAPI void get_object_by_type(list<string>& _otype_list,
                                     list<og_session_object_ptr>* _sesn_obj_list);

  OG_COREAPI void get_object_by_name(list<string>& _oname_list,
                                     list<og_session_object_ptr>* _sesn_obj_list);

  OG_COREAPI void get_object_by_schema_object_name(list<string>&
      _schm_obj_oname_list,
      list<og_session_object_ptr>* _sesn_obj_list);

  OG_COREAPI void get_object_by_schema_object(list<og_schema_object_ptr>&
      _schm_obj_list,
      list<og_session_object_ptr>* _sesn_obj_list);

  OG_COREAPI void get_object_by_parameter(string& _param_name,
                                          og::core::parameter_value_variant& _value,
                                          list<og_session_object_ptr>* _sesn_obj_list);
  OG_COREAPI void get_object_by_parameter_range(string& _param_name,
                                          og::core::parameter_value_variant& _value_min,
                                          og::core::parameter_value_variant& _value_max,
                                          list<og_session_object_ptr>* _sesn_obj_list);

  ///////////////////////////////
  // relation
  ///////////////////////////////
  OG_COREAPI void get_connected_object_from(string _to_id,
      list<string> _rel_type_list,
      list<og_session_object_ptr>* _sesn_obj_list);
  OG_COREAPI void get_connected_object_to(string _from_id,
                                          list<string> _rel_type_list,
                                          list<og_session_object_ptr>* _sesn_obj_list);
  OG_COREAPI og::core::connection_validation_result_enum validate_connect(
    string _from_id, string _to_id,
    list<string> _rel_type_list);

  OG_COREAPI boost::optional<og_session_relation_ptr> get_relation(
    string _rel_id);
  OG_COREAPI void get_relation(list<og_session_relation_ptr>* _sesn_rel_list);
  OG_COREAPI void get_relation(list<string> _rel_id_list,
                               list<og_session_relation_ptr>* _sesn_rel_list);
  OG_COREAPI void get_relation(string _from_id, string _to_id,
                               list<og_session_relation_ptr>* _sesn_rel_list);
  OG_COREAPI void get_relation_by_type(list<string> _rel_type_list,
                                       list<og_session_relation_ptr>* _sesn_rel_list);
  OG_COREAPI void get_relation_by_name(list<string> _rel_name_list,
                                       list<og_session_relation_ptr>* _sesn_rel_list);

  OG_COREAPI void disconnect(string _rel_id);
  // relation ends.

  OG_COREAPI og_session_object_ptr get_property_object();

  scoped_ptr<og::core::session> session_;
  scoped_ptr<og::og_schema> og_schema_;
};

}// namespace og
