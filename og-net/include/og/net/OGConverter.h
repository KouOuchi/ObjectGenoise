#pragma once

#include <list>
#include <map>
#include <string>

#include "og/net/OGException.h"
#include "og/net/OGSharedPtr.h"
#include "og/net/OGSchemaObject.h"
#include "og/net/OGSchemaRelation.h"
#include "og/net/OGSession.h"
#include "og/net/OGSessionObject.h"
#include "og/net/OGSessionRelation.h"

#include "og/og_schema.h"
#include "og/og_schema_object.h"

namespace og
{
namespace net
{

using namespace System;
using namespace System::Collections::Generic;

ref class OGConverter
{
public:
  OGConverter(void);
  virtual ~OGConverter(void);

  // pointer types
  template <class CLR_T, class STD_T>
  static void convert_clr_to_std(CLR_T^ _source, STD_T* _target);

  template <class STD_T, class CLR_T>
  static void convert_std_to_clr(STD_T* _source, CLR_T^ _target);

  // std::string/String
  template <>
  static void convert_clr_to_std
  ( String^ _source, std::string* _target )
  {
    using namespace Runtime::InteropServices;
    const char* chars =
      (const char*)(Marshal::StringToHGlobalAnsi(_source)).ToPointer();
    *_target = chars;
    Marshal::FreeHGlobal(IntPtr((void*)chars));
  };

  template <>
  static void convert_std_to_clr
  ( std::string* _source, String^ _target )
  {
	  _target = gcnew String(_source->c_str());
  };

  // value types
  template <class CLR_T, class STD_T>
  static void convert_value_clr_to_std(CLR_T^ _source, STD_T* _target);

  template <class STD_T, class CLR_T>
  static void convert_value_std_to_clr(STD_T* _source, CLR_T^ _target);

  // std::list/List value types
  template <>
  static void convert_value_clr_to_std<List<int>, std::list<int>>
      (List<int>^ _source, std::list<int>* _target)
  {
    if(_source == nullptr) { throw gcnew OGException(gcnew System::String("_source is null")); }
    if(!_target) { throw gcnew OGException(gcnew System::String("_target is null")); }
    for each (int s in _source)
    {
      _target->push_back(s); // no conversion
    }
  };

  template <>
  static void convert_value_std_to_clr<std::list<int>, List<int>>
      (std::list<int>* _source, List<int>^ _target)
  {
    if(_source == nullptr) { throw gcnew OGException(gcnew System::String("_source is null")); }
    if(!_target) { throw gcnew OGException(gcnew System::String("_target is null")); }

    for(std::list<int>::const_iterator it = _source->begin();
        it != _source->end(); ++it)
    {
      _target->Add(*it); // no conversion
    }
  };

  template <>
  static void convert_value_clr_to_std<List<double>, std::list<double>>
      (List<double>^ _source, std::list<double>* _target)
  {
    if(_source == nullptr) { throw gcnew OGException(gcnew System::String("_source is null")); }
    if(!_target) { throw gcnew OGException(gcnew System::String("_target is null")); }
    for each (double s in _source)
    {
      _target->push_back(s); // no conversion
    }
  };

  template <>
  static void convert_value_std_to_clr<std::list<double>, List<double>>
      (std::list<double>* _source, List<double>^ _target)
  {
    if(_source == nullptr) { throw gcnew OGException(gcnew System::String("_source is null")); }
    if(!_target) { throw gcnew OGException(gcnew System::String("_target is null")); }

    for(std::list<double>::const_iterator it = _source->begin();
        it != _source->end(); ++it)
    {
      _target->Add(*it); // no conversion
    }
  };


  template <>
  static void convert_clr_to_std<List<String^>, std::list<string>>
      (List<String^>^ _source, std::list<string>* _target)
  {
    if(_source == nullptr) { throw gcnew OGException(gcnew System::String("_source is null")); }
    if(!_target) { throw gcnew OGException(gcnew System::String("_target is null")); }
    for each (String^ s in _source)
    {
      std::string t;
      convert_clr_to_std<String, std::string>(s, &t);
      _target->push_back(t);
    }
  };

  template <>
  static void convert_std_to_clr<std::list<string>, List<String^>>
      (std::list<string>* _source, List<String^>^ _target)
  {
    if(_source == nullptr) { throw gcnew OGException(gcnew System::String("_source is null")); }
    if(!_target) { throw gcnew OGException(gcnew System::String("_target is null")); }

    for(std::list<std::string>::const_iterator it = _source->begin();
        it != _source->end(); ++it)
    {
      _target->Add(gcnew String(it->c_str()));
    }
  };

  template <>
  static void convert_value_clr_to_std<List<bool>, std::list<int>>
      (List<bool>^ _source, std::list<int>* _target)
  {
    if(_source == nullptr) { throw gcnew OGException(gcnew System::String("_source is null")); }
    if(!_target) { throw gcnew OGException(gcnew System::String("_target is null")); }
    for each (int s in _source)
    {
      _target->push_back(s ? 1 : 0); // no conversion
    }
  };

  template <>
  static void convert_value_std_to_clr<std::list<int>, List<bool>>
      (std::list<int>* _source, List<bool>^ _target)
  {
    if(_source == nullptr) { throw gcnew OGException(gcnew System::String("_source is null")); }
    if(!_target) { throw gcnew OGException(gcnew System::String("_target is null")); }

    for(std::list<int>::const_iterator it = _source->begin();
        it != _source->end(); ++it)
    {
      _target->Add(*it == 1); // no conversion
    }
  };


  template <>
  static void
  convert_std_to_clr<std::list<og::og_schema_object_ptr>, List<OGSchemaObject^>>
      ( std::list<og::og_schema_object_ptr>* _source, List<OGSchemaObject^>^ _target )
  {
    if(!_source) { throw gcnew OGException(gcnew System::String("_source is null")); }
    if(_target==nullptr) { throw gcnew OGException(gcnew System::String("_target is null")); }

    for(std::list<og::og_schema_object_ptr>::iterator it = _source->begin();
        it != _source->end(); ++it)
    {
      _target->Add( gcnew OGSchemaObject(OGSharedPtr<og::og_schema_object>(*it)) );
    }
  };

  template <>
  static void
  convert_std_to_clr<std::list<og::og_schema_relation_ptr>, List<OGSchemaRelation^>>
      ( std::list<og::og_schema_relation_ptr>* _source,
        List<OGSchemaRelation^>^ _target )
  {
    if(!_source) { throw gcnew OGException(gcnew System::String("_source is null")); }
    if(_target==nullptr) { throw gcnew OGException(gcnew System::String("_target is null")); }

    for(std::list<og::og_schema_relation_ptr>::iterator it = _source->begin();
        it != _source->end(); ++it)
    {
      _target->Add( gcnew OGSchemaRelation(OGSharedPtr<og::og_schema_relation>
                                           (*it)) );
    }
  };

  template <>
  static void
  convert_std_to_clr<std::list<og::og_session_object_ptr>, List<OGSessionObject^>>
      ( std::list<og::og_session_object_ptr>* _source,
        List<OGSessionObject^>^ _target )
  {
    if(!_source) { throw gcnew OGException(gcnew System::String("_source is null")); }
    if(_target==nullptr) { throw gcnew OGException(gcnew System::String("_target is null")); }

    for(std::list<og::og_session_object_ptr>::iterator it = _source->begin();
        it != _source->end(); ++it)
    {
      _target->Add( gcnew OGSessionObject(OGSharedPtr<og::og_session_object>(*it)) );
    }
  };

  template <>
  static void
  convert_std_to_clr<std::list<og::og_session_relation_ptr>, List<OGSessionRelation^>>
      ( std::list<og::og_session_relation_ptr>* _source,
        List<OGSessionRelation^>^ _target )
  {
    if(!_source) { throw gcnew OGException(gcnew System::String("_source is null")); }
    if(_target==nullptr) { throw gcnew OGException(gcnew System::String("_target is null")); }

    for(std::list<og::og_session_relation_ptr>::iterator it = _source->begin();
        it != _source->end(); ++it)
    {
      _target->Add( gcnew OGSessionRelation(OGSharedPtr<og::og_session_relation>
                                            (*it)) );
    }
  };

  template <>
  static void
  convert_std_to_clr<std::map<std::string, std::list<og::og_schema_object_ptr>>, Dictionary<String^, List<OGSchemaObject^>^>>
      ( std::map<std::string, std::list<og::og_schema_object_ptr> >* _source,
        Dictionary<String^, List<OGSchemaObject^>^>^ _target )
  {
    if(!_source) { throw gcnew OGException(gcnew System::String("_source is null")); }
    if(_target==nullptr) { throw gcnew OGException(gcnew System::String("_target is null")); }

    for(std::map<std::string, std::list<og::og_schema_object_ptr> >::iterator it =
          _source->begin();
        it != _source->end(); ++it)
    {
      List<OGSchemaObject^>^ values = gcnew List<OGSchemaObject^>();
      convert_std_to_clr<std::list<og::og_schema_object_ptr>, List<OGSchemaObject^>>
          (&(it->second), values);

      _target->Add(gcnew String(it->first.c_str()), values);
    }
  };

  template <>
  static void 
  convert_std_to_clr<std::list<boost::tuple<std::string, og::og_schema_parameter_ptr>>, List<Tuple<String^, OGSchemaParameter^>^>>
      ( std::list<boost::tuple<std::string, og::og_schema_parameter_ptr>>* _source,
        List<Tuple<String^, OGSchemaParameter^>^>^ _target )
  {
    if(!_source) { throw gcnew OGException(gcnew System::String("_source is null")); }
    if(_target==nullptr) { throw gcnew OGException(gcnew System::String("_target is null")); }

    for(std::list<boost::tuple<std::string, og::og_schema_parameter_ptr>>::iterator it =
          _source->begin();
        it != _source->end(); ++it)
    {

      List<Tuple<String^, OGSchemaParameter^>^>^ values = gcnew
          List<Tuple<String^, OGSchemaParameter^>^>();

	  String^ s = gcnew String(it->get<0>().c_str());
	  OGSchemaParameter^ p = gcnew OGSchemaParameter(OGSharedPtr<og::og_schema_parameter>(it->get<1>()));

	  _target->Add(gcnew Tuple<String^, OGSchemaParameter^>(s, p));
    }
  };

  template <>
  static void convert_clr_to_std<List<Object^>, std::list<og::core::parameter_value_variant>>
      (List<Object^>^ _source, std::list<og::core::parameter_value_variant>* _target)
  {
    if(_source == nullptr) { throw gcnew OGException(gcnew System::String("_source is null")); }
    if(!_target) { throw gcnew OGException(gcnew System::String("_target is null")); }
    for each (Object^ s in _source)
    {

//      _target->push_back(t);
    }
  };
};

}
}