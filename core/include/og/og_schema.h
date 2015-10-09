#pragma once
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>
#include <list>

#include "og/core/og.h"
#include "og/og_schema_parameter.h"

namespace og
{
namespace core
{
class schema;
//class schema_relation;

class parameter_basetype_integer;
class parameter_basetype_real;
class parameter_basetype_text;
}
using namespace boost;
using namespace std;

class og_schema_object;
class og_schema_relation;
class og_schema_parameter;
class og_session_object;
class og_session_relation;
class og_session_parameter;

typedef boost::shared_ptr<og_schema_object> og_schema_object_ptr;
typedef boost::shared_ptr<og_schema_relation> og_schema_relation_ptr;
typedef boost::shared_ptr<og_schema_parameter> og_schema_parameter_ptr;

typedef boost::shared_ptr<og_session_object> og_session_object_ptr;
typedef boost::shared_ptr<og_session_relation> og_session_relation_ptr;
typedef boost::shared_ptr<og_session_parameter> og_session_parameter_ptr;

typedef og::core::parameter_basetype_integer og_integer;
typedef og::core::parameter_basetype_real og_real;
typedef og::core::parameter_basetype_text og_text;
typedef og::core::parameter_basetype_integer og_bool;

class og_schema
{
public:
  OG_COREAPI og_schema(og::core::schema* _schema);
  OG_COREAPI virtual ~og_schema();

  ///////////////////////////////
  // object and its relation
  ///////////////////////////////
  OG_COREAPI og_schema_object_ptr create_object(string _otype, string _oname);

  OG_COREAPI void get_object(list<string>& _oid_list,
                  list<og_schema_object_ptr>* _schm_obj_list);
  OG_COREAPI void get_object(list<og_schema_object_ptr>* _schm_obj_list);
  OG_COREAPI void delete_object(string _oid);
  OG_COREAPI boost::optional<og_schema_object_ptr> get_object(string _oid);
  OG_COREAPI void get_object_by_type(list<string>& _otype_list,
                          list<og_schema_object_ptr>* _schm_obj_list);
  OG_COREAPI void get_object_by_name(list<string>& _name_list,
                          list<og_schema_object_ptr>* _schm_obj_list);
  OG_COREAPI void get_object_type(list<string>* _type_list);

  OG_COREAPI void get_relation_by_type(list<string>& _otype_list,
                            list<og_schema_relation_ptr>* _schm_rel_list);
  OG_COREAPI void get_relation(list<string>& _relid_list,
                    list<og_schema_relation_ptr>* _schm_rel_list);
  OG_COREAPI void get_relation(list<og_schema_relation_ptr>* _schm_rel_list);
  OG_COREAPI optional<og_schema_relation_ptr> get_relation(string _relid);
  OG_COREAPI void get_relation_type(list<string>* _rel_type_list);

  OG_COREAPI void get_connected_object_to(string _from_o_id, list<string>& _rel_type_list,
                               list<og_schema_object_ptr>* _schm_object_list);
  OG_COREAPI void get_connected_object_from(string _to_o_id, list<string>& _rel_type_list,
                                 list<og_schema_object_ptr>* _schm_object_list);

  OG_COREAPI void get_connected_relation_to(string _from_o_id, list<string>& _rel_type_list,
                                 list<og_schema_relation_ptr>* _schm_rel_list);
  OG_COREAPI void get_connected_relation_from(string _to_o_id, list<string>& _rel_type_list,
                                   list<og_schema_relation_ptr>* _schm_rel_list);

  OG_COREAPI bool validate_connect_by_type(string _otype_from, string _otype_to);
  OG_COREAPI bool validate_connect_by_type_id(string _otype, string _oid_from,
                                   string _oid_to);
  //object and its relation ends.

  OG_COREAPI static const char* schema_property_object_type();
  OG_COREAPI static const char* schema_property_object_name();
  OG_COREAPI static const char* schema_property_core_revision();

  ///////////////////////////////
  // parameters
  ///////////////////////////////
  template <typename P>
  og_schema_parameter_ptr create_parameter(string _param_type,
      string _comment, P* _basetype)
  {
    og::core::schema_parameter_ptr p =
      schema_->create_parameter(_param_type, _comment, _basetype);

    return og_schema_parameter_ptr(new og_schema_parameter(p));
  }

  template <typename P>
  og_schema_parameter_ptr create_parameter(string _param_type,
      string _comment, P* _basetype, int _default_array_size, int _min_array_size,
      int _max_array_size)
  {
    og::core::schema_parameter_ptr p =
      schema_->create_parameter(_param_type, _comment, _basetype, _default_array_size,
                                _min_array_size, _max_array_size);

    return og_schema_parameter_ptr(new og_schema_parameter(p));
  }
  // parameters ends.

  ///////////////////////////////
  // file io and management
  ///////////////////////////////
  OG_COREAPI void export_to_file(string _path);
  OG_COREAPI bool import_from_file(string _path);
  OG_COREAPI void purge();

  // file io and management end.

private:
  og::core::schema* schema_;

};

}// namespace og
