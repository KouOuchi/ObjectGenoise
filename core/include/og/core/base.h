#pragma once

#include "og/core/og.h"
#include "og/core/exception.h"

#define BATCH_SIZE 100
#define REVISION_INIT_VALUE "0"

#define SCHEMA_OBJECT_ID_PREFIX "SCHM_O_"
#define SCHEMA_RELATION_ID_PREFIX "SHCM_R_"
#define SCHEMA_PARAMETER_ID_PREFIX "SCHM_P_"
#define SESSION_OBJECT_ID_PREFIX "SESN_O_"
#define SESSION_RELATION_ID_PREFIX "SESN_R_"

namespace og
{
namespace core
{
//class parameter_basetype_boolean;
class parameter_basetype_integer;
class parameter_basetype_real;
class parameter_basetype_text;
//class parameter_basetype_select;

class schema_object_parameter;
class schema_relation_parameter;
class schema_object;
class schema_relation;
class schema_parameter;
class session_object;
class session_relation;

/*template <typename T>*/
class session_parameter;

typedef shared_ptr<schema_object> schema_object_ptr;
typedef shared_ptr<schema_relation> schema_relation_ptr;
typedef shared_ptr<schema_parameter> schema_parameter_ptr;

typedef shared_ptr<session_object> session_object_ptr;
typedef shared_ptr<session_relation> session_relation_ptr;
typedef shared_ptr<session_parameter> session_parameter_ptr;

class schema;

typedef boost::variant <
boost::blank,
      parameter_basetype_integer,
      parameter_basetype_real,
      parameter_basetype_text
//parameter_basetype_select
      > parameter_basetype_variant;

class base
{

public:
  base()
    : auto_revision_up_(false), auto_sync_(false)
  { }

  virtual ~base() { }

  // <--- parameter
  virtual string get_id() { return id_; }
  virtual string get_comment() { return comment_; }
  virtual string get_name() { return name_; }
  virtual string get_type() { return type_; }
  virtual string get_revision() { return revision_; }
  virtual string get_create_date()
  {
    return pt::to_simple_string(pt::ptime_from_tm(create_date_));
  }
  virtual string get_update_date()
  {
    return pt::to_simple_string(pt::ptime_from_tm(update_date_));
  }

  virtual void set_id(string _oid)
  {
    id_ = _oid;
    set_updated();
  }
  virtual void set_comment(string _comment)
  {
    comment_ = _comment;
    set_updated();
  }
  virtual void set_name(string _oname)
  {
    name_ = _oname;
    set_updated();
  }
  virtual void set_type(string _otype)
  {
    type_ = _otype;
    set_updated();
  }
  virtual void set_revision(string _rev)
  {
    try
    {
      long r = boost::lexical_cast<long>(_rev);
      revision_ = _rev;
      set_updated();
    }
    catch (...)
    {
      OG_LOG << "Error. Revision must be long value:" << _rev;
    }
  }
  virtual void set_create_date(string _date)
  {
    create_date_ = pt::to_tm(pt::time_from_string(_date));
  }
  virtual void set_update_date(string _date)
  {
    update_date_ = pt::to_tm(pt::time_from_string(_date));
  }

  void set_updated()
  {
    update_date_ = pt::to_tm(pt::microsec_clock::local_time());
  }

  void set_auto_revision_up(bool _s)
  {
    auto_revision_up_ = _s;
  }
  bool get_auto_revision_up() { return auto_revision_up_; }
  void set_auto_sync(bool _s)
  {
    auto_sync_ = _s;
  }
  bool get_auto_sync() { return auto_sync_; }

  virtual void sync() = 0;
  virtual void revision_up() = 0;

  void sync_auto()
  {
    if (auto_sync_) { sync(); }
  }
  void revision_up_auto()
  {
    if (auto_revision_up_) { revision_up(); }
  }

protected:

  string id_;
  string comment_;
  string name_;
  string type_;
  string revision_;
  tm create_date_;
  tm update_date_;

  bool auto_revision_up_;
  bool auto_sync_;
};

} //namespace core;
} //namespace og;
