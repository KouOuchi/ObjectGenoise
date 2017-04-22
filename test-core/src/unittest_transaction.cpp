#include "fixtures.h"
#include "utility.h"

#ifdef TEST_OG_TRAN

BOOST_FIXTURE_TEST_SUITE(tran, fixture_clean_session);

BOOST_AUTO_TEST_CASE( transaction_1000 )
{
#ifdef _WINDOWS
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
  string id;
  {
    // create transaction
    og::og_transaction tran(cleaned_session_);

    // create schema obj
    og::og_schema_object_ptr schm_obj1 = cleaned_session_.schema()->create_object(OTYPE1,
                                  ONAME1);
    id = schm_obj1->get_id();
	schm_obj1->set_comment("aaa");

    og::og_schema_object_ptr schm_obj2 = cleaned_session_.schema()->create_object(OTYPE2,
                                  ONAME2);
    // connect
    schm_obj1->connect_to(schm_obj2, RELTYPE);
    tran.commit();
  }

  {
    // check
    optional<og::og_schema_object_ptr> schm_obj1 = cleaned_session_.schema()->get_object(
        id);
    BOOST_REQUIRE(schm_obj1.is_initialized());

    list<og::og_schema_object_ptr> schm_children;
    schm_obj1.get()->get_connected_object_to(reltype_list, &schm_children);
    BOOST_REQUIRE(schm_children.size() == 1);
	BOOST_REQUIRE_EQUAL(schm_obj1->get()->get_comment(), "aaa");
  }
}

BOOST_AUTO_TEST_CASE( transaction_1001 )
{
#ifdef _WINDOWS
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
  string id;
  string child_id;
  {
    // create transaction
    og::og_transaction tran(cleaned_session_);

    // create schema obj
    og::og_schema_object_ptr schm_obj1 = cleaned_session_.schema()->create_object(OTYPE1,
                                  ONAME1);
	schm_obj1->set_comment("aaa");
    id = schm_obj1->get_id();

    og::og_schema_object_ptr schm_obj2 = cleaned_session_.schema()->create_object(OTYPE2,
                                  ONAME2);
    // connect
    schm_obj1->connect_to(schm_obj2, RELTYPE);
    child_id = schm_obj2->get_id();
  }

  {
    // check
    optional<og::og_schema_object_ptr> schm_obj1 = cleaned_session_.schema()->get_object(
        id);
    BOOST_REQUIRE(!schm_obj1.is_initialized());

    optional<og::og_schema_object_ptr> schm_obj2 = cleaned_session_.schema()->get_object(
        child_id);
    BOOST_REQUIRE(!schm_obj2.is_initialized());
  }
}

// parameter and transaction
BOOST_AUTO_TEST_CASE(transaction_1002)
{
#ifdef _WINDOWS
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

  og::og_schema_object_ptr p1 = cleaned_session_.schema()->create_object(OTYPE1, ONAME);
  og::og_schema_object_ptr p2 = cleaned_session_.schema()->create_object(OTYPE2, ONAME);

  og::og_schema_relation_ptr rel_ptr = p1->connect_to(p2, RELTYPE);

  og::core::parameter_basetype_real type1;
  type1.default_value_ = 1.414;
  type1.warn_max_ = 3;
  type1.system_max_ = 3;
  type1.warn_min_ = 1;
  type1.system_min_ = 1;

  og::core::parameter_basetype_real type2;
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

  // session object test (explicit)
  {
    og::og_transaction tran(cleaned_session_);

    og::og_session_object_ptr o1 = cleaned_session_.create_object(p1);
    //o1->sync();

    std::string oid = o1->get_id();

    tran.rollback();

    optional<og::og_session_object_ptr> o1_2 = cleaned_session_.get_object(oid);

    // confirm rollback(discard object creation)
    BOOST_REQUIRE_EQUAL(o1_2.is_initialized(), false);
  }


  // session object test (implicit)
  {
    og::og_session_object_ptr o1 = cleaned_session_.create_object(p1);
    string oid(o1->get_id());
    {
      og::og_transaction tran(cleaned_session_);

      o1->set_parameter_value<double>("H1", 1.1234);
      o1->set_comment("aaa");

    } // rollback

    {
      optional<og::og_session_object_ptr> o1_2 = cleaned_session_.get_object(oid);

      BOOST_REQUIRE_EQUAL(o1_2.is_initialized(), true);

      double h1;
      o1_2.get()->get_parameter_value<double>("H2", &h1);
      // confirm rollback
      BOOST_REQUIRE_EQUAL(h1, 2.236);
	  BOOST_REQUIRE_EQUAL(o1_2.get()->get_comment(), "");
    }
  }

  // session object test (commit)
  {
    og::og_session_object_ptr o1 = cleaned_session_.create_object(p1);
    string oid(o1->get_id());
    {
      og::og_transaction tran(cleaned_session_);

      o1->set_parameter_value<double>("H1", 1.1234);
      o1->set_comment("aaa");

      tran.commit();
    } // commit;

    {
      optional<og::og_session_object_ptr> o1_2 = cleaned_session_.get_object(oid);

      BOOST_REQUIRE_EQUAL(o1_2.is_initialized(), true);

      double h1;
      o1_2.get()->get_parameter_value<double>("H2", &h1);
      // confirm commit
      BOOST_REQUIRE_EQUAL(h1, 2.236);
	  BOOST_REQUIRE_EQUAL(o1_2.get()->get_comment(), "aaa");

    }
  }

  // session relation test (explicit)
  {
    og::og_session_object_ptr o1 = cleaned_session_.create_object(p1);
    og::og_session_object_ptr o2 = cleaned_session_.create_object(p2);

    og::og_transaction tran(cleaned_session_);
    og::og_session_relation_ptr o1o2 = o1->connect_to(o2, RELTYPE);
    string relid(o1o2->get_id());


    tran.rollback();

    list<og::og_session_object_ptr> connected;
    o1->get_connected_object(&connected);
    BOOST_REQUIRE_EQUAL(connected.size(), 0);
  }

  // session relation test (implicit)
  {
    og::og_session_object_ptr o1 = cleaned_session_.create_object(p1);
    og::og_session_object_ptr o2 = cleaned_session_.create_object(p2);

    {
      og::og_transaction tran(cleaned_session_);
      og::og_session_relation_ptr o1o2 = o1->connect_to(o2, RELTYPE);
      string relid(o1o2->get_id());

    } //implicit rollback;

    list<og::og_session_object_ptr> connected;
    o1->get_connected_object(&connected);
    BOOST_REQUIRE_EQUAL(connected.size(), 0);
  }

    // session relation test (explicit)
  {
    og::og_session_object_ptr o1 = cleaned_session_.create_object(p1);
    og::og_session_object_ptr o2 = cleaned_session_.create_object(p2);

    og::og_transaction tran(cleaned_session_);
    og::og_session_relation_ptr o1o2 = o1->connect_to(o2, RELTYPE);
    string relid(o1o2->get_id());
    o1o2->set_comment("aaa");
    //o1o2->sync();

    tran.commit();

    list<og::og_session_object_ptr> connected;
    o1->get_connected_object(&connected);
	BOOST_REQUIRE_EQUAL(connected.size(), 1);
    BOOST_REQUIRE_EQUAL(o1o2.get()->get_comment(), "aaa");
  }

  // session object test (implicit)
  {
    og::og_session_object_ptr o1 = cleaned_session_.create_object(p1);
    og::og_session_object_ptr o2 = cleaned_session_.create_object(p2);
	og::og_session_relation_ptr o1o2 = o1->connect_to(o2, RELTYPE);

    string relid(o1o2->get_id());
    {
      og::og_transaction tran(cleaned_session_);

      o1o2->set_parameter_value<double>("H3", 2.2345);
      //o1o2->sync();
	  tran.rollback();
    } // explicit rollback

    {
      double h1;
	  optional<og::og_session_relation_ptr> o1o2_2 = cleaned_session_.get_relation(relid);
	  
      o1o2_2.get()->get_parameter_value<double>("H3", &h1);
      // confirm rollback
      BOOST_REQUIRE_EQUAL(h1, 1.414);
    }

    {
      og::og_transaction tran(cleaned_session_);

      o1o2->set_parameter_value<double>("H3", 2.2345);
      //o1o2->sync();
    } // implicit rollback

    {
      double h1;
	  optional<og::og_session_relation_ptr> o1o2_2 = cleaned_session_.get_relation(relid);
	  
      o1o2_2.get()->get_parameter_value<double>("H3", &h1);
      // confirm rollback
      BOOST_REQUIRE_EQUAL(h1, 1.414);
    }

	{
      og::og_transaction tran(cleaned_session_);

      o1o2->set_parameter_value<double>("H3", 1.1234);
      //o1o2->sync();
      tran.commit();
    } // commit;

    {
      double h1;
      o1o2.get()->get_parameter_value<double>("H3", &h1);
      // confirm commit
      BOOST_REQUIRE_EQUAL(h1, 1.1234);
    }
  }

}
BOOST_AUTO_TEST_SUITE_END()
#endif
