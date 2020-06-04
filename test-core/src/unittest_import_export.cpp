#include "fixtures.h"
#include "utility.h"

using namespace std;
//using namespace og::core;

#ifdef TEST_OG_IMPORT_EXPORT

BOOST_FIXTURE_TEST_SUITE(import_export, fixture_clean_session);

string sesn_o1_id;


// xml export
BOOST_AUTO_TEST_CASE(import_export_1000_basic)
{
#ifdef _WINDOWS
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
  string OTYPE2a = "Document2002toa";
  string OTYPE2b = "Document2002tob";
  string RELTYPE = "Document2002rel";
  string RELTYPEa = "Document2002rel";
  string RELTYPEb = "Document2002rel";
  string ONAME = "Dcument-Name2002";
  string ONAMEa = "Dcument-Name2002a";
  string ONAMEb = "Dcument-Name2002b";
  list<string> otype_list;
  otype_list.push_back(OTYPE1);
  otype_list.push_back(OTYPE2);

  list<string> rel_type_list;
  rel_type_list.push_back(RELTYPE);

  og::og_schema_object_ptr p1 = cleaned_session_.schema()->create_object(OTYPE1,
                                ONAME);
  og::og_schema_object_ptr p2 = cleaned_session_.schema()->create_object(OTYPE2,
                                ONAME);
  og::og_schema_object_ptr p2a = cleaned_session_.schema()->create_object(OTYPE2a,
                                 ONAMEa);
  og::og_schema_object_ptr p2b = cleaned_session_.schema()->create_object(OTYPE2b,
                                 ONAMEb);

  std::cout << "schem:" << p2b->get_id() << std::endl;

  og::og_schema_relation_ptr rel_ptr = p1->connect_to(p2, RELTYPE);
  og::og_schema_relation_ptr rel_ptr_a = p2->connect_to(p2a, RELTYPEa);
  og::og_schema_relation_ptr rel_ptr_b = p2->connect_to(p2b, RELTYPEb);

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

  p2->add_parameter_definition("H2", ptest2);
  p2a->add_parameter_definition("H4", ptest4);
  p2b->add_parameter_definition("H6", ptest6);

  // apply parameter to schema_relation
  rel_ptr->add_parameter_definition("I1", ptest1);
  rel_ptr->add_parameter_definition("I3", ptest3);
  rel_ptr->add_parameter_definition("I5", ptest5);

  rel_ptr_a->add_parameter_definition("I2", ptest2);
  rel_ptr_b->add_parameter_definition("I4", ptest4);
  ///////////////////////////////////////////////////////////
  /// setup schema done
  ///////////////////////////////////////////////////////////


  og::og_session_object_ptr o1 = cleaned_session_.create_object(p1);
  sesn_o1_id = o1->get_id();
  og::og_session_object_ptr o2 = cleaned_session_.create_object(p2);
  og::og_session_relation_ptr o1o2 = o1->connect_to(o2, RELTYPE);

  og::og_session_object_ptr o2a = cleaned_session_.create_object(p2a);
  og::og_session_object_ptr o2b = cleaned_session_.create_object(p2b);
  og::og_session_relation_ptr o2o2a = o2->connect_to(o2a, RELTYPEa);
  og::og_session_relation_ptr o2o2b = o2->connect_to(o2b, RELTYPEb);

  o1->set_parameter_value("H1", 99);
  o1->set_parameter_value("H3", 3.14);
  o1->set_parameter_value("H5", std::string("x"));
  o2->set_parameter_value("H2", 100);
  o2a->set_parameter_value("H4", 9.99);
  o2b->set_parameter_value("H6", std::string("y"));

  std::list<og::og_session_object_ptr> objs1;
  cleaned_session_.get_object(&objs1);

  for (auto o : objs1)
  {
    std::cout << "type:" <<
              o->get_schema_object_type() << " name:" <<
              o->get_schema_object_name() << std::endl;
  }

  o1->export_to_recursively(string("export1.aaa"));
}

// xml export
BOOST_AUTO_TEST_CASE(import_export_1000_import)
{
#ifdef _WINDOWS
  // TODO: CrtCheckMemory detects memory leak in this test.
  //og::core::CrtCheckMemory __check__;
#endif

  fixture_once();

  // initialize db
  og::og_session cleaned_session_;
  cleaned_session_.open(DBPATH);
  cleaned_session_.purge();
  cleaned_session_.schema()->purge();

  string OTYPE1 = "Document2002from";
  string OTYPE2 = "Document2002to";
  string OTYPE2a = "Document2002toa";
  string OTYPE2b = "Document2002tob";
  string RELTYPE = "Document2002rel";
  string RELTYPEa = "Document2002rel";
  string RELTYPEb = "Document2002rel";
  string ONAME = "Dcument-Name2002";
  string ONAMEa = "Dcument-Name2002a";
  string ONAMEb = "Dcument-Name2002b";
  list<string> otype_list;
  otype_list.push_back(OTYPE1);
  otype_list.push_back(OTYPE2);

  list<string> rel_type_list;
  rel_type_list.push_back(RELTYPE);

  og::og_schema_object_ptr p1 = cleaned_session_.schema()->create_object(OTYPE1,
                                ONAME);
  og::og_schema_object_ptr p2 = cleaned_session_.schema()->create_object(OTYPE2,
                                ONAME);
  og::og_schema_object_ptr p2a = cleaned_session_.schema()->create_object(OTYPE2a,
                                 ONAMEa);
  og::og_schema_object_ptr p2b = cleaned_session_.schema()->create_object(OTYPE2b,
                                 ONAMEb);

  std::cout << "schem:" << p2b->get_id() << std::endl;

  og::og_schema_relation_ptr rel_ptr = p1->connect_to(p2, RELTYPE);
  og::og_schema_relation_ptr rel_ptr_a = p2->connect_to(p2a, RELTYPEa);
  og::og_schema_relation_ptr rel_ptr_b = p2->connect_to(p2b, RELTYPEb);

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

  p2->add_parameter_definition("H2", ptest2);
  p2a->add_parameter_definition("H4", ptest4);
  p2b->add_parameter_definition("H6", ptest6);

  // apply parameter to schema_relation
  rel_ptr->add_parameter_definition("I1", ptest1);
  rel_ptr->add_parameter_definition("I3", ptest3);
  rel_ptr->add_parameter_definition("I5", ptest5);

  rel_ptr_a->add_parameter_definition("I2", ptest2);
  rel_ptr_b->add_parameter_definition("I4", ptest4);
  ///////////////////////////////////////////////////////////
  /// setup schema done
  ///////////////////////////////////////////////////////////

  boost::optional<og::og_session_object_ptr> res =
    cleaned_session_.import_object_from_file(string("export1.aaa"));

  BOOST_REQUIRE_EQUAL(true, res.is_initialized());
  BOOST_REQUIRE_EQUAL(sesn_o1_id, res.get()->get_id()); // match because env was empty

  {
    std::list<og::og_session_object_ptr> objs1;
    cleaned_session_.get_object(&objs1);

    for (auto o : objs1)
    {
      std::cout << "type:" <<
                o->get_schema_object_type() << " name:" <<
                o->get_schema_object_name() << std::endl;
    }
  }
  {
    std::list<string> types;
    types.push_back(OTYPE1);

    std::list<og::og_session_object_ptr> objs1;
    cleaned_session_.get_object_by_type(types, &objs1);

    BOOST_REQUIRE_EQUAL(objs1.size(), 1);
    int i_;
    objs1.begin()->get()->get_parameter_value("H1", &i_);
    BOOST_REQUIRE_EQUAL(i_, 99);
    double d_;
    objs1.begin()->get()->get_parameter_value("H3",&d_);
    BOOST_REQUIRE_EQUAL(d_, 3.14);
    std::string s_;
    objs1.begin()->get()->get_parameter_value("H5", &s_);
    BOOST_REQUIRE_EQUAL(s_.c_str(), std::string("x").c_str());
  }

  {
    std::list<string> types;
    types.push_back(OTYPE2);

    std::list<og::og_session_object_ptr> objs1;
    cleaned_session_.get_object_by_type(types, &objs1);

    BOOST_REQUIRE_EQUAL(objs1.size(), 1);
    int i_;
    objs1.begin()->get()->get_parameter_value("H2", &i_);
    BOOST_REQUIRE_EQUAL(i_, 100);
  }

  {
    std::list<string> types;
    types.push_back(OTYPE2a);

    std::list<og::og_session_object_ptr> objs1;
    cleaned_session_.get_object_by_type(types, &objs1);

    BOOST_REQUIRE_EQUAL(objs1.size(), 1);
    double d_;
    objs1.begin()->get()->get_parameter_value("H4", &d_);
    BOOST_REQUIRE_EQUAL(d_, 9.99);
  }


  {
    std::list<string> types;
    types.push_back(OTYPE2b);

    std::list<og::og_session_object_ptr> objs1;
    cleaned_session_.get_object_by_type(types, &objs1);

    BOOST_REQUIRE_EQUAL(objs1.size(), 1);
    string s_;
    objs1.begin()->get()->get_parameter_value("H6", &s_);
    BOOST_REQUIRE_EQUAL(s_.c_str(), "y");
  }

  // transaction
  og::og_transaction tran1(cleaned_session_);

  boost::optional<og::og_session_object_ptr> res2 =
	  cleaned_session_.import_object_from_file(string("export1.aaa"));
  BOOST_REQUIRE_EQUAL(true, res2.is_initialized());
  BOOST_REQUIRE_NE(sesn_o1_id, res2.get()->get_id()); // unmatch because env was *not* empty

  tran1.commit();

  {
    std::list<string> types;
    types.push_back(OTYPE2b);

    std::list<og::og_session_object_ptr> objs1;
    cleaned_session_.get_object_by_type(types, &objs1);

    BOOST_REQUIRE_EQUAL(objs1.size(), 2);
    string s_;
    objs1.begin()->get()->get_parameter_value("H6", &s_);
    BOOST_REQUIRE_EQUAL(s_.c_str(), "y");

    string s2_;
    std::next(objs1.begin())->get()->get_parameter_value("H6", &s2_);
    BOOST_REQUIRE_EQUAL(s2_.c_str(), "y");
  }

  // transaction
  og::og_transaction tran2(cleaned_session_);

  boost::optional<og::og_session_object_ptr> res3 =
	  cleaned_session_.import_object_from_file(string("export1.aaa"));
  BOOST_REQUIRE_EQUAL(true, res2.is_initialized());

  tran2.rollback();


  {
	  std::list<string> types;
	  types.push_back(OTYPE2b);

	  std::list<og::og_session_object_ptr> objs1;
	  cleaned_session_.get_object_by_type(types, &objs1);

	  BOOST_REQUIRE_EQUAL(objs1.size(), 2);
	  string s_;
	  objs1.begin()->get()->get_parameter_value("H6", &s_);
	  BOOST_REQUIRE_EQUAL(s_.c_str(), "y");

	  string s2_;
	  std::next(objs1.begin())->get()->get_parameter_value("H6", &s2_);
	  BOOST_REQUIRE_EQUAL(s2_.c_str(), "y");
  }
}
BOOST_AUTO_TEST_SUITE_END()

#endif //TEST_OG_IMPORT_EXPORT

