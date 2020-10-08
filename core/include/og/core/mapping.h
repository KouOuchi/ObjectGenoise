#pragma once

#include <list>
#include <vector>
#include <string>
#include <map>

#include "og/core/session.h"
#include "og/core/schema.h"
#include "og/core/transaction.h"

#include "og/core/schema_object.h"
#include "og/core/schema_relation.h"
#include "og/core/schema_parameter.h"
#include "og/core/multiplicity.h"

#include "og/core/session_object.h"
#include "og/core/session_relation.h"

#include <boost/lexical_cast.hpp>

#include <soci/soci.h>

namespace soci
{

template<>
struct type_conversion<og::core::schema_object>
{
  typedef values base_type;

  static void from_base(values const & v, indicator /* ind */,
                        og::core::schema_object& _schm_obj)
  {
    _schm_obj.set_id( v.get<std::string>("id_") );
    _schm_obj.set_comment( v.get<std::string>("comment") );
    _schm_obj.set_name( v.get<std::string>("name") );
    _schm_obj.set_type( v.get<std::string>("type") );
    _schm_obj.set_revision( v.get<std::string>("revision") );
    _schm_obj.set_create_date( v.get<std::string>("create_date") );
    _schm_obj.set_update_date( v.get<std::string>("update_date") );
  }

  static void to_base(/*const*/ og::core::schema_object & _schm_obj, values & v,
                                indicator & ind)
  {
    v.set("id_", _schm_obj.get_id());
    v.set("comment", _schm_obj.get_comment());
    v.set("name", _schm_obj.get_name());
    v.set("type", _schm_obj.get_type());
    v.set("revision", _schm_obj.get_revision());
    v.set("create_date", _schm_obj.get_create_date());
    v.set("update_date", _schm_obj.get_update_date());

    ind = i_ok;
  }
};

template<>
struct type_conversion<og::core::schema_relation>
{
  typedef values base_type;

  static void from_base(values const & v, indicator /* ind */,
                        og::core::schema_relation& _schm_rel)
  {
    _schm_rel.set_id( v.get<std::string>("id_") );
    _schm_rel.set_from_id( v.get<std::string>("from_id") );
    _schm_rel.set_to_id( v.get<std::string>("to_id") );
    _schm_rel.set_comment( v.get<std::string>("comment") );
    _schm_rel.set_name( v.get<std::string>("name") );
    _schm_rel.set_type( v.get<std::string>("type") );
    _schm_rel.set_revision( v.get<std::string>("revision") );
    _schm_rel.set_create_date( v.get<std::string>("create_date") );
    _schm_rel.set_update_date( v.get<std::string>("update_date") );

	_schm_rel.set_from_multiplicity( og::core::multiplicity(v.get<int>("from_multiplicity_min"),
                                  v.get<int>("from_multiplicity_max")));
    _schm_rel.set_to_multiplicity( og::core::multiplicity(v.get<int>("to_multiplicity_min"),
                                  v.get<int>("to_multiplicity_max")));
  }

  static void to_base(/*const*/ og::core::schema_relation & _schm_rel, values & v,
                                indicator & ind)
  {
    v.set("id_", _schm_rel.get_id());
    v.set("from_id", _schm_rel.get_from_id());
    v.set("to_id", _schm_rel.get_to_id());
    v.set("comment", _schm_rel.get_comment());
    v.set("name", _schm_rel.get_name());
    v.set("type", _schm_rel.get_type());
    v.set("revision", _schm_rel.get_revision());
    v.set("create_date", _schm_rel.get_create_date());
    v.set("update_date", _schm_rel.get_update_date());

	v.set("from_multiplicity_min", _schm_rel.get_from_multiplicity().get_min());
	v.set("from_multiplicity_max", _schm_rel.get_from_multiplicity().get_max());
	v.set("to_multiplicity_min", _schm_rel.get_to_multiplicity().get_min());
	v.set("to_multiplicity_max", _schm_rel.get_to_multiplicity().get_max());

    ind = i_ok;
  }
};

template<>
struct type_conversion<og::core::schema_parameter>
{
  typedef values base_type;

  static void from_base(values const & v, indicator /* ind */,
                        og::core::schema_parameter& _schm_par)
  {
    _schm_par.set_id( v.get<std::string>("id_") );
    _schm_par.set_comment( v.get<std::string>("comment") );
    _schm_par.set_type( v.get<std::string>("type") );
    _schm_par.set_default_array_size(v.get<int>("default_array_size") );
    _schm_par.set_min_array_size( v.get<int>("min_array_size") );
    _schm_par.set_max_array_size( v.get<int>("max_array_size") );
    _schm_par.set_revision( v.get<std::string>("revision") );
    _schm_par.set_create_date( v.get<std::string>("create_date") );
    _schm_par.set_update_date( v.get<std::string>("update_date") );

    // not initialized
    if (_schm_par.basetype_variant_.which() == (int)
        og::core::parameter_basetype_enum::blank)
    {
      og::core::parameter_basetype_enum b =
        _schm_par.convert_to_parameter_basetype_enum(v.get<int>("basetype_name"));
      _schm_par.initialize_basetype(b, _schm_par.get_id());
    }
  }

  static void to_base(/*const*/ og::core::schema_parameter & _schm_par,
                                values & v,
                                indicator & ind)
  {
    v.set("id_", _schm_par.get_id());
    v.set("comment", _schm_par.get_comment());
    v.set("default_array_size", _schm_par.get_default_array_size());
    v.set("min_array_size", _schm_par.get_min_array_size());
    v.set("max_array_size", _schm_par.get_max_array_size());
    v.set("type", _schm_par.get_type());
    v.set("revision", _schm_par.get_revision());
    v.set("create_date", _schm_par.get_create_date());
    v.set("update_date", _schm_par.get_update_date());
    v.set("basetype_name", (int)(_schm_par.get_basetype()));

    ind = i_ok;
  }
};

template<>
struct type_conversion<og::core::parameter_basetype_integer>
{
  typedef values base_type;

  static void from_base(values const & v, indicator /* ind */,
                        og::core::parameter_basetype_integer& _param)
  {
    _param.id_ = v.get<std::string>("id_");
    _param.default_value_ = v.get<int>("default_value");
    _param.system_min_ = v.get<int>("system_min");
    _param.system_max_ = v.get<int>("system_max");
    _param.warn_min_ = v.get<int>("warn_min");
    _param.warn_max_ = v.get<int>("warn_max");
  }

  static void to_base(/*const*/ og::core::parameter_basetype_integer & _schm_par,
                                values & v, indicator & ind)
  {
    v.set("id_", _schm_par.id_);
    v.set("default_value", _schm_par.default_value_);
    v.set("system_min", _schm_par.system_min_);
    v.set("system_max", _schm_par.system_max_);
    v.set("warn_min", _schm_par.warn_min_);
    v.set("warn_max", _schm_par.warn_max_);

    ind = i_ok;
  }
};

template<>
struct type_conversion<og::core::parameter_basetype_real>
{
  typedef values base_type;

  static void from_base(values const & v, indicator /* ind */,
                        og::core::parameter_basetype_real& _param)
  {
    _param.id_ = v.get<std::string>("id_");
    _param.default_value_ = v.get<double>("default_value");
    _param.system_min_ = v.get<double>("system_min");
    _param.system_max_ = v.get<double>("system_max");
    _param.warn_min_ = v.get<double>("warn_min");
    _param.warn_max_ = v.get<double>("warn_max");
  }

  static void to_base(/*const*/ og::core::parameter_basetype_real & _schm_par,
                                values & v, indicator & ind)
  {
    v.set("id_", _schm_par.id_);
    v.set("default_value", _schm_par.default_value_);
    v.set("system_min", _schm_par.system_min_);
    v.set("system_max", _schm_par.system_max_);
    v.set("warn_min", _schm_par.warn_min_);
    v.set("warn_max", _schm_par.warn_max_);

    ind = i_ok;
  }
};

template<>
struct type_conversion<og::core::parameter_basetype_text>
{
  typedef values base_type;

  static void from_base(values const & v, indicator /* ind */,
                        og::core::parameter_basetype_text& _param)
  {
    _param.id_ = v.get<std::string>("id_");
    _param.default_value_ = v.get<std::string>("default_value");
    _param.system_min_ = v.get<int>("system_min");
    _param.system_max_ = v.get<int>("system_max");
    _param.warn_min_ = v.get<int>("warn_min");
    _param.warn_max_ = v.get<int>("warn_max");
  }

  static void to_base(/*const*/ og::core::parameter_basetype_text & _schm_par,
                                values & v, indicator & ind)
  {
    v.set("id_", _schm_par.id_);
    v.set("default_value", _schm_par.default_value_);
    v.set("system_min", _schm_par.system_min_);
    v.set("system_max", _schm_par.system_max_);
    v.set("warn_min", _schm_par.warn_min_);
    v.set("warn_max", _schm_par.warn_max_);

    ind = i_ok;
  }
};

template<>
struct type_conversion<og::core::schema_object_parameter>
{
  typedef values base_type;

  static void from_base(values const & v, indicator /* ind */,
                        og::core::schema_object_parameter& _schm_obj_prm)
  {
    _schm_obj_prm.schema_object_id_ = v.get<std::string>("schema_object_id");
    _schm_obj_prm.pid_ = v.get<std::string>("pid");
    _schm_obj_prm.param_name_ = v.get<std::string>("param_name");
//    _schm_obj_prm.comment_ = v.get<std::string>("comment");
  }

  static void to_base(/*const*/ og::core::schema_object_parameter & _schm_obj_prm,
                                values & v, indicator & ind)
  {
    v.set("schema_object_id", _schm_obj_prm.schema_object_id_);
    v.set("pid", _schm_obj_prm.pid_);
    v.set("param_name", _schm_obj_prm.param_name_);
//    v.set("comment", _schm_obj_prm.comment_);

    ind = i_ok;
  }
};

template<>
struct type_conversion<og::core::schema_relation_parameter>
{
  typedef values base_type;

  static void from_base(values const & v, indicator /* ind */,
                        og::core::schema_relation_parameter& _schm_rel_prm)
  {
    _schm_rel_prm.schema_relation_id_ = v.get<std::string>("schema_relation_id");
    _schm_rel_prm.pid_ = v.get<std::string>("pid");
    _schm_rel_prm.param_name_ = v.get<std::string>("param_name");
//    _schm_rel_prm.comment_ = v.get<std::string>("comment");
  }

  static void to_base(/*const*/ og::core::schema_relation_parameter &
                                _schm_obj_prm,
                                values & v, indicator & ind)
  {
    v.set("schema_relation_id", _schm_obj_prm.schema_relation_id_);
    v.set("pid", _schm_obj_prm.pid_);
    v.set("param_name", _schm_obj_prm.param_name_);
//    v.set("comment", _schm_obj_prm.comment_);

    ind = i_ok;
  }
};

template<>
struct type_conversion<og::core::session_object>
{
  typedef values base_type;

  static void from_base(values const & v, indicator /* ind */,
                        og::core::session_object& _schm_obj)
  {
    _schm_obj.set_id( v.get<std::string>("id_") );
    _schm_obj.set_schema_object_by_id( v.get<std::string>("schema_object_id") );
    _schm_obj.set_comment( v.get<std::string>("comment") );
    _schm_obj.set_name( v.get<std::string>("name") );
    _schm_obj.set_revision( v.get<std::string>("revision") );
    _schm_obj.set_create_date( v.get<std::string>("update_date") );
    _schm_obj.set_update_date( v.get<std::string>("update_date") );
  }

  static void to_base(/*const*/ og::core::session_object & _sesn_obj, values & v,
                                indicator & ind)
  {
    v.set("id_", _sesn_obj.get_id());
    v.set("schema_object_id", _sesn_obj.get_schema_object_id());
    v.set("comment", _sesn_obj.get_comment());
    v.set("name", _sesn_obj.get_name());
    v.set("revision", _sesn_obj.get_revision());
    v.set("create_date", _sesn_obj.get_create_date());
    v.set("update_date", _sesn_obj.get_update_date());

    ind = i_ok;
  }
};

template<>
struct type_conversion<og::core::session_relation>
{
  typedef values base_type;

  static void from_base(values const & v, indicator /* ind */,
                        og::core::session_relation& _schm_rel)
  {
    _schm_rel.set_id( v.get<std::string>("id_") );
    _schm_rel.set_schema_relation_by_id( v.get<std::string>("schema_relation_id") );

    _schm_rel.set_from_id( v.get<std::string>("from_id") );
    _schm_rel.set_to_id( v.get<std::string>("to_id") );

    _schm_rel.set_comment( v.get<std::string>("comment") );
    _schm_rel.set_name( v.get<std::string>("name") );
    _schm_rel.set_revision( v.get<std::string>("revision") );
    _schm_rel.set_create_date( v.get<std::string>("update_date") );
    _schm_rel.set_update_date( v.get<std::string>("update_date") );
  }

  static void to_base(/*const*/ og::core::session_relation & _sesn_rel,
                                values & v,
                                indicator & ind)
  {
    v.set("id_", _sesn_rel.get_id());
    v.set("schema_relation_id", _sesn_rel.get_schema_relation_id());

    v.set("from_id", _sesn_rel.get_from_id());
    v.set("to_id", _sesn_rel.get_to_id());

    v.set("comment", _sesn_rel.get_comment());
    v.set("name", _sesn_rel.get_name());
    v.set("revision", _sesn_rel.get_revision());
    v.set("create_date", _sesn_rel.get_create_date());
    v.set("update_date", _sesn_rel.get_update_date());

    ind = i_ok;
  }
};


} // namespace soci

