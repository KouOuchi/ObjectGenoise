#pragma once

#include <list>
#include <vector>
#include <string>
#include <map>

#include "og/core/schema.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/utility.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

using namespace boost::property_tree;
using namespace boost::property_tree::xml_parser;

namespace og
{
namespace core
{

template<typename T>
struct serializer
{
  static void serialize(ptree& _src, T _tgt)
  {}
  static bool deserialize(const ptree::value_type& _tgt, T _src)
  {}
};

template<>
struct serializer <session_object_ptr>
{
  static void serialize(ptree& _pt,
                        session_object_ptr _s)
  {
    ptree& child = _pt.add("objects.object", "");

    child.put("<xmlattr>.id", _s->get_id());
    child.put("<xmlattr>.schema_object_id", _s->get_schema_object_id());
    child.put("<xmlattr>.type", _s->get_type());
    child.put("<xmlattr>.name", _s->get_name());
    child.put("<xmlattr>.comment", _s->get_comment());
    child.put("<xmlattr>.revision", _s->get_revision());
    child.put("<xmlattr>.update_date", _s->get_update_date());
    child.put("<xmlattr>.create_date", _s->get_create_date());

    _s->get_session()->export_parameter_to_file
    (_s->get_schema_object(), _s, child);
  }
  static bool deserialize(const ptree::value_type& _pt,
                          session_object_ptr _base)
  {

    optional<string> oid = _pt.second.get_optional<string>("<xmlattr>.id");
    optional<string> soid =
      _pt.second.get_optional<string>("<xmlattr>.schema_object_id");
    optional<string> type = _pt.second.get_optional<string>("<xmlattr>.type");
    optional<string> name = _pt.second.get_optional<string>("<xmlattr>.name");
    optional<string> revision =
      _pt.second.get_optional<string>("<xmlattr>.revision");
    optional<string> comment =
      _pt.second.get_optional<string>("<xmlattr>.comment");
    optional<string> update_date =
      _pt.second.get_optional<string>("<xmlattr>.update_date");
    optional<string> create_date =
      _pt.second.get_optional<string>("<xmlattr>.create_date");

    if (oid.is_initialized() && soid.is_initialized() && type.is_initialized()
        && name.is_initialized() && revision.is_initialized())
    {
      _base->set_id(oid.get());
	  _base->set_schema_object_by_id(soid.get());
      _base->set_type(type.get());
      _base->set_name(name.get());
      _base->set_revision(revision.get());
      if (comment) { _base->set_comment(comment.get()); }
      if (update_date) { _base->set_update_date(update_date.get()); }
      if (create_date) { _base->set_create_date(create_date.get()); }

      return true;
    }
    else
    {
      return false;
    }

  }
};

template<>
struct serializer <session_relation_ptr>
{
  static void serialize(ptree& _pt,
                        session_relation_ptr _s)
  {
    ptree& child = _pt.add("relations.relation", "");

    child.put("<xmlattr>.id", _s->get_id());
	child.put("<xmlattr>.schema_relation_id", _s->get_schema_relation_id());
    child.put("<xmlattr>.name", _s->get_name());
    child.put("<xmlattr>.from_id", _s->get_from_id());
    child.put("<xmlattr>.to_id", _s->get_to_id());
    child.put("<xmlattr>.comment", _s->get_comment());
    child.put("<xmlattr>.revision", _s->get_revision());
    child.put("<xmlattr>.update_date", _s->get_update_date());
    child.put("<xmlattr>.create_date", _s->get_create_date());

    _s->get_session()->export_parameter_to_file
    (_s->get_schema_relation(), _s, child);

  }
  static bool deserialize(const ptree::value_type& _pt,
                          session_relation_ptr _base)
  {

    optional<string> relid = _pt.second.get_optional<string>("<xmlattr>.id");
    optional<string> srelid =
      _pt.second.get_optional<string>("<xmlattr>.schema_relation_id");
    optional<string> name = _pt.second.get_optional<string>("<xmlattr>.name");
    optional<string> from_id =
      _pt.second.get_optional<string>("<xmlattr>.from_id");
    optional<string> to_id = _pt.second.get_optional<string>("<xmlattr>.to_id");
    optional<string> revision =
      _pt.second.get_optional<string>("<xmlattr>.revision");
    optional<string> comment =
      _pt.second.get_optional<string>("<xmlattr>.comment");
    optional<string> update_date =
      _pt.second.get_optional<string>("<xmlattr>.update_date");
    optional<string> create_date =
      _pt.second.get_optional<string>("<xmlattr>.create_date");

    if (relid.is_initialized() && srelid.is_initialized() 
        && name.is_initialized() && from_id.is_initialized() && to_id.is_initialized()
        && revision.is_initialized() && update_date.is_initialized()
        && create_date.is_initialized())
    {
      _base->set_id(relid.get());
	  _base->set_schema_relation_by_id(srelid.get());

      _base->set_from_id(from_id.get());
      _base->set_to_id(to_id.get());

	  _base->set_name(name.get());
      _base->set_revision(revision.get());
      _base->set_comment(comment.is_initialized() ? comment.get() : string(""));
      _base->set_update_date(update_date.get());
      _base->set_create_date(create_date.get());
      return true;
    }
    else
    {
      return false;
    }

  }
};

template<>
struct serializer < schema_object_parameter* >
{

  static void serialize(ptree& _pt,
                        schema_object_parameter* _base)
  {
    ptree& child = _pt.add("schema_object_parameter", "");
    child.put("<xmlattr>.schema_object_id", _base->schema_object_id_);
    child.put("<xmlattr>.pid", _base->pid_);
    child.put("<xmlattr>.param_name", _base->param_name_);
  }
  static bool deserialize(const ptree::value_type& _pt,
                          schema_object_parameter* _base)
  {
    if (!_pt.second.get_optional<string>("schema_object_parameter").is_initialized())
    {
      return false;
    }

    optional<string> sid =
      _pt.second.get_optional<string>("<xmlattr>.schema_object_id");
    optional<string> pid = _pt.second.get_optional<string>("<xmlattr>.pid");
    optional<string> pname =
      _pt.second.get_optional<string>("<xmlattr>.param_name");


    if (sid.is_initialized() && pid.is_initialized()
        && pname.is_initialized())
    {
      _base->schema_object_id_ = sid.get();
      _base->pid_ = pid.get();
      _base->param_name_ = pname.get();

      return true;
    }
    else
    {
      return false;
    }
  }
};

template<>
struct serializer < schema_relation_parameter* >
{
  static void serialize(ptree& _pt,
                        schema_relation_parameter* _base)
  {
    ptree& child = _pt.add("schema_relation_parameter", "");
    child.put("<xmlattr>.schema_relation_id", _base->schema_relation_id_);
    child.put("<xmlattr>.pid", _base->pid_);
    child.put("<xmlattr>.param_name", _base->param_name_);
  }

  static bool deserialize(const ptree::value_type& _pt,
                          schema_relation_parameter* _base)
  {
    if (!_pt.second.get_optional<string>("schema_relation_parameter").is_initialized())
    {
      return false;
    }

    optional<string> sid =
      _pt.second.get_optional<string>("<xmlattr>.schema_relation_id");
    optional<string> pid = _pt.second.get_optional<string>("<xmlattr>.pid");
    optional<string> pname =
      _pt.second.get_optional<string>("<xmlattr>.param_name");

    if (sid.is_initialized() && pid.is_initialized()
        && pname.is_initialized())
    {
      _base->schema_relation_id_ = sid.get();
      _base->pid_ = pid.get();
      _base->param_name_ = pname.get();

      return true;
    }
    else
    {
      return false;
    }
  }
};

template<>
struct serializer<parameter_basetype_integer*>
{

  static void serialize(ptree& _pt,
                        parameter_basetype_integer* _base)
  {
    ptree& child = _pt.add("parameter_basetype_integer", "");
    child.put("<xmlattr>.default_value", _base->default_value_);
    child.put("<xmlattr>.system_min", _base->system_min_);
    child.put("<xmlattr>.system_max", _base->system_max_);
    child.put("<xmlattr>.warn_min", _base->warn_min_);
    child.put("<xmlattr>.warn_max", _base->warn_max_);
  }
  static bool deserialize(const ptree::value_type& _pt,
                          parameter_basetype_integer* _base)
  {
    if (!_pt.second.get_optional<string>("parameter_basetype_integer").is_initialized())
    {
      return false;
    }

    optional<string> def =
      _pt.second.get_optional<string>("<xmlattr>.default_value");
    optional<long> sys_min = _pt.second.get_optional<long>("<xmlattr>.system_min");
    optional<long> sys_max = _pt.second.get_optional<long>("<xmlattr>.system_max");
    optional<long> war_min = _pt.second.get_optional<long>("<xmlattr>.warn_min");
    optional<long> war_max = _pt.second.get_optional<long>("<xmlattr>.warn_max");

    if (def.is_initialized() && sys_min.is_initialized()
        && sys_max.is_initialized() && war_min.is_initialized()
        && war_max.is_initialized())
    {
      _base->id_ = def.get();
      _base->system_min_ = sys_min.get();
      _base->system_max_ = sys_max.get();
      _base->warn_min_ = war_min.get();
      _base->warn_max_ = war_max.get();

      return true;
    }
    else
    {
      return false;
    }
  }
};

template<>
struct serializer<parameter_basetype_real*>
{
  static void serialize(ptree& _pt,
                        parameter_basetype_real* _base)
  {
    ptree& child = _pt.add("parameter_basetype_real", "");
    child.put("<xmlattr>.default_value", _base->default_value_);
    child.put("<xmlattr>.system_min", _base->system_min_);
    child.put("<xmlattr>.system_max", _base->system_max_);
    child.put("<xmlattr>.warn_min", _base->warn_min_);
    child.put("<xmlattr>.warn_max", _base->warn_max_);
  }

  static bool deserialize(const ptree::value_type& _pt,
                          parameter_basetype_real* _base)
  {
    if (!_pt.second.get_optional<string>("parameter_basetype_real").is_initialized())
    {
      return false;
    }

    optional<string> def =
      _pt.second.get_optional<string>("<xmlattr>.default_value");
    optional<double> sys_min =
      _pt.second.get_optional<double>("<xmlattr>.system_min");
    optional<double> sys_max =
      _pt.second.get_optional<double>("<xmlattr>.system_max");
    optional<double> war_min =
      _pt.second.get_optional<double>("<xmlattr>.warn_min");
    optional<double> war_max =
      _pt.second.get_optional<double>("<xmlattr>.warn_max");

    if (def.is_initialized() && sys_min.is_initialized()
        && sys_max.is_initialized() && war_min.is_initialized()
        && war_max.is_initialized())
    {
      _base->id_ = def.get();
      _base->system_min_ = sys_min.get();
      _base->system_max_ = sys_max.get();
      _base->warn_min_ = war_min.get();
      _base->warn_max_ = war_max.get();

      return true;
    }
    else
    {
      return false;
    }
  }
};

template<>
struct serializer<parameter_basetype_text*>
{
  static void serialize(ptree& _pt,
                        parameter_basetype_text* _base)
  {
    ptree& child = _pt.add("parameter_basetype_text", "");
    child.put("<xmlattr>.default_value", _base->default_value_);
    child.put("<xmlattr>.system_min", _base->system_min_);
    child.put("<xmlattr>.system_max", _base->system_max_);
    child.put("<xmlattr>.warn_min", _base->warn_min_);
    child.put("<xmlattr>.warn_max", _base->warn_max_);
  }

  static bool deserialize(const ptree::value_type& _pt,
                          parameter_basetype_text* _base)
  {
    if (!_pt.second.get_optional<string>("parameter_basetype_real").is_initialized())
    {
      return false;
    }

    optional<string> def =
      _pt.second.get_optional<string>("<xmlattr>.default_value");
    optional<long> sys_min = _pt.second.get_optional<long>("<xmlattr>.system_min");
    optional<long> sys_max = _pt.second.get_optional<long>("<xmlattr>.system_max");
    optional<long> war_min = _pt.second.get_optional<long>("<xmlattr>.warn_min");
    optional<long> war_max = _pt.second.get_optional<long>("<xmlattr>.warn_max");

    if (def.is_initialized() && sys_min.is_initialized()
        && sys_max.is_initialized() && war_min.is_initialized()
        && war_max.is_initialized())
    {
      _base->id_ = def.get();
      _base->system_min_ = sys_min.get();
      _base->system_max_ = sys_max.get();
      _base->warn_min_ = war_min.get();
      _base->warn_max_ = war_max.get();

      return true;
    }
    else
    {
      return false;
    }
  }
};

template<>
struct serializer<schema_parameter_ptr>
{
  static void serialize(ptree& _pt, schema_parameter_ptr _schm_par)
  {
    ptree& child = _pt.add("parameters.parameter", "");

    child.put("<xmlattr>.id", _schm_par->get_id());
    child.put("<xmlattr>.type", _schm_par->get_type());
    child.put("<xmlattr>.default_array_size", _schm_par->get_default_array_size());
    child.put("<xmlattr>.min_array_size", _schm_par->get_min_array_size());
    child.put("<xmlattr>.max_array_size", _schm_par->get_max_array_size());
    child.put("<xmlattr>.basetype_name", _schm_par->get_basetype());

    child.put("<xmlattr>.revision", _schm_par->get_revision());
    child.put("<xmlattr>.comment", _schm_par->get_comment());
    child.put("<xmlattr>.update_date", _schm_par->get_update_date());
    child.put("<xmlattr>.create_date", _schm_par->get_create_date());

    export_parameter(_schm_par, child);
  }
  static void export_parameter(schema_parameter_ptr _schm_par,
                               ptree& _pt)
  {
    parameter_basetype_enum t = (parameter_basetype_enum)_schm_par->get_basetype();

    switch (t)
    {
    case parameter_basetype_enum::integer:
    {
      parameter_basetype_integer* p =
        boost::get<parameter_basetype_integer>(&_schm_par->basetype_variant_);

      // serialize
      serializer<parameter_basetype_integer*>::serialize(_pt, p);
    }
    break;

    case parameter_basetype_enum::real:
    {
      parameter_basetype_real* p =
        boost::get<parameter_basetype_real>(&_schm_par->basetype_variant_);

      // serialize
      serializer<parameter_basetype_real*>::serialize(_pt, p);
    }
    break;
    case parameter_basetype_enum::text:
    {
      parameter_basetype_text* p =
        boost::get<parameter_basetype_text>(&_schm_par->basetype_variant_);

      // serialize
      serializer<parameter_basetype_text*>::serialize(_pt, p);
    }
    break;
    }
  }

  static bool deserialize(const ptree::value_type& _pt,
                          schema_parameter_ptr _schm_par)
  {
    optional<string> pid = _pt.second.get_optional<string>("<xmlattr>.id");
    optional<string> type = _pt.second.get_optional<string>("<xmlattr>.type");
    optional<int> default_array_size =
      _pt.second.get_optional<int>("<xmlattr>.default_array_size");
    optional<int> min_array_size =
      _pt.second.get_optional<int>("<xmlattr>.min_array_size");
    optional<int> max_array_size =
      _pt.second.get_optional<int>("<xmlattr>.max_array_size");
    optional<int> basetype_name =
      _pt.second.get_optional<int>("<xmlattr>.basetype_name");

    optional<string> revision =
      _pt.second.get_optional<string>("<xmlattr>.revision");
    optional<string> comment =
      _pt.second.get_optional<string>("<xmlattr>.comment");
    optional<string> update_date =
      _pt.second.get_optional<string>("<xmlattr>.update_date");
    optional<string> create_date =
      _pt.second.get_optional<string>("<xmlattr>.create_date");

    if (pid.is_initialized() && type.is_initialized() &&
        default_array_size.is_initialized() &&
        min_array_size.is_initialized() && max_array_size.is_initialized() &&
        basetype_name.is_initialized() &&
        revision.is_initialized() && update_date.is_initialized() &&
        create_date.is_initialized())
    {
      ;
    }
    else
    {
      return false;
    }

    _schm_par->set_id(pid.get());
    _schm_par->set_type(type.get());

    _schm_par->initialize_basetype((parameter_basetype_enum)(basetype_name.get()),
                                   pid.get());

    _schm_par->set_default_array_size(default_array_size.get());
    _schm_par->set_min_array_size(min_array_size.get());
    _schm_par->set_max_array_size(max_array_size.get());

    _schm_par->set_revision(revision.get());
    _schm_par->set_comment(comment.is_initialized() ? comment.get() : string(""));
    _schm_par->set_update_date(update_date.get());
    _schm_par->set_create_date(create_date.get());

    if(!import_parameter(_schm_par, _pt))
    {
      return false;
    }

    return true;
  }

  static bool import_parameter(schema_parameter_ptr _schm_par,
                               const ptree::value_type& _pt)
  {
    parameter_basetype_enum t = (parameter_basetype_enum)_schm_par->get_basetype();

    switch (t)
    {
    case parameter_basetype_enum::integer:
    {
      parameter_basetype_integer* p =
        boost::get<parameter_basetype_integer>(&_schm_par->basetype_variant_);

      // deserialize
      if (serializer<parameter_basetype_integer*>::deserialize(_pt, p))
      {
        return true;
      }
    }
    break;

    case parameter_basetype_enum::real:
    {
      parameter_basetype_real* p =
        boost::get<parameter_basetype_real>(&_schm_par->basetype_variant_);

      // deserialize
      if (serializer<parameter_basetype_real*>::deserialize(_pt, p))
      {
        return true;
      }
    }
    break;

    case parameter_basetype_enum::text:
    {
      parameter_basetype_text* p =
        boost::get<parameter_basetype_text>(&_schm_par->basetype_variant_);
      // deserialize
      parameter_basetype_text base_t;
      if (serializer<parameter_basetype_text*>::deserialize(_pt, p))
      {
        return true;
      }
    }
    break;
    }

    return false;
  }

};

template<>
struct serializer<schema_object_ptr>
{
  static void serialize(ptree& _pt,
                        schema_object_ptr _schm_obj)
  {
    ptree& child = _pt.add("objects.object", "");

    child.put("<xmlattr>.id", _schm_obj->get_id());
    child.put("<xmlattr>.type", _schm_obj->get_type());
    child.put("<xmlattr>.name", _schm_obj->get_type());
    child.put("<xmlattr>.revision", _schm_obj->get_revision());
    child.put("<xmlattr>.comment", _schm_obj->get_comment());
    child.put("<xmlattr>.update_date", _schm_obj->get_update_date());
    child.put("<xmlattr>.create_date", _schm_obj->get_create_date());

    list < boost::tuple<string, schema_parameter_ptr> >
    param_name_types;
    _schm_obj->get_parameters(&param_name_types);

    for (list<boost::tuple<string, schema_parameter_ptr>>::iterator pit
         = param_name_types.begin(); pit != param_name_types.end(); pit++)
    {
      schema_object_parameter p(_schm_obj->get_id(),
                                pit->get<1>()->get_id(),
                                pit->get<0>());

      serializer<schema_object_parameter*>::serialize(child, &p);
    }
  }
  static bool deserialize(const ptree::value_type& _pt,
                          schema_object_ptr _schm_obj)
  {
    optional<string> oid = _pt.second.get_optional<string>("<xmlattr>.id");
    optional<string> type = _pt.second.get_optional<string>("<xmlattr>.type");
    optional<string> name = _pt.second.get_optional<string>("<xmlattr>.name");
    optional<string> revision =
      _pt.second.get_optional<string>("<xmlattr>.revision");
    optional<string> comment =
      _pt.second.get_optional<string>("<xmlattr>.comment");
    optional<string> update_date =
      _pt.second.get_optional<string>("<xmlattr>.update_date");
    optional<string> create_date =
      _pt.second.get_optional<string>("<xmlattr>.create_date");

    if (oid.is_initialized() && type.is_initialized() && revision.is_initialized()
        && update_date.is_initialized() && create_date.is_initialized())
    {
      _schm_obj->set_id(oid.get());
      _schm_obj->set_type(type.get());
      _schm_obj->set_name(name.get());
      _schm_obj->set_revision(revision.get());
      _schm_obj->set_comment(comment.is_initialized() ? comment.get() : string(""));
      _schm_obj->set_update_date(update_date.get());
      _schm_obj->set_create_date(create_date.get());

      return true;
    }
    else
    {
      return false;
    }
  }
};

template<>
struct serializer<schema_relation_ptr>
{
  static void serialize(ptree& _pt,
                        schema_relation_ptr _schm_rel)
  {
    ptree& child = _pt.add("relations.relation", "");

    child.put("<xmlattr>.id", _schm_rel->get_id());
    child.put("<xmlattr>.type", _schm_rel->get_type());
    child.put("<xmlattr>.name", _schm_rel->get_name());
    child.put("<xmlattr>.from_id", _schm_rel->get_from_id());
    child.put("<xmlattr>.to_id", _schm_rel->get_to_id());
    child.put("<xmlattr>.revision", _schm_rel->get_revision());
    child.put("<xmlattr>.comment", _schm_rel->get_comment());
    child.put("<xmlattr>.update_date", _schm_rel->get_update_date());
    child.put("<xmlattr>.create_date", _schm_rel->get_create_date());

    child.put("<xmlattr>.from_max_multiplicity",
              _schm_rel->get_from_multiplicity().get_max());
    child.put("<xmlattr>.from_min_multiplicity",
              _schm_rel->get_from_multiplicity().get_min());
    child.put("<xmlattr>.to_max_multiplicity",
              _schm_rel->get_to_multiplicity().get_max());
    child.put("<xmlattr>.to_min_multiplicity",
              _schm_rel->get_to_multiplicity().get_min());

    list < boost::tuple<string, schema_parameter_ptr> >
    param_name_types;
    _schm_rel->get_parameters(&param_name_types);

    for (list<boost::tuple<string, schema_parameter_ptr>>::iterator pit
         = param_name_types.begin(); pit != param_name_types.end(); pit++)
    {
      schema_relation_parameter p(_schm_rel->get_id(),
                                  pit->get<1>()->get_id(),
                                  pit->get<0>());

      serializer<schema_relation_parameter*>::serialize(child, &p);
    }

  }
  static bool deserialize(const ptree::value_type& _pt,
                          schema_relation_ptr _schm_rel)
  {
    optional<string> relid = _pt.second.get_optional<string>("<xmlattr>.id");
    optional<string> type = _pt.second.get_optional<string>("<xmlattr>.type");
    optional<string> name = _pt.second.get_optional<string>("<xmlattr>.name");
    optional<string> from_id =
      _pt.second.get_optional<string>("<xmlattr>.from_id");
    optional<string> to_id = _pt.second.get_optional<string>("<xmlattr>.to_id");
    optional<string> revision =
      _pt.second.get_optional<string>("<xmlattr>.revision");
    optional<string> comment =
      _pt.second.get_optional<string>("<xmlattr>.comment");
    optional<string> update_date =
      _pt.second.get_optional<string>("<xmlattr>.update_date");
    optional<string> create_date =
      _pt.second.get_optional<string>("<xmlattr>.create_date");

    optional<int> from_max_multiplicity =
      _pt.second.get_optional<int>("<xmlattr>.from_max_multiplicity");
    optional<int> from_min_multiplicity =
      _pt.second.get_optional<int>("<xmlattr>.from_min_multiplicity");
    optional<int> to_max_multiplicity =
      _pt.second.get_optional<int>("<xmlattr>.to_max_multiplicity");
    optional<int> to_min_multiplicity =
      _pt.second.get_optional<int>("<xmlattr>.to_min_multiplicity");

    if (relid.is_initialized() && type.is_initialized() && from_id.is_initialized()
        && to_id.is_initialized() &&
        revision.is_initialized() && update_date.is_initialized()
        && create_date.is_initialized() &&
        from_max_multiplicity.is_initialized()
        && from_min_multiplicity.is_initialized()
        && to_max_multiplicity.is_initialized()
        && to_min_multiplicity.is_initialized())
    {
      _schm_rel->set_id(relid.get());
      _schm_rel->set_type(type.get());
      _schm_rel->set_name(name.get());
      _schm_rel->set_from_id(from_id.get());
      _schm_rel->set_to_id(to_id.get());
      _schm_rel->set_revision(revision.get());
      _schm_rel->set_comment(comment.is_initialized() ? comment.get() : string(""));
      _schm_rel->set_update_date(update_date.get());
      _schm_rel->set_create_date(create_date.get());

      multiplicity& from_m(_schm_rel->get_from_multiplicity());
      multiplicity& to_m(_schm_rel->get_to_multiplicity());

      from_m.set_multiplicity_(from_min_multiplicity.get(),
                               from_max_multiplicity.get());
      to_m.set_multiplicity_(to_min_multiplicity.get(), to_max_multiplicity.get());

      return true;
    }
    else
    {
      return false;
    }
  }
};


} //namespace core
} //namespace og
