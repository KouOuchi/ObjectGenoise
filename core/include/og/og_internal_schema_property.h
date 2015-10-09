#pragma once

#include "og/og_session.h"
#include "og/og_schema_object.h"
#include "og/og_session_object.h"
#include "og/og_session_relation.h"

#include "og/core/exception.h"

#include <boost/tuple/tuple.hpp>

namespace og
{

class internal_schema_property
{
public:
  void internal_initialize(og_session* _session)
  {
    // check existance
    list<string> prop_type;
    list<og_session_object_ptr> prop_objs;

    prop_type.push_back(og_schema::schema_property_object_type());

    _session->get_object_by_type(prop_type, &prop_objs);


	// clean
	if (prop_objs.size() > 1)
	{
		// errror
		for (list<og_session_object_ptr>::iterator it = prop_objs.begin(); it != prop_objs.end(); it++)
		{
			it->get()->delete_object();
			(*_session->get_object((*it)->get_schema_object_id()))->delete_object();
		}
	}
	
	
	if (prop_objs.size() == 0)
    {
      // insert
      og_schema_object_ptr pobj = _session->schema()->create_object(
                                    og_schema::schema_property_object_type(),
                                    og_schema::schema_property_object_name());

      // param
      og::og_integer rev_type;
      rev_type.default_value_ = 0;
      rev_type.warn_max_ = 1000000;
      rev_type.system_max_ = 1000000;
      rev_type.warn_min_ = 1;
      rev_type.system_min_ = 1;

      og_schema_parameter_ptr rev_param_ptr =
        _session->schema()->create_parameter("rev_type", "core schema rev",
                                             &rev_type);

      pobj->add_parameter_definition(og_schema::schema_property_core_revision(),
                                     rev_param_ptr);

      _session->create_object(pobj);

    }
    else if (prop_objs.size() == 1)
    {
      // check
      list<boost::tuple<string, og_schema_parameter_ptr>> params;
      boost::optional<og_schema_object_ptr> schm_obj(_session->schema()->get_object(
            prop_objs.front()->get_schema_object_id()));

      if (!schm_obj.is_initialized())
      {
        // unexpected
        BOOST_THROW_EXCEPTION(og::core::exception() <<
                              og::core::exception_message("__og_schema_property__ unexpected.no param"));

      }

	  schm_obj.get()->get_parameters(&params);

      bool found = false;
      for (list<boost::tuple<string, og_schema_parameter_ptr>>::iterator it =
             params.begin();
           it != params.end(); it++)
      {
        string t(it->get<0>());

        if (t == string(og_schema::schema_property_core_revision()))
        {
          found = true;
          break;
        }
      }

      if (!found)
      {
        // unexpected
        BOOST_THROW_EXCEPTION(og::core::exception() <<
                              og::core::exception_message("__og_schema_property__ unexpected."));
      }
    }
  }

};

}// namespace og;