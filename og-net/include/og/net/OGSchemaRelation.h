#pragma once

#include "og/net/OGSchemaParameter.h"
#include "og/net/OGSchemaObject.h"
#include "og/net/OGSharedPtr.h"

#include "og/core/og.h"

namespace og
{
class og_schema_relation;

namespace net
{
using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;

ref class OGSession;

public ref class OGSchemaRelation
{
public:

  OGSchemaRelation(OGSharedPtr<og::og_schema_relation> _rel_ptr);
  virtual ~OGSchemaRelation(void);

  ///////////////////////////////
  // property
  ///////////////////////////////
  String^ get_id();
  String^ get_type();
  String^ get_name();
  void set_name(String^ _name);
  String^ get_comment();
  void set_comment(String^ _comment);
  String^ get_revision();
  String^ get_create_date();
  String^ get_update_date();

  String^ get_from_id();
  String^ get_to_id();

  ///////////////////////////////
  // multiplicity(TODO)
  ///////////////////////////////
  //og::core::multiplicity& get_from_multiplicity(){}
  //og::core::multiplicity& get_to_multiplicity(){}
  //void set_from_multiplicity(og::core::multiplicity& _mul){}
  //void set_get_to_multiplicity(og::core::multiplicity& _mul){}
  // multiplicity ends.

  ///////////////////////////////
  // parameters
  ///////////////////////////////
  void get_parameters(
    List<Tuple<String^, OGSchemaParameter^>^>^
    _param_name_types);

  og::core::parameter_basetype_enum get_parameter_basetype_enum_by_param_name(
    String^ _param_name);

  void add_parameter_definition(String^ _param_name,
                                OGSchemaParameter^ _schm_par);

  template <typename P>
  void get_parameter_definition(String^ _param_name, P^ _param_basetype) {}

internal:
  OGSharedPtr<og::og_schema_relation> og_schema_relation_;

};

}
}
