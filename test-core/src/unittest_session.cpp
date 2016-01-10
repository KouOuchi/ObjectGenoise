#include "fixtures.h"
#include "utility.h"
#include "og_session_object_comparer.h"

#ifdef TEST_OG_SESSION
BOOST_FIXTURE_TEST_SUITE(session, fixture_clean_session);

// basic
BOOST_AUTO_TEST_CASE( session_1000 )
{
#ifdef WINDOWS
  og::core::CrtCheckMemory __check__;
#endif

  // initialize db
  og::og_session cleaned_session_;
  cleaned_session_.open(DBPATH);
  cleaned_session_.purge();
  cleaned_session_.schema()->purge();

  string OTYPE = "type_session_1000";
  string ONAME = "name_session_1000";
  list<string> oname_list; oname_list.push_back(ONAME);
  {
    optional<og::og_schema_object_ptr> schm_ptr(
      cleaned_session_.schema()->create_object(
        OTYPE, ONAME));

    og::og_session_object_ptr sesn_ptr = cleaned_session_.create_object(
                                           schm_ptr.get());

    BOOST_REQUIRE(sesn_ptr->get_schema_object_id() == schm_ptr.get()->get_id());
    BOOST_REQUIRE(sesn_ptr->get_schema_object_type() == schm_ptr.get()->get_type());
  }
}

// not found
BOOST_AUTO_TEST_CASE( session_1001 )
{
#ifdef WINDOWS
  og::core::CrtCheckMemory __check__;
#endif

  // initialize db
  og::og_session cleaned_session_;
  cleaned_session_.open(DBPATH);
  cleaned_session_.purge();
  cleaned_session_.schema()->purge();

  string OTYPE = "type_session_1001";
  string ONAME = "name_session_1001";
  list<string> otype_list; otype_list.push_back(OTYPE);
  list<string> oname_list; oname_list.push_back(ONAME);
  {
    // create schema obj
    og::og_schema_object_ptr schm_obj = cleaned_session_.schema()->create_object(
                                          OTYPE,
                                          ONAME);

    // create 2 session obj
    for(int i=0; i<2; i++)
    {
      og::og_session_object_ptr sesn_ptr = cleaned_session_.create_object(schm_obj);
      sesn_ptr->set_comment("hogehoge");
      sesn_ptr->set_instance_name("v");
      //sesn_ptr->set_revision("2");
      //sesn_ptr->sync();
    }

    list<og::og_session_object_ptr> sesn_obj_list;
    cleaned_session_.get_object_by_type(otype_list, &sesn_obj_list);

    BOOST_REQUIRE(sesn_obj_list.size() == 2);
    BOOST_FOREACH(og::og_session_object_ptr &r, sesn_obj_list)
    {
      BOOST_REQUIRE(r->get_comment() == "hogehoge");
      BOOST_REQUIRE(r->get_instance_name() == "v");
      //BOOST_REQUIRE(r->get_revision() == "2");
    }
  }

  {
    // re open session
    og::og_session ses2;
    ses2.open(DBPATH);

    list<og::og_session_object_ptr> sesn_obj_list;
    ses2.get_object_by_type(otype_list, &sesn_obj_list);

    BOOST_REQUIRE(sesn_obj_list.size() == 2);
    BOOST_FOREACH(og::og_session_object_ptr &r, sesn_obj_list)
    {
      BOOST_REQUIRE(r->get_comment() == "hogehoge");
      BOOST_REQUIRE(r->get_instance_name() == "v");
      //BOOST_REQUIRE(r->get_revision() == "2");
    }
  }
}

// session relatoin basic test
BOOST_AUTO_TEST_CASE( session_1002 )
{
#ifdef WINDOWS
  og::core::CrtCheckMemory __check__;
#endif

  // initialize db
  og::og_session cleaned_session_;
  cleaned_session_.open(DBPATH);
  cleaned_session_.purge();
  cleaned_session_.schema()->purge();

  string OTYPE1 = "type_session_1002_1";
  string ONAME1 = "name_session_1002_1";
  string OTYPE2 = "type_session_1002_2";
  string ONAME2 = "name_session_1002_2";
  string RELTYPE = "reltype_session_1002";
  string RELNAME = "relnamee_session_1002";
  list<string> otype_list1; otype_list1.push_back(OTYPE1);
  list<string> oname_list1; oname_list1.push_back(ONAME1);
  list<string> otype_list2; otype_list2.push_back(OTYPE2);
  list<string> oname_list2; oname_list2.push_back(ONAME2);
  list<string> reltype_list; reltype_list.push_back(RELTYPE);
  list<string> relname_list; relname_list.push_back(RELNAME);
  {
    // create schema obj
    og::og_schema_object_ptr schm_obj1 = cleaned_session_.schema()->create_object(
                                           OTYPE1,
                                           ONAME1);
    og::og_schema_object_ptr schm_obj2 = cleaned_session_.schema()->create_object(
                                           OTYPE2,
                                           ONAME2);
    og::og_schema_relation_ptr rel = schm_obj1->connect_to(schm_obj2, RELTYPE);

    og::og_session_object_ptr o1 = cleaned_session_.create_object(schm_obj1);
    og::og_session_object_ptr o2 = cleaned_session_.create_object(schm_obj2);

    BOOST_REQUIRE((o1->validate_connect_to(o2,
                                           RELTYPE) & og::core::connection_validation_result_enum::valid) ==
                  og::core::connection_validation_result_enum::valid);
    BOOST_REQUIRE((o2->validate_connect_to(o1,
                                           RELTYPE) & og::core::connection_validation_result_enum::invalid) ==
                  og::core::connection_validation_result_enum::invalid);
    BOOST_REQUIRE((o1->validate_connect_to(o1,
                                           RELTYPE) & og::core::connection_validation_result_enum::invalid) ==
                  og::core::connection_validation_result_enum::invalid);
    BOOST_REQUIRE((o2->validate_connect_to(o2,
                                           RELTYPE) & og::core::connection_validation_result_enum::invalid) ==
                  og::core::connection_validation_result_enum::invalid);

    BOOST_REQUIRE((o1->validate_connect_from(o2,
                   RELTYPE) & og::core::connection_validation_result_enum::invalid) ==
                  og::core::connection_validation_result_enum::invalid);
    BOOST_REQUIRE((o2->validate_connect_from(o1,
                   RELTYPE) & og::core::connection_validation_result_enum::valid) ==
                  og::core::connection_validation_result_enum::valid);
    BOOST_REQUIRE((o1->validate_connect_from(o1,
                   RELTYPE) & og::core::connection_validation_result_enum::invalid) ==
                  og::core::connection_validation_result_enum::invalid);
    BOOST_REQUIRE((o2->validate_connect_from(o2,
                   RELTYPE) & og::core::connection_validation_result_enum::invalid) ==
                  og::core::connection_validation_result_enum::invalid);

    o1->connect_to(o2, RELTYPE);

    list<og::og_session_object_ptr> sesn_obj_list;
    o1->get_connected_object_to(&sesn_obj_list);
    BOOST_REQUIRE(sesn_obj_list.size() == 1);
    BOOST_REQUIRE((*(sesn_obj_list.begin()))->get_id() == o2->get_id());

    sesn_obj_list.clear();
    o1->get_connected_object_from(&sesn_obj_list);
    BOOST_REQUIRE(sesn_obj_list.size() == 0);

    sesn_obj_list.clear();
    o2->get_connected_object_from(&sesn_obj_list);
    BOOST_REQUIRE(sesn_obj_list.size() == 1);
    BOOST_REQUIRE((*(sesn_obj_list.begin()))->get_id() == o1->get_id());

    sesn_obj_list.clear();
    o2->get_connected_object_to(&sesn_obj_list);
    BOOST_REQUIRE(sesn_obj_list.size() == 0);

    sesn_obj_list.clear();
    o2->get_connected_object(&sesn_obj_list);
    BOOST_REQUIRE(sesn_obj_list.size() == 1);
    BOOST_REQUIRE((*(sesn_obj_list.begin()))->get_id() == o1->get_id());

    sesn_obj_list.clear();
    o2->get_connected_object(reltype_list, &sesn_obj_list);
    BOOST_REQUIRE(sesn_obj_list.size() == 1);
    BOOST_REQUIRE((*(sesn_obj_list.begin()))->get_id() == o1->get_id());

    sesn_obj_list.clear();
    o1->get_connected_object(&sesn_obj_list);
    BOOST_REQUIRE(sesn_obj_list.size() == 1);
    BOOST_REQUIRE((*(sesn_obj_list.begin()))->get_id() == o2->get_id());

    sesn_obj_list.clear();
    o1->get_connected_object(reltype_list, &sesn_obj_list);
    BOOST_REQUIRE(sesn_obj_list.size() == 1);
    BOOST_REQUIRE((*(sesn_obj_list.begin()))->get_id() == o2->get_id());

    list<og::og_session_relation_ptr> sesn_rel_list;
    cleaned_session_.get_relation_by_type(reltype_list, &sesn_rel_list);
    BOOST_REQUIRE(sesn_rel_list.size() == 1);

    sesn_rel_list.clear();
    cleaned_session_.get_relation_by_name(relname_list, &sesn_rel_list);
    BOOST_REQUIRE(sesn_rel_list.size() == 1);

    o1->disconnect(o2);

    o1->get_connected_object_to(&sesn_obj_list);
    BOOST_REQUIRE(sesn_obj_list.size() == 0);

    sesn_obj_list.clear();
    o1->get_connected_object_from(&sesn_obj_list);
    BOOST_REQUIRE(sesn_obj_list.size() == 0);

    sesn_obj_list.clear();
    o2->get_connected_object_from(&sesn_obj_list);
    BOOST_REQUIRE(sesn_obj_list.size() == 0);

    sesn_obj_list.clear();
    o2->get_connected_object_to(&sesn_obj_list);
    BOOST_REQUIRE(sesn_obj_list.size() == 0);

    sesn_obj_list.clear();
    o2->get_connected_object(&sesn_obj_list);
    BOOST_REQUIRE(sesn_obj_list.size() == 0);

    sesn_obj_list.clear();
    o2->get_connected_object(reltype_list, &sesn_obj_list);
    BOOST_REQUIRE(sesn_obj_list.size() == 0);

    sesn_obj_list.clear();
    o1->get_connected_object(&sesn_obj_list);
    BOOST_REQUIRE(sesn_obj_list.size() == 0);

    sesn_obj_list.clear();
    o1->get_connected_object(reltype_list, &sesn_obj_list);
    BOOST_REQUIRE(sesn_obj_list.size() == 0);

    sesn_rel_list.clear();
    cleaned_session_.get_relation_by_type(reltype_list, &sesn_rel_list);
    BOOST_REQUIRE(sesn_rel_list.size() == 0);

    sesn_rel_list.clear();
    cleaned_session_.get_relation_by_name(relname_list, &sesn_rel_list);
    BOOST_REQUIRE(sesn_rel_list.size() == 0);
  }
}

#ifdef TEST_REQUIRE_THROW
// session obj with no schema
BOOST_AUTO_TEST_CASE( session_1003 )
{
  og::og_schema_object_ptr o(new og::og_schema_object(
                               cleaned_session_.schema().get()));
  BOOST_REQUIRE_THROW( cleaned_session_.create_object(o), og::core::exception );
}
#endif

// connectable relation
BOOST_AUTO_TEST_CASE( session_1004 )
{
#ifdef WINDOWS
  og::core::CrtCheckMemory __check__;
#endif

  // initialize db
  og::og_session cleaned_session_;
  cleaned_session_.open(DBPATH);
  cleaned_session_.purge();
  cleaned_session_.schema()->purge();

  string OTYPE1 = "type_session_1004_1";
  string ONAME1 = "name_session_1004_1";
  string OTYPE2 = "type_session_1004_2";
  string ONAME2 = "name_session_1004_2";
  string RELTYPE1 = "reltype_session_1004_1";
  string RELTYPE2 = "reltype_session_1004_2";
  list<string> otype_list1; otype_list1.push_back(OTYPE1);
  list<string> oname_list1; oname_list1.push_back(ONAME1);
  list<string> otype_list2; otype_list2.push_back(OTYPE2);
  list<string> oname_list2; oname_list2.push_back(ONAME2);
  list<string> reltype_list; reltype_list.push_back(RELTYPE1);

  // create schema obj
  og::og_schema_object_ptr schm_obj1 = cleaned_session_.schema()->create_object(
                                         OTYPE1,
                                         ONAME1);
  og::og_schema_object_ptr schm_obj2 = cleaned_session_.schema()->create_object(
                                         OTYPE2,
                                         ONAME2);
  og::og_schema_object_ptr schm_obj3 = cleaned_session_.schema()->create_object(
                                         OTYPE1,
                                         ONAME1);
  og::og_schema_object_ptr schm_obj4 = cleaned_session_.schema()->create_object(
                                         OTYPE2,
                                         ONAME2);
  schm_obj1->connect_to(schm_obj2, RELTYPE1);
  schm_obj1->connect_to(schm_obj4, RELTYPE1);
  schm_obj3->connect_to(schm_obj4, RELTYPE2);

  og::og_session_object_ptr o1 = cleaned_session_.create_object(schm_obj1);
  og::og_session_object_ptr o2 = cleaned_session_.create_object(schm_obj2);

  // check connectable rel type
  list<string> con_type1;
  o1->get_connectable_relation_type_to(&con_type1);
  BOOST_REQUIRE(con_type1.size() == 1);
  BOOST_REQUIRE( RELTYPE1.compare(*con_type1.begin()) == 0 );

  list<string> con_type2;
  o1->get_connectable_relation_type_from(&con_type2);
  BOOST_REQUIRE(con_type2.size() == 0);

  list<string> con_type3;
  o2->get_connectable_relation_type_to(&con_type3);
  BOOST_REQUIRE(con_type3.size() == 0);

  list<string> con_type4;
  o2->get_connectable_relation_type_from(&con_type4);
  BOOST_REQUIRE(con_type4.size() == 1);
  BOOST_REQUIRE( RELTYPE1.compare(*con_type4.begin()) == 0 );

  // check connectable rel type and schema obj
  map<string, list<og::og_schema_object_ptr>> con_pair1;
  o1->get_connectable_to(&con_pair1);
  BOOST_REQUIRE(con_pair1.size() == 1);
  BOOST_REQUIRE( RELTYPE1.compare(con_pair1.begin()->first) == 0 );
  BOOST_REQUIRE( con_pair1.find(RELTYPE1)->second.size() == 2 );
  for(list<og::og_schema_object_ptr>::iterator it = con_pair1.find(
        RELTYPE1)->second.begin();
      it != con_pair1.find(RELTYPE1)->second.end(); ++it)
  {
    BOOST_REQUIRE( (*it)->get_id() == schm_obj2->get_id()
                   || (*it)->get_id() == schm_obj4->get_id() );
  }

  map<string, list<og::og_schema_object_ptr>> con_pair2;
  o1->get_connectable_from(&con_pair2);
  BOOST_REQUIRE(con_pair2.size() == 0);

  map<string, list<og::og_schema_object_ptr>> con_pair3;
  o2->get_connectable_to(&con_pair3);
  BOOST_REQUIRE(con_pair3.size() == 0);

  map<string, list<og::og_schema_object_ptr>> con_pair4;
  o2->get_connectable_from(&con_pair4);
  BOOST_REQUIRE(con_pair4.size() == 1);
  BOOST_REQUIRE( RELTYPE1.compare(con_pair4.begin()->first) == 0 );
  BOOST_REQUIRE( con_pair4.find(RELTYPE1)->second.size() == 1 );
  for(list<og::og_schema_object_ptr>::iterator it = con_pair4.find(
        RELTYPE1)->second.begin();
      it != con_pair4.find(RELTYPE1)->second.end(); ++it)
  {
    std::cout << (*it)->get_id() << std::endl;
    std::cout << schm_obj1->get_id() << std::endl;

    BOOST_REQUIRE( (*it)->get_id() == schm_obj1->get_id() );
  }

}

// 2 schema relation
BOOST_AUTO_TEST_CASE( session_1005 )
{
#ifdef WINDOWS
  og::core::CrtCheckMemory __check__;
#endif

  // initialize db
  og::og_session cleaned_session_;
  cleaned_session_.open(DBPATH);
  cleaned_session_.purge();
  cleaned_session_.schema()->purge();

  string OTYPE1 = "type_session_1005_1";
  string ONAME1 = "name_session_1005_1";
  string OTYPE2 = "type_session_1005_2";
  string ONAME2 = "name_session_1005_2";
  string OTYPE3 = "type_session_1005_3";
  string ONAME3 = "name_session_1005_3";
  string RELTYPE1 = "reltype_session_1004_1";

  // create schema obj
  og::og_schema_object_ptr schm_obj1 = cleaned_session_.schema()->create_object(
                                         OTYPE1,
                                         ONAME1);
  og::og_schema_object_ptr schm_obj2 = cleaned_session_.schema()->create_object(
                                         OTYPE2,
                                         ONAME2);
  og::og_schema_object_ptr schm_obj3 = cleaned_session_.schema()->create_object(
                                         OTYPE3,
                                         ONAME3);

  schm_obj1->connect_to(schm_obj2, RELTYPE1);
  schm_obj1->connect_to(schm_obj3, RELTYPE1);

  og::og_session_object_ptr o1 = cleaned_session_.create_object(schm_obj1);
  og::og_session_object_ptr o2 = cleaned_session_.create_object(schm_obj2);
  og::og_session_object_ptr o3 = cleaned_session_.create_object(schm_obj2);

  og::og_session_relation_ptr r1 = o1->connect_to(o2, RELTYPE1);
  og::og_session_relation_ptr r2 = o1->connect_to(o3, RELTYPE1);

  BOOST_REQUIRE(RELTYPE1.compare( r1->get_schema_relation_type() ) == 0);
  BOOST_REQUIRE(RELTYPE1.compare( r2->get_schema_relation_type() ) == 0);
}

// parameter test (integer)
BOOST_AUTO_TEST_CASE(session_1006)
{
#ifdef WINDOWS
  og::core::CrtCheckMemory __check__;
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

  og::og_schema_parameter_ptr test1 =
    cleaned_session_.schema()->create_parameter("hoge_type1", "comment1",
        &type1);
  og::og_schema_parameter_ptr test2 =
    cleaned_session_.schema()->create_parameter("hoge_type2", "comment2",
        &type2, 3, 2, 4);

  BOOST_REQUIRE_EQUAL(test1->get_comment(), "comment1");
  BOOST_REQUIRE_EQUAL(test1->get_type(), "hoge_type1");

  BOOST_REQUIRE_EQUAL(test2->get_comment(), "comment2");
  BOOST_REQUIRE_EQUAL(test2->get_type(), "hoge_type2");

  list<boost::tuple<string, og::og_schema_parameter_ptr>> param_name_types0;
  p1->get_parameters(&param_name_types0);

  BOOST_REQUIRE_EQUAL(param_name_types0.size(), 0);

  // apply parameter to schema_object
  p1->add_parameter_definition("H1", test1);
  p1->add_parameter_definition("H2", test2);

  // apply parameter to schema_relation
  rel_ptr->add_parameter_definition("H3", test1);
  rel_ptr->add_parameter_definition("H4", test2);

  {
    list<boost::tuple<string, og::og_schema_parameter_ptr>> param_name_types1;
    p1->get_parameters(&param_name_types1);

    BOOST_REQUIRE_EQUAL(param_name_types1.size(), 2);

    bool h1_hit = false;
    bool h2_hit = false;
    list<boost::tuple<string, og::og_schema_parameter_ptr>>::iterator it
        = param_name_types1.begin();
    for (; it != param_name_types1.end(); it++)
    {
      if (it->get<0>().compare("H1") == 0)
      {
        h1_hit = true;
      }
      else if (it->get<0>().compare("H2") == 0)
      {
        h2_hit = true;
      }
      else
      {
        BOOST_FAIL("test failed.");
      }
    }
    BOOST_REQUIRE_EQUAL(h1_hit, true);
    BOOST_REQUIRE_EQUAL(h2_hit, true);
  }

  // session object test
  {
    og::og_session_object_ptr o1 = cleaned_session_.create_object(p1);
    o1->set_parameter_value<int>("H1", 3);
//    o1->sync();

    optional<og::og_session_object_ptr> o1_2 = cleaned_session_.get_object(
          o1->get_id());

    if (!o1_2) { BOOST_FAIL("null"); }

    int h1;
    o1_2.get()->get_parameter_value<int>("H2", &h1);
    BOOST_REQUIRE_EQUAL(h1, 2);

    std::list<int> h2;

    o1_2.get()->get_parameter_values<int>("H2", &h2);
    BOOST_REQUIRE_EQUAL(h2.size(), 3);

    BOOST_FOREACH(int &i, h2)
    {
      BOOST_REQUIRE_EQUAL(i, 2);
    }

    h2.push_back(1);

    o1_2.get()->set_parameter_values<int>("H2", h2);
    BOOST_REQUIRE_EQUAL(h2.size(), 4);
    h2.clear();

    o1_2.get()->get_parameter_values<int>("H2", &h2);
    BOOST_REQUIRE_EQUAL(h2.size(), 4);
    int row = 0;
    BOOST_FOREACH(int &i, h2)
    {
      if (row == 0) { BOOST_REQUIRE_EQUAL(i, 2); }
      if (row == 1) { BOOST_REQUIRE_EQUAL(i, 2); }
      if (row == 2) { BOOST_REQUIRE_EQUAL(i, 2); }
      if (row == 3) { BOOST_REQUIRE_EQUAL(i, 1); }
      row++;
    }
  }

  // session releation
  {
    og::og_session_object_ptr o1 = cleaned_session_.create_object(p1);
    og::og_session_object_ptr o2 = cleaned_session_.create_object(p2);

    og::og_session_relation_ptr o1o2 = o1->connect_to(o2, RELTYPE);
//    o1o2->sync();

    optional<og::og_session_relation_ptr> o1_2 = cleaned_session_.get_relation(
          o1o2->get_id());

    if (!o1_2) { BOOST_FAIL("null"); }

    int h1;
    o1_2.get()->get_parameter_value<int>("H3", &h1);
    BOOST_REQUIRE_EQUAL(h1, 1);

    std::list<int> h2;
    o1_2.get()->get_parameter_values<int>("H4", &h2);
    BOOST_REQUIRE_EQUAL(h2.size(), 3);

    BOOST_FOREACH(int &i, h2)
    {
      BOOST_REQUIRE_EQUAL(i, 2);
    }

    h2.push_back(3);

    o1_2.get()->set_parameter_values<int>("H4", h2);
    BOOST_REQUIRE_EQUAL(h2.size(), 4);

    h2.clear();
    o1_2.get()->get_parameter_values<int>("H4", &h2);
    BOOST_REQUIRE_EQUAL(h2.size(), 4);
    int row = 0;
    BOOST_FOREACH(int &i, h2)
    {
      if (row == 0) { BOOST_REQUIRE_EQUAL(i, 2); }
      if (row == 1) { BOOST_REQUIRE_EQUAL(i, 2); }
      if (row == 2) { BOOST_REQUIRE_EQUAL(i, 2); }
      if (row == 3) { BOOST_REQUIRE_EQUAL(i, 3); }
      row++;
    }
  }
}


// parameter test (real)
BOOST_AUTO_TEST_CASE(session_1007)
{
#ifdef WINDOWS
  og::core::CrtCheckMemory __check__;
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

  og::og_real type1;
  type1.default_value_ = 1.141;
  type1.warn_max_ = 3;
  type1.system_max_ = 3;
  type1.warn_min_ = 1;
  type1.system_min_ = 1;

  og::og_real type2;
  type2.default_value_ = 2.236;
  type2.warn_max_ = 3;
  type2.system_max_ = 3;
  type2.warn_min_ = 1;
  type2.system_min_ = 1;

  og::og_schema_parameter_ptr ptest1 =
    cleaned_session_.schema()->create_parameter("hoge_type1", "comment1",
        &type1);
  og::og_schema_parameter_ptr ptest2 =
    cleaned_session_.schema()->create_parameter("hoge_type2", "comment2",
        &type2, 3, 2, 4);

  BOOST_REQUIRE_EQUAL(ptest1->get_comment(), "comment1");
  BOOST_REQUIRE_EQUAL(ptest1->get_type(), "hoge_type1");

  BOOST_REQUIRE_EQUAL(ptest2->get_comment(), "comment2");
  BOOST_REQUIRE_EQUAL(ptest2->get_type(), "hoge_type2");

  list<boost::tuple<string, og::og_schema_parameter_ptr>> param_name_types0;
  p1->get_parameters(&param_name_types0);

  BOOST_REQUIRE_EQUAL(param_name_types0.size(), 0);

  // apply parameter to schema_object
  p1->add_parameter_definition("H1", ptest1);
  p1->add_parameter_definition("H2", ptest2);

  // apply parameter to schema_relation
  rel_ptr->add_parameter_definition("H3", ptest1);
  rel_ptr->add_parameter_definition("H4", ptest2);

  {
    list<boost::tuple<string, og::og_schema_parameter_ptr>> param_name_types1;
    p1->get_parameters(&param_name_types1);

    BOOST_REQUIRE_EQUAL(param_name_types1.size(), 2);

    bool h1_hit = false;
    bool h2_hit = false;
    list<boost::tuple<string, og::og_schema_parameter_ptr>>::iterator it
        = param_name_types1.begin();
    for (; it != param_name_types1.end(); it++)
    {
      if (it->get<0>().compare("H1") == 0)
      {
        h1_hit = true;
      }
      else if (it->get<0>().compare("H2") == 0)
      {
        h2_hit = true;
      }
      else
      {
        BOOST_FAIL("test failed.");
      }
    }
    BOOST_REQUIRE_EQUAL(h1_hit, true);
    BOOST_REQUIRE_EQUAL(h2_hit, true);
  }

  // session object test
  {
    og::og_session_object_ptr o1 = cleaned_session_.create_object(p1);
    o1->set_parameter_value<double>("H1", 1.1234);
    //o1->sync();

    optional<og::og_session_object_ptr> o1_2 = cleaned_session_.get_object(
          o1->get_id());

    if (!o1_2) { BOOST_FAIL("null"); }

    double h1;
    o1_2.get()->get_parameter_value<double>("H1", &h1);
    BOOST_REQUIRE_EQUAL(h1, 1.1234);

    std::list<double> h2;
    o1_2.get()->get_parameter_values<double>("H2", &h2);

    BOOST_REQUIRE_EQUAL(h2.size(), 3);

    BOOST_FOREACH(double &i, h2)
    {
      BOOST_REQUIRE_EQUAL(i, 2.236);
    }

    h2.push_back(1.1234);

    o1_2.get()->set_parameter_values<double>("H2", h2);

    h2.clear();
    o1_2.get()->get_parameter_values<double>("H2", &h2);
    BOOST_REQUIRE_EQUAL(h2.size(), 4);
    int row = 0;
    BOOST_FOREACH(double &i, h2)
    {
      if (row == 0) { BOOST_REQUIRE_EQUAL(i, 2.236); }
      if (row == 1) { BOOST_REQUIRE_EQUAL(i, 2.236); }
      if (row == 2) { BOOST_REQUIRE_EQUAL(i, 2.236); }
      if (row == 3) { BOOST_REQUIRE_EQUAL(i, 1.1234); }
      row++;
    }

  }

  // session releation
  {
    og::og_session_object_ptr o1 = cleaned_session_.create_object(p1);
    og::og_session_object_ptr o2 = cleaned_session_.create_object(p2);

    og::og_session_relation_ptr o1o2 = o1->connect_to(o2, RELTYPE);
    //o1o2->sync();

    optional<og::og_session_relation_ptr> o1_2 = cleaned_session_.get_relation(
          o1o2->get_id());

    if (!o1_2) { BOOST_FAIL("null"); }

    double h1;
    o1_2.get()->get_parameter_value<double>("H3", &h1);
    BOOST_REQUIRE_EQUAL(h1, 1.141);

    std::list<double> h2;
    o1_2.get()->get_parameter_values<double>("H4", &h2);

    BOOST_REQUIRE_EQUAL(h2.size(), 3);

    BOOST_FOREACH(double &i, h2)
    {
      BOOST_REQUIRE_EQUAL(i, 2.236);
    }

    h2.push_back(1.1234);

    o1_2.get()->set_parameter_values<double>("H4", h2);

    h2.clear();
    o1_2.get()->get_parameter_values<double>("H4", &h2);
    BOOST_REQUIRE_EQUAL(h2.size(), 4);
    int row = 0;
    BOOST_FOREACH(double &i, h2)
    {
      if (row == 0) { BOOST_REQUIRE_EQUAL(i, 2.236); }
      if (row == 1) { BOOST_REQUIRE_EQUAL(i, 2.236); }
      if (row == 2) { BOOST_REQUIRE_EQUAL(i, 2.236); }
      if (row == 3) { BOOST_REQUIRE_EQUAL(i, 1.1234); }
      row++;
    }
  }
}

// parameter test text
BOOST_AUTO_TEST_CASE(session_1008)
{
#ifdef WINDOWS
  og::core::CrtCheckMemory __check__;
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

  og::og_text ptype1;
  ptype1.default_value_ = std::string("foo");
  ptype1.warn_max_ = 10;
  ptype1.system_max_ = 10;
  ptype1.warn_min_ = 1;
  ptype1.system_min_ = 1;

  og::og_text ptype2;
  ptype2.default_value_ = std::string("bar");
  ptype2.warn_max_ = 10;
  ptype2.system_max_ = 10;
  ptype2.warn_min_ = 1;
  ptype2.system_min_ = 1;

  // array size is set to 1 implicitly
  og::og_schema_parameter_ptr ptest1 =
    cleaned_session_.schema()->create_parameter("hoge_type1", "comment1",
        &ptype1);
  // array size is set to 3 explicitly
  og::og_schema_parameter_ptr ptest2 =
    cleaned_session_.schema()->create_parameter("hoge_type2", "comment2",
        &ptype2, 3, 2, 4);

  BOOST_REQUIRE_EQUAL(ptest1->get_comment(), "comment1");
  BOOST_REQUIRE_EQUAL(ptest1->get_type(), "hoge_type1");

  BOOST_REQUIRE_EQUAL(ptest2->get_comment(), "comment2");
  BOOST_REQUIRE_EQUAL(ptest2->get_type(), "hoge_type2");

  list<boost::tuple<string, og::og_schema_parameter_ptr>> param_name_types0;
  p1->get_parameters(&param_name_types0);

  BOOST_REQUIRE_EQUAL(param_name_types0.size(), 0);

  // apply parameter to schema_object
  p1->add_parameter_definition("H1", ptest1);
  p1->add_parameter_definition("H2", ptest2);

  // apply parameter to schema_relation
  rel_ptr->add_parameter_definition("H3", ptest1);
  rel_ptr->add_parameter_definition("H4", ptest2);

  // test definition
  {
    list<boost::tuple<string, og::og_schema_parameter_ptr>> param_name_types1;
    p1->get_parameters(&param_name_types1);

    BOOST_REQUIRE_EQUAL(param_name_types1.size(), 2);

    bool h1_hit = false;
    bool h2_hit = false;
    list<boost::tuple<string, og::og_schema_parameter_ptr>>::iterator it
        = param_name_types1.begin();
    for (; it != param_name_types1.end(); it++)
    {
      if (it->get<0>().compare("H1") == 0)
      {
        h1_hit = true;
      }
      else if (it->get<0>().compare("H2") == 0)
      {
        h2_hit = true;
      }
      else
      {
        BOOST_FAIL("test failed.");
      }
    }
    BOOST_REQUIRE_EQUAL(h1_hit, true);
    BOOST_REQUIRE_EQUAL(h2_hit, true);
  }

  // session object test
  {
    og::og_session_object_ptr o1 = cleaned_session_.create_object(p1);
    o1->set_parameter_value<string>("H1", std::string("aaa"));
//    o1->sync();

    optional<og::og_session_object_ptr> o1_2 = cleaned_session_.get_object(
          o1->get_id());

    if (!o1_2) { BOOST_FAIL("null"); }

    std::string h1;
    o1_2->get()->get_parameter_value<std::string>("H1", &h1);
    BOOST_REQUIRE_EQUAL(h1, std::string("aaa"));

    std::list<std::string> h2;

    o1_2.get()->get_parameter_values<std::string>("H2", &h2);

    BOOST_FOREACH(std::string &i, h2)
    {
      BOOST_REQUIRE_EQUAL(i, "bar");
    }

    h2.push_back("foo");

    o1_2.get()->set_parameter_values<std::string>("H2", h2);

    h2.clear();
    o1_2.get()->get_parameter_values<std::string>("H2", &h2);
    BOOST_REQUIRE_EQUAL(h2.size(), 4);
    int row = 0;
    BOOST_FOREACH(std::string &i, h2)
    {
      if (row == 0) { BOOST_REQUIRE_EQUAL(i, "bar"); }
      if (row == 1) { BOOST_REQUIRE_EQUAL(i, "bar"); }
      if (row == 2) { BOOST_REQUIRE_EQUAL(i, "bar"); }
      if (row == 3) { BOOST_REQUIRE_EQUAL(i, "foo"); }
      row++;
    }
  }

  // session releation
  {
    og::og_session_object_ptr o1 = cleaned_session_.create_object(p1);
    og::og_session_object_ptr o2 = cleaned_session_.create_object(p2);

    og::og_session_relation_ptr o1o2 = o1->connect_to(o2, RELTYPE);
//    o1o2->sync();

    optional<og::og_session_relation_ptr> o1_2 = cleaned_session_.get_relation(
          o1o2->get_id());

    if (!o1_2) { BOOST_FAIL("null"); }

    std::string h1;
    o1_2->get()->get_parameter_value<std::string>("H3", &h1);
    BOOST_REQUIRE_EQUAL(h1, std::string("foo"));

    std::list<std::string> h2;

    o1_2.get()->get_parameter_values<std::string>("H4", &h2);

    BOOST_REQUIRE_EQUAL(h2.size(), 3);

    BOOST_FOREACH(std::string &i, h2)
    {
      BOOST_REQUIRE_EQUAL(i, "bar");
    }

    h2.push_back("foo");

    o1_2.get()->set_parameter_values<std::string>("H4", h2);

    h2.clear();
    o1_2.get()->get_parameter_values<std::string>("H4", &h2);
    BOOST_REQUIRE_EQUAL(h2.size(), 4);
    int row = 0;
    BOOST_FOREACH(std::string &i, h2)
    {
      if (row == 0) { BOOST_REQUIRE_EQUAL(i, "bar"); }
      if (row == 1) { BOOST_REQUIRE_EQUAL(i, "bar"); }
      if (row == 2) { BOOST_REQUIRE_EQUAL(i, "bar"); }
      if (row == 3) { BOOST_REQUIRE_EQUAL(i, "foo"); }
      row++;
    }
  }
}

// disconnect
BOOST_AUTO_TEST_CASE( session_1100 )
{
#ifdef WINDOWS
  og::core::CrtCheckMemory __check__;
#endif

  // initialize db
  og::og_session cleaned_session_;
  cleaned_session_.open(DBPATH);
  cleaned_session_.purge();
  cleaned_session_.schema()->purge();

  string OTYPE1 = "type_session_1005_1";
  string ONAME1 = "name_session_1005_1";
  string OTYPE2 = "type_session_1005_2";
  string ONAME2 = "name_session_1005_2";
  string RELTYPE1 = "reltype_session_1004_1";

  // create schema obj
  og::og_schema_object_ptr schm_obj1 = cleaned_session_.schema()->create_object(
                                         OTYPE1,
                                         ONAME1);
  og::og_schema_object_ptr schm_obj2 = cleaned_session_.schema()->create_object(
                                         OTYPE2,
                                         ONAME2);

  schm_obj1->connect_to(schm_obj2, RELTYPE1);

  og::og_session_object_ptr o1 = cleaned_session_.create_object(schm_obj1);
  og::og_session_object_ptr o2 = cleaned_session_.create_object(schm_obj2);

  og::og_session_relation_ptr r1 = o1->connect_to(o2, RELTYPE1);
  string did = r1->get_id();

  o1->disconnect(o2);
  optional<og::og_session_relation_ptr> disconnected =
    cleaned_session_.get_relation(did);

  BOOST_REQUIRE(!disconnected.is_initialized());
}

// COPY
BOOST_AUTO_TEST_CASE( session_1111 )
{
#ifdef WINDOWS
  og::core::CrtCheckMemory __check__;
#endif

  // initialize db
  og::og_session cleaned_session_;
  cleaned_session_.open(DBPATH);
  cleaned_session_.purge();
  cleaned_session_.schema()->purge();

  string OTYPE1 = "type_session_1005_1";
  string ONAME1 = "name_session_1005_1";
  string OTYPE2 = "type_session_1005_2";
  string ONAME2 = "name_session_1005_2";
  string RELTYPE1 = "reltype_session_1004_1";

  // create schema obj
  og::og_schema_object_ptr schm_obj1 = cleaned_session_.schema()->create_object(
                                         OTYPE1,
                                         ONAME1);
  og::og_schema_object_ptr schm_obj2 = cleaned_session_.schema()->create_object(
                                         OTYPE2,
                                         ONAME2);

  og::og_text text_type1;
  text_type1.default_value_ = std::string("foo");
  text_type1.warn_max_ = 10;
  text_type1.system_max_ = 10;
  text_type1.warn_min_ = 1;
  text_type1.system_min_ = 1;

  og::og_schema_parameter_ptr text_param1 =
    cleaned_session_.schema()->create_parameter("foo_type", "foo",
        &text_type1, 3, 2, 4);

  og::og_integer int_type1;
  int_type1.default_value_ = 1;
  int_type1.warn_max_ = 3;
  int_type1.system_max_ = 3;
  int_type1.warn_min_ = 1;
  int_type1.system_min_ = 1;

  og::og_schema_parameter_ptr int_param1 =
    cleaned_session_.schema()->create_parameter("bar_type", "bar",
        &int_type1, 3, 2, 4);

  og::og_real real_type1;
  real_type1.default_value_ = 1.141;
  real_type1.warn_max_ = 3;
  real_type1.system_max_ = 3;
  real_type1.warn_min_ = 1;
  real_type1.system_min_ = 1;

  og::og_schema_parameter_ptr real_param1 =
    cleaned_session_.schema()->create_parameter("hoge_type", "hoge",
        &real_type1, 1, 2, 3);

  schm_obj1->add_parameter_definition("foo1", text_param1);
  schm_obj1->add_parameter_definition("bar1", int_param1);
  schm_obj1->add_parameter_definition("hoge1", real_param1);

  schm_obj2->add_parameter_definition("foo2", text_param1);
  schm_obj2->add_parameter_definition("bar2", int_param1);
  schm_obj2->add_parameter_definition("hoge2", real_param1);

  og::og_schema_relation_ptr schm_rel = schm_obj1->connect_to(schm_obj2,
                                        RELTYPE1);

  schm_rel->add_parameter_definition("foo3", text_param1);
  schm_rel->add_parameter_definition("bar3", int_param1);
  schm_rel->add_parameter_definition("hoge3", real_param1);



  og::og_session_object_ptr o1 = cleaned_session_.create_object(schm_obj1);
  og::og_session_object_ptr o2 = cleaned_session_.create_object(schm_obj2);
  o1->set_instance_name("a");
  o1->set_parameter_value<string>("foo1", "foox1");
  o1->set_parameter_value<int>("bar1", 4);
  o1->set_parameter_value<double>("hoge1", 4.1);

  o2->set_instance_name("b");
  o2->set_parameter_value<string>("foo2", "foox2");
  o2->set_parameter_value<int>("bar2", 5);
  o2->set_parameter_value<double>("hoge2", 4.2);

  og::og_session_relation_ptr r1 = o1->connect_to(o2, RELTYPE1);
  r1->set_instance_name("c");
  r1->set_parameter_value<string>("foo3", "foox3");
  r1->set_parameter_value<int>("bar3", 6);
  r1->set_parameter_value<double>("hoge3", 4.3);

  // basic copy
  {
    og::og_session_object_ptr copied = o1.get()->copy_object();

	string t;
    copied->get_parameter_value<string>("foo1", &t);
    OG_LOG << t;

    BOOST_REQUIRE(
      og::core::og_session_object_comparer::compare(&cleaned_session_, o1, copied));
  }

  // recurive copy (copy direction 'to')
  {
    og::og_session_object_ptr copied = o1->copy_object(
                                         og::core::connection_direction_enum::direction_to);
    BOOST_REQUIRE(
      og::core::og_session_object_comparer::compare(&cleaned_session_, o1, copied));

    // childs : 1 object
    {
      list<og::og_session_object_ptr> childs;
      copied->get_connected_object_to(&childs);

      BOOST_REQUIRE_EQUAL(1, childs.size());
      BOOST_REQUIRE(
        og::core::og_session_object_comparer::compare(&cleaned_session_, o2,
            childs.front()));
    }

    // childs of copied : 1 (with relation)
    {
      list<boost::tuple<og::og_session_object_ptr,og::og_session_relation_ptr>>
          childs;
      copied->get_connected_to(&childs);

      BOOST_REQUIRE_EQUAL(1, childs.size());
      BOOST_REQUIRE(
        og::core::og_session_object_comparer::compare(&cleaned_session_, o2,
            childs.front().get<0>()));
      BOOST_REQUIRE(
        og::core::og_session_relation_comparer::compare(&cleaned_session_, r1,
            childs.front().get<1>()));
    }

    // parents of copied : 0
    {
      list<og::og_session_object_ptr> parents;
      copied->get_connected_object_from(&parents);
      BOOST_REQUIRE_EQUAL(0, parents.size());
    }
    // parents of copied : 0 (with relation)
    {
      list<boost::tuple<og::og_session_object_ptr,og::og_session_relation_ptr>>
          childs;
      copied->get_connected_from(&childs);

      BOOST_REQUIRE_EQUAL(0, childs.size());
    }

    // childs of copied : 1 object
    {
      list<og::og_session_object_ptr> childs;
      copied->get_connected_object(&childs);

      BOOST_REQUIRE_EQUAL(1, childs.size());
      BOOST_REQUIRE(
        og::core::og_session_object_comparer::compare(&cleaned_session_, o2,
            childs.front()));
    }
    // childs of copied : 1 (with relation)
    {
      list<boost::tuple<og::og_session_object_ptr,og::og_session_relation_ptr>>
          childs;
      copied->get_connected(&childs);

      BOOST_REQUIRE_EQUAL(1, childs.size());
      BOOST_REQUIRE(
        og::core::og_session_object_comparer::compare(&cleaned_session_, o2,
            childs.front().get<0>()));
      BOOST_REQUIRE(
        og::core::og_session_relation_comparer::compare(&cleaned_session_, r1,
            childs.front().get<1>()));
    }
  }

  {
    og::og_session_object_ptr copied = o2->copy_object(
                                         og::core::connection_direction_enum::direction_from);
    BOOST_REQUIRE(
      og::core::og_session_object_comparer::compare(&cleaned_session_, o2, copied));

    // childs : 0 object
    {
      list<og::og_session_object_ptr> childs;
      copied->get_connected_object_to(&childs);

      BOOST_REQUIRE_EQUAL(0, childs.size());
    }
    // childs of copied : 0 (with relation)
    {
      list<boost::tuple<og::og_session_object_ptr, og::og_session_relation_ptr>>
          childs;
      copied->get_connected_to(&childs);

      BOOST_REQUIRE_EQUAL(0, childs.size());
    }

    // parents of copied : 1 object
    {
      list<og::og_session_object_ptr> parents;
      copied->get_connected_object_from(&parents);

      BOOST_REQUIRE_EQUAL(1, parents.size());
      BOOST_REQUIRE(
        og::core::og_session_object_comparer::compare(&cleaned_session_, o1,
            parents.front()));
    }
    // parents of copied : 1 (with relation)
    {
      list<boost::tuple<og::og_session_object_ptr, og::og_session_relation_ptr>>
          parents;
      copied->get_connected_from(&parents);

      BOOST_REQUIRE_EQUAL(1, parents.size());
      BOOST_REQUIRE(
        og::core::og_session_object_comparer::compare(&cleaned_session_, o1,
            parents.front().get<0>()));
      BOOST_REQUIRE(
        og::core::og_session_relation_comparer::compare(&cleaned_session_, r1,
            parents.front().get<1>()));
    }

    // parents of copied : 1 object
    {
      list<og::og_session_object_ptr> parents;
      copied->get_connected_object(&parents);

      BOOST_REQUIRE_EQUAL(1, parents.size());
      BOOST_REQUIRE(
        og::core::og_session_object_comparer::compare(&cleaned_session_, o1,
            parents.front()));
    }
    // childs of copied : 1 (with relation)
    {
      list<boost::tuple<og::og_session_object_ptr, og::og_session_relation_ptr>>
          childs;
      copied->get_connected(&childs);

      BOOST_REQUIRE_EQUAL(1, childs.size());
      BOOST_REQUIRE(
        og::core::og_session_object_comparer::compare(&cleaned_session_, o1,
            childs.front().get<0>()));
      BOOST_REQUIRE(
        og::core::og_session_relation_comparer::compare(&cleaned_session_, r1,
            childs.front().get<1>()));
    }
  }
}

BOOST_AUTO_TEST_SUITE_END()
#endif // TEST_OG_SESSION
