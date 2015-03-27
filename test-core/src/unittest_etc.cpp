#include "fixtures.h"
#include "utility.h"

using namespace std;
//using namespace og::core;

#ifdef TEST_OG_ETC

BOOST_FIXTURE_TEST_SUITE(etc, fixture_clean_session);

// xml export
BOOST_AUTO_TEST_CASE(etc_1000)
{
#ifdef WINDOWS
  og::core::CrtCheckMemory __check__;
#endif

  // initialize db
  og::og_session cleaned_session_;
  cleaned_session_.connect(DBPATH);
  cleaned_session_.purge();
  cleaned_session_.schema()->purge();

  string OTYPE1 = "Document2002from";
  string OTYPE2 = "Document2002to";
  string RELTYPE = "Document2002rel";
  string ONAME = "Dcument-Name2002";
  list<string> otype_list;
  otype_list.push_back(OTYPE1);
  otype_list.push_back(OTYPE2);

  list<string> rel_type_list;
  rel_type_list.push_back(RELTYPE);

  og::og_schema_object_ptr p1 = cleaned_session_.schema()->create_object(OTYPE1,
                                ONAME);
  og::og_schema_object_ptr p2 = cleaned_session_.schema()->create_object(OTYPE2,
                                ONAME);

  og::og_schema_relation_ptr rel_ptr = p1->connect_to(p2, RELTYPE);

  og::og_integer type1;
  type1.default_value_ = 1;
  type1.warn_max_ = 3;
  type1.system_max_ = 3;
  type1.warn_min_ = 1;
  type1.system_min_ = 1;

  og::og_integer type2;
  type2.default_value_ = 2;
  type2.warn_max_ = 3;
  type2.system_max_ = 3;
  type2.warn_min_ = 1;
  type2.system_min_ = 1;

  og::og_real type3;
  type3.default_value_ = 1.141;
  type3.warn_max_ = 3.0;
  type3.system_max_ = 3.0;
  type3.warn_min_ = 1.0;
  type3.system_min_ = 1.0;

  og::og_real type4;
  type4.default_value_ = 2.236;
  type4.warn_max_ = 3;
  type4.system_max_ = 3;
  type4.warn_min_ = 1;
  type4.system_min_ = 1;

  og::og_text type5;
  type5.default_value_ = std::string("foo");
  type5.warn_max_ = 10;
  type5.system_max_ = 10;
  type5.warn_min_ = 1;
  type5.system_min_ = 1;

  og::og_text type6;
  type6.default_value_ = std::string("bar");
  type6.warn_max_ = 10;
  type6.system_max_ = 10;
  type6.warn_min_ = 1;
  type6.system_min_ = 1;

  // array size is set to 1 implicitly

  og::og_schema_parameter_ptr ptest1 =
    cleaned_session_.schema()->create_parameter("hoge_type1", "comment1",
        &type1);
  og::og_schema_parameter_ptr ptest3 =
    cleaned_session_.schema()->create_parameter("hoge_type3", "comment1",
        &type3);
  og::og_schema_parameter_ptr ptest5 =
    cleaned_session_.schema()->create_parameter("hoge_type5", "comment1",
        &type5);

  // array size is set to 3 explicitly
  og::og_schema_parameter_ptr ptest2 =
    cleaned_session_.schema()->create_parameter("hoge_type2", "comment2",
        &type2, 3, 2, 4);
  og::og_schema_parameter_ptr ptest4 =
    cleaned_session_.schema()->create_parameter("hoge_type4", "comment2",
        &type4, 3, 2, 4);
  og::og_schema_parameter_ptr ptest6 =
    cleaned_session_.schema()->create_parameter("hoge_type6", "comment2",
        &type6, 3, 2, 4);

  // apply parameter to schema_object
  p1->add_parameter_definition("H1", ptest1);
  p1->add_parameter_definition("H3", ptest3);
  p1->add_parameter_definition("H5", ptest5);

  p1->add_parameter_definition("H2", ptest2);
  p1->add_parameter_definition("H4", ptest4);
  p1->add_parameter_definition("H6", ptest6);

  // apply parameter to schema_relation
  rel_ptr->add_parameter_definition("I1", ptest1);
  rel_ptr->add_parameter_definition("I3", ptest3);
  rel_ptr->add_parameter_definition("I5", ptest5);

  rel_ptr->add_parameter_definition("I2", ptest2);
  rel_ptr->add_parameter_definition("I4", ptest4);
  rel_ptr->add_parameter_definition("I6", ptest6);

  og::og_session_object_ptr o1 = cleaned_session_.create_object(p1);
  og::og_session_object_ptr o2 = cleaned_session_.create_object(p2);
  og::og_session_relation_ptr o1o2 = o1->connect_to(o2, RELTYPE);
  //o1o2->sync();

  //cleaned_session_.schema_->export_to_file("schema.xml");
  cleaned_session_.schema()->export_to_file("schema.xml.gz");
  //cleaned_session_.export_to_file("session.xml");
  cleaned_session_.export_to_file("session.xml.gz");
  cleaned_session_.purge();
  cleaned_session_.import_from_file("session.xml.gz");

  list<og::og_session_object_ptr> sesn_objs;
  cleaned_session_.get_object_by_type(otype_list, &sesn_objs);

  BOOST_REQUIRE_EQUAL(sesn_objs.size(), 2);

  for (list<og::og_session_object_ptr>::iterator it = sesn_objs.begin();
       it != sesn_objs.end(); it++)
  {
    if ((*it)->get_schema_object_type() == OTYPE1)
    {
      list<og::og_session_object_ptr> sesn_cons;
      (*it)->get_connected_object(&sesn_cons);
      BOOST_REQUIRE_EQUAL(sesn_cons.size(), 1);
    }
  }
}

// xml import
BOOST_AUTO_TEST_CASE(etc_1001)
{
#ifdef WINDOWS
  og::core::CrtCheckMemory __check__;
#endif

  // initialize db
  og::og_session cleaned_session_;
  cleaned_session_.connect(DBPATH);
  cleaned_session_.purge();
  cleaned_session_.schema()->purge();

  string OTYPE1 = "Document2002from";
  string OTYPE2 = "Document2002to";
  string RELTYPE = "Document2002rel";
  string ONAME = "Dcument-Name2002";
  list<string> otype_list;
  otype_list.push_back(OTYPE1);
  otype_list.push_back(OTYPE2);

  list<string> rel_type_list;
  rel_type_list.push_back(RELTYPE);

  cleaned_session_.schema()->import_from_file("schema.xml.gz");
  cleaned_session_.import_from_file("session.xml.gz");


  list<og::og_session_object_ptr> sesn_objs;
  cleaned_session_.get_object_by_type(otype_list, &sesn_objs);

  BOOST_REQUIRE_EQUAL(sesn_objs.size(), 2);

  for (list<og::og_session_object_ptr>::iterator it = sesn_objs.begin();
       it != sesn_objs.end(); it++)
  {
    if ((*it)->get_schema_object_type() == OTYPE1)
    {
      list<og::og_session_object_ptr> sesn_cons;
      (*it)->get_connected_object(&sesn_cons);
      BOOST_REQUIRE_EQUAL(sesn_cons.size(), 1);
    }
  }

}


BOOST_AUTO_TEST_SUITE_END()

#endif //TEST_OG_ETC


