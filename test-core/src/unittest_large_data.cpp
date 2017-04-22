#include "fixtures.h"
#include "utility.h"

#ifndef TEST_OG_LARGE_DATA

BOOST_FIXTURE_TEST_SUITE(large_data, fixture_clean_session);

// parameter test
BOOST_AUTO_TEST_CASE(large_data_1001)
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
  //string RELNAME = "Rel-Name2002";
  list<string> otype_list;
  otype_list.push_back(OTYPE1);
  otype_list.push_back(OTYPE2);

  list<string> rel_type_list;
  rel_type_list.push_back(RELTYPE);

  og::og_schema_object_ptr p1 = cleaned_session_.schema()->create_object(OTYPE1, ONAME);
  og::og_schema_object_ptr p2 = cleaned_session_.schema()->create_object(OTYPE2, ONAME);


  for (int i = 0; i < 100; i++)
  {
    cleaned_session_.schema()->create_object(OTYPE2, ONAME);
  }

  list<string> oname_list; 
  oname_list.push_back(ONAME);
  list<og::og_schema_object_ptr> olist;
  cleaned_session_.schema()->get_object_by_name(oname_list, &olist);
  BOOST_REQUIRE_EQUAL(olist.size(), 102);
  olist.clear();

  cleaned_session_.schema()->get_object_by_type(otype_list, &olist);
  BOOST_REQUIRE_EQUAL(olist.size(), 102);

  og::og_schema_relation_ptr rel_ptr = p1->connect_to(p2, RELTYPE);

  og::og_integer type1;
  type1.default_value_ = 100;

  og::og_schema_parameter_ptr booltest1 =
    cleaned_session_.schema()->create_parameter("hoge_type1", "comment1",
        &type1, 1000, 2, 2000);

  // apply parameter to schema_object
  p1->add_parameter_definition("H1", booltest1);

  // apply parameter to schema_relation
  rel_ptr->add_parameter_definition("H2", booltest1);

  // session rollback
  {
    og::og_session_object_ptr o1 = cleaned_session_.create_object(p1);
    //o1->sync();

    optional<og::og_session_object_ptr> o1_2 = cleaned_session_.get_object(o1->get_id());
    if (!o1_2) { BOOST_FAIL("null"); }

    std::list<int> h1, h2, h3;
    o1_2.get()->get_parameter_values<int>("H1", &h1);

    BOOST_REQUIRE_EQUAL(h1.size(), 1000);
    BOOST_FOREACH(int &i, h1)
    {
      BOOST_REQUIRE_EQUAL(i, 100);
    }

    for (int i = 0; i < 99; i++)
    {
      h2.push_back(200);
    }

    o1_2.get()->set_parameter_values<int>("H1", h2);

    o1_2.get()->get_parameter_values<int>("H1", &h3);
    BOOST_REQUIRE_EQUAL(h3.size(), 99);
    BOOST_FOREACH(int &i, h3)
    {
      BOOST_REQUIRE_EQUAL(i, 200);
    }
  }



}
BOOST_AUTO_TEST_SUITE_END()
#endif
