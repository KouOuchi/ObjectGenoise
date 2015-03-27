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
  og_session();
  virtual ~og_session();

  ///////////////////////////////
  // schema
  ///////////////////////////////
  og_schema* schema();

  ///////////////////////////////
  // file io and management
  ///////////////////////////////
  bool import_from_file(string _path);
  void export_to_file(string _path);

  void connect(string _connection_string);
  void purge();
  void build();
  // file io and management ends.

  ///////////////////////////////
  // object
  ///////////////////////////////
  og_session_object_ptr create_object(og_schema_object_ptr _schm_obj);
  void delete_object(string _id);

  optional<og_session_object_ptr> get_object(string _id);
  void get_object(list<og_session_object_ptr>* _sesn_obj_list);
  void get_object(list<string>& _oid_list,
                  list<og_session_object_ptr>* _sesn_obj_list);

  void get_object_by_type(list<string>& _otype_list,
                          list<og_session_object_ptr>* _sesn_obj_list);

  void get_object_by_name(list<string>& _oname_list,
                          list<og_session_object_ptr>* _sesn_obj_list);

  void get_object_by_schema_object_name(list<string>& _schm_obj_oname_list,
                                        list<og_session_object_ptr>* _sesn_obj_list);

  void get_object_by_schema_object(list<og_schema_object_ptr>& _schm_obj_list,
                                   list<og_session_object_ptr>* _sesn_obj_list);

  ///////////////////////////////
  // relation
  ///////////////////////////////
  void get_connected_object_from(string _to_id, list<string> _rel_type_list,
                                 list<og_session_object_ptr>* _sesn_obj_list);
  void get_connected_object_to(string _from_id, list<string> _rel_type_list,
                               list<og_session_object_ptr>* _sesn_obj_list);
  bool validate_connect(string _from_id, string _to_id,
                        list<string> _rel_type_list);

  boost::optional<og_session_relation_ptr> get_relation(string _rel_id);
  void get_relation(list<og_session_relation_ptr>* _sesn_rel_list);
  void get_relation(list<string> _rel_id_list,
                    list<og_session_relation_ptr>* _sesn_rel_list);
  void get_relation(string _from_id, string _to_id,
                    list<og_session_relation_ptr>* _sesn_rel_list);
  void get_relation_by_type(list<string> _rel_type_list,
                            list<og_session_relation_ptr>* _sesn_rel_list);
  void get_relation_by_name(list<string> _rel_name_list,
                            list<og_session_relation_ptr>* _sesn_rel_list);

  void disconnect(string _rel_id);
  // relation ends.

  scoped_ptr<og::core::session> session_;
  scoped_ptr<og::og_schema> og_schema_;
};

}// namespace og
