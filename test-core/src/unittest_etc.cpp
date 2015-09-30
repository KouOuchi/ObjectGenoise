#include "fixtures.h"
#include "utility.h"

using namespace std;
//using namespace og::core;

#ifdef TEST_OG_ETC

BOOST_FIXTURE_TEST_SUITE(etc, fixture_clean_session);

// xml export
BOOST_AUTO_TEST_CASE(etc_1000_basic_import)
{
#ifdef WINDOWS
  // TODO: CrtCheckMemory detects memory leak in this test.
  //og::core::CrtCheckMemory __check__;
#endif

  // initialize db
  og::og_session cleaned_session_;
  cleaned_session_.open(DBPATH);
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

  list<boost::tuple<string, og::og_schema_parameter_ptr>> __params_p1;
  p1->get_parameters(&__params_p1);
  list<boost::tuple<string, og::og_schema_parameter_ptr>> __params_rel;
  rel_ptr->get_parameters(&__params_rel);
  string p1id(p1->get_id());
  string relid(rel_ptr->get_id());

  //cleaned_session_.schema_->export_to_file("schema.xml");
  cleaned_session_.schema()->export_to_file("schema.xml.gz");
  cleaned_session_.schema()->purge();
  cleaned_session_.schema()->import_from_file("schema.xml.gz");

  boost::optional<og::og_schema_object_ptr> p1test = cleaned_session_.schema()->get_object(p1id);
  boost::optional<og::og_schema_relation_ptr> reltest = cleaned_session_.schema()->get_relation(relid);

  list<boost::tuple<string, og::og_schema_parameter_ptr>> __params_p1_after;
  p1test.get()->get_parameters(&__params_p1_after);
  list<boost::tuple<string, og::og_schema_parameter_ptr>> __params_rel_after;
  reltest.get()->get_parameters(&__params_rel_after);

  BOOST_REQUIRE_EQUAL(__params_p1.size(), __params_p1_after.size());
  BOOST_REQUIRE_EQUAL(__params_rel.size(), __params_rel_after.size());
}

// xml export
BOOST_AUTO_TEST_CASE(etc_1002_schema_verup)
{
#ifdef WINDOWS
  // TODO: CrtCheckMemory detects memory leak in this test.
  //og::core::CrtCheckMemory __check__;
#endif

  // initialize db
  og::og_session cleaned_session_;
  cleaned_session_.open(DBPATH);
  cleaned_session_.purge();
  cleaned_session_.schema()->purge();
  cleaned_session_.close();

  cleaned_session_.open(DBPATH);

  string OTYPE1 = "Document2002from";
  string OTYPE2 = "Document2002a to";
  string OTYPE3 = "Document2002b to";
  string RELTYPE1 = "Document2002rel";
  string RELTYPE2 = "Document2002rel";
  string ONAME = "Dcument-Name2002";
  list<string> otype_list;
  otype_list.push_back(OTYPE1);
  otype_list.push_back(OTYPE2);
  otype_list.push_back(OTYPE3);

  og::og_schema_object_ptr p1 = cleaned_session_.schema()->create_object(OTYPE1,
                                ONAME);
  og::og_schema_object_ptr p2 = cleaned_session_.schema()->create_object(OTYPE2,
                                ONAME);
  og::og_schema_object_ptr p3 = cleaned_session_.schema()->create_object(OTYPE3,
                                ONAME);

  og::og_schema_relation_ptr rel_ptr12 = p1->connect_to(p2, RELTYPE1);
  og::og_schema_relation_ptr rel_ptr13 = p1->connect_to(p3, RELTYPE2);

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
  type3.warn_max_ = 200.0;
  type3.system_max_ = 200.0;
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

  p2->add_parameter_definition("V1", ptest1);
  p2->add_parameter_definition("V3", ptest3);
  p2->add_parameter_definition("V5", ptest5);

  p3->add_parameter_definition("V2", ptest2);
  p3->add_parameter_definition("V4", ptest4);
  p3->add_parameter_definition("V6", ptest6);

  // apply parameter to schema_relation
  rel_ptr12->add_parameter_definition("I1", ptest1);
  rel_ptr12->add_parameter_definition("I3", ptest3);
  rel_ptr12->add_parameter_definition("I5", ptest5);

  rel_ptr12->add_parameter_definition("I2", ptest2);
  rel_ptr12->add_parameter_definition("I4", ptest4);
  rel_ptr12->add_parameter_definition("I6", ptest6);

  rel_ptr13->add_parameter_definition("I1", ptest1);
  rel_ptr13->add_parameter_definition("I3", ptest3);
  rel_ptr13->add_parameter_definition("I5", ptest5);

  rel_ptr13->add_parameter_definition("I2", ptest2);
  rel_ptr13->add_parameter_definition("I4", ptest4);
  rel_ptr13->add_parameter_definition("I6", ptest6);

  og::og_session_object_ptr o1 = cleaned_session_.create_object(p1);
  og::og_session_object_ptr o2 = cleaned_session_.create_object(p2);
  og::og_session_object_ptr o3 = cleaned_session_.create_object(p3);
  og::og_session_relation_ptr o1o2 = o1->connect_to(o2, RELTYPE1);
  og::og_session_relation_ptr o1o3 = o1->connect_to(o3, RELTYPE2);

  o2->set_parameter_value<int>("V1", 100);
  o2->set_parameter_value<double>("V3", 100.0);
  o2->set_parameter_value<string>("V5", "100");

  o3->set_parameter_value<int>("V2", 101);
  o3->set_parameter_value<double>("V4", 101.0);
  o3->set_parameter_value<string>("V6", "101");

  cleaned_session_.schema()->export_to_file("schema_xml_catchup0.xml.gz");
  cleaned_session_.export_to_file("session_xml_catchup0.xml.gz");

  // catchup schema
  list<string> prop_type;
  prop_type.push_back(og::og_schema::schema_property_object_type());

  list<og::og_session_object_ptr> prop_objs;
  cleaned_session_.get_object_by_type(prop_type, &prop_objs);

  BOOST_REQUIRE_EQUAL(prop_objs.size(), 1);

  int ver;
  prop_objs.front()->get_parameter_value<int>
  (og::og_schema::schema_property_core_revision(), &ver);

  ++ver;
  prop_objs.front()->set_parameter_value<int>
  (og::og_schema::schema_property_core_revision(), ver);

  //"delete param definition" deletes all session's parameters
  p2->add_parameter_definition(string("add int"), ptest1);
  p2->add_parameter_definition(string("add real"), ptest3);
  p2->add_parameter_definition(string("add text"), ptest5);

  //"delete param definition" deletes all session's parameters
  p3->delete_parameter_definition(string("V6"), ptest6);
  rel_ptr13->delete_parameter_definition(string("I6"), ptest6);

  cleaned_session_.schema()->export_to_file("schema_xml_catchup1.xml.gz");

  cleaned_session_.purge();
  BOOST_REQUIRE_EQUAL(true, cleaned_session_.import_from_file("session_xml_catchup0.xml.gz"));

  list<og::og_session_object_ptr> sesn_objs;
  cleaned_session_.get_object_by_type(otype_list, &sesn_objs);

  BOOST_REQUIRE_EQUAL(sesn_objs.size(), 3);

  for (list<og::og_session_object_ptr>::iterator it = sesn_objs.begin();
       it != sesn_objs.end(); it++)
  {
    if ((*it)->get_schema_object_type() == OTYPE1)
    {
      list<og::og_session_object_ptr> sesn_cons;
      (*it)->get_connected_object(&sesn_cons);
      BOOST_REQUIRE_EQUAL(sesn_cons.size(), 2);

      {
        const string & sid = (*it)->get_schema_object_id();
        boost::optional<og::og_schema_object_ptr> ssidob =
          cleaned_session_.schema()->get_object(sid);

        list<boost::tuple<string, og::og_schema_parameter_ptr>> __params;
        ssidob.get()->get_parameters(&__params);

        BOOST_REQUIRE_EQUAL(__params.size(), 6);
      }

      for (list<og::og_session_object_ptr>::iterator it2 = sesn_cons.begin();
           it2 != sesn_cons.end(); it2++)
      {
        if (it2->get()->get_schema_object_type() == OTYPE2)
        {
          list<boost::tuple<string, og::og_schema_parameter_ptr>> __params2;
          cleaned_session_.schema()->get_object(
            it2->get()->get_schema_object_id())->get()->get_parameters(&__params2);

          BOOST_REQUIRE_EQUAL(6, __params2.size());
          it2->get()->set_parameter_value<int>("add int", 10);
          it2->get()->set_parameter_value<double>("add real", 1.4142);
          it2->get()->set_parameter_value<string>("add text", "hoge");

          int i;
          double ii;
          string iii;
		  it2->get()->get_parameter_value<int>("add int", &i);
          it2->get()->get_parameter_value<double>("add real", &ii);
          it2->get()->get_parameter_value<string>("add text", &iii);

        }
        if (it2->get()->get_schema_object_type() == OTYPE3)
        {
          list<boost::tuple<string, og::og_schema_parameter_ptr>> __params3;
          cleaned_session_.schema()->get_object(
            it2->get()->get_schema_object_id())->get()->get_parameters(&__params3);

          BOOST_REQUIRE_EQUAL(2, __params3.size());
        }
      }
    }
  }
}

// xml export
BOOST_AUTO_TEST_CASE(etc_1003_schema_catchup)
{
#ifdef WINDOWS
  // TODO: CrtCheckMemory detects memory leak in this test.
  //og::core::CrtCheckMemory __check__;
#endif
  string OTYPE1 = "Document2002from";
  string OTYPE2 = "Document2002a to";
  string OTYPE3 = "Document2002b to";
  string RELTYPE1 = "Document2002rel";
  string RELTYPE2 = "Document2002rel";
  string ONAME = "Dcument-Name2002";
  list<string> otype_list;
  otype_list.push_back(OTYPE1);
  otype_list.push_back(OTYPE2);
  otype_list.push_back(OTYPE3);

  // initialize db
  og::og_session cleaned_session_;
  cleaned_session_.open(DBPATH);
  cleaned_session_.purge();
  cleaned_session_.schema()->purge();
  cleaned_session_.close();

  cleaned_session_.open(DBPATH);

  // pre condition
  cleaned_session_.schema()->import_from_file("schema_xml_catchup0.xml.gz");
  cleaned_session_.import_from_file("session_xml_catchup0.xml.gz");

  bool res = cleaned_session_.catchup_schema("schema_xml_catchup1.xml.gz");

  BOOST_REQUIRE_EQUAL(true, res);

  list<og::og_session_object_ptr> sesn_objs;
  cleaned_session_.get_object_by_type(otype_list, &sesn_objs);

  BOOST_REQUIRE_EQUAL(sesn_objs.size(), 3);

  for (list<og::og_session_object_ptr>::iterator it = sesn_objs.begin();
       it != sesn_objs.end(); it++)
  {
    if ((*it)->get_schema_object_type() == OTYPE1)
    {
      list<og::og_session_object_ptr> sesn_cons;
      (*it)->get_connected_object(&sesn_cons);
      BOOST_REQUIRE_EQUAL(sesn_cons.size(), 2);

      {
        const string & sid = (*it)->get_schema_object_id();
        boost::optional<og::og_schema_object_ptr> ssidob =
          cleaned_session_.schema()->get_object(sid);

        list<boost::tuple<string, og::og_schema_parameter_ptr>> __params;
        ssidob.get()->get_parameters(&__params);

        BOOST_REQUIRE_EQUAL(6, __params.size());
      }

      for (list<og::og_session_object_ptr>::iterator it2 = sesn_cons.begin();
           it2 != sesn_cons.end(); it2++)
      {
        if (it2->get()->get_schema_object_type() == OTYPE2)
        {
          list<boost::tuple<string, og::og_schema_parameter_ptr>> __params2;
          cleaned_session_.schema()->get_object(
            it2->get()->get_schema_object_id())->get()->get_parameters(&__params2);

          BOOST_REQUIRE_EQUAL(6, __params2.size());
          it2->get()->set_parameter_value<int>("add int", 10);
          it2->get()->set_parameter_value<double>("add real", 1.4142);
          it2->get()->set_parameter_value<string>("add text", "hoge");

          int i;
          double ii;
          string iii;
		  it2->get()->get_parameter_value<int>("add int", &i);
          it2->get()->get_parameter_value<double>("add real", &ii);
          it2->get()->get_parameter_value<string>("add text", &iii);

        }
        if (it2->get()->get_schema_object_type() == OTYPE3)
        {
          list<boost::tuple<string, og::og_schema_parameter_ptr>> __params3;
          cleaned_session_.schema()->get_object(
            it2->get()->get_schema_object_id())->get()->get_parameters(&__params3);

          BOOST_REQUIRE_EQUAL(2, __params3.size());
        }
      }
    }
  }
}

BOOST_AUTO_TEST_SUITE_END()

#endif //TEST_OG_ETC
