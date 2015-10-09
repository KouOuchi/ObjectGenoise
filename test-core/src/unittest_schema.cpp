#include "fixtures.h"
#include "utility.h"

#ifdef TEST_OG_SCHEMA

BOOST_FIXTURE_TEST_SUITE(schema, fixture_clean_session);

void _check_schema_object(og::og_schema_object_ptr _soptr, string _oid,
                          string _otype,
                          string _oname)
{
  // type check
  BOOST_REQUIRE_EQUAL(_soptr->get_id(), _oid);
  BOOST_REQUIRE_EQUAL(_soptr->get_name(), _oname);
  BOOST_REQUIRE_EQUAL(_soptr->get_type(), _otype);
}

// schema basics
BOOST_AUTO_TEST_CASE(schema_0001)
{
#ifdef WINDOWS
	og::core::CrtCheckMemory __check__;
#endif

	// initialize db
  og::og_session cleaned_session_;
  cleaned_session_.open(DBPATH);

  list<string> ts;
  list < og::og_session_object_ptr> objs;
  ts.push_back(og::og_schema::schema_property_object_type());

  cleaned_session_.get_object_by_type(ts, &objs);

  BOOST_REQUIRE_EQUAL(objs.size(), 1);

  int val;
  objs.front()->get_parameter_value<int>(string(og::og_schema::schema_property_core_revision()), &val);
  BOOST_REQUIRE_EQUAL(val, 0);

}


BOOST_AUTO_TEST_CASE(schema_999)
{
#ifdef WINDOWS
  og::core::CrtCheckMemory __check__;
#endif

  // initialize db
  og::og_session cleaned_session_;
  cleaned_session_.open(DBPATH);
  cleaned_session_.purge();
  cleaned_session_.schema()->purge();

  string OTYPE = "Document";
  string ONAME = "Dcument-Name";
  og::og_schema_object_ptr soptr = cleaned_session_.schema()->create_object(OTYPE,
                                   ONAME);
}

BOOST_AUTO_TEST_CASE( schema_1000 )
{
#ifdef WINDOWS
  og::core::CrtCheckMemory __check__;
#endif

  // initialize db
  og::og_session cleaned_session_;
  cleaned_session_.open(DBPATH);
  cleaned_session_.purge();
  cleaned_session_.schema()->purge();

  string OTYPE = "Document";
  string ONAME = "Dcument-Name";
  {
    og::og_schema_object_ptr soptr = cleaned_session_.schema()->create_object(OTYPE,
                                     ONAME);

	BOOST_REQUIRE_EQUAL(soptr->get_revision(), "0");
	soptr->revision_up();

    // instance check
    _check_schema_object(soptr, soptr->get_id(), OTYPE, ONAME);

    // get_object_id
    optional<og::og_schema_object_ptr> soptr2 =
      cleaned_session_.schema()->get_object(
        soptr->get_id());

    BOOST_REQUIRE(soptr2.is_initialized());

    _check_schema_object(soptr, soptr2.get()->get_id(), OTYPE, ONAME);
	BOOST_REQUIRE_EQUAL(soptr2.get()->get_revision(), "1");

    // get_object_id
    list<string> type_list;
    type_list.push_back(OTYPE);

    list<og::og_schema_object_ptr> soref_list1;
    cleaned_session_.schema()->get_object_by_type(type_list, &soref_list1);

    BOOST_FOREACH(og::og_schema_object_ptr &e2, soref_list1)
    {
      BOOST_REQUIRE(e2->get_type() == OTYPE);
      BOOST_REQUIRE(e2->get_name() == ONAME);
    }
  }
}

// over session and get/set comment/revision
BOOST_AUTO_TEST_CASE( schema_1001 )
{
#ifdef WINDOWS
  og::core::CrtCheckMemory __check__;
#endif

  // initialize db
  og::og_session cleaned_session_;
  cleaned_session_.open(DBPATH);
  cleaned_session_.purge();
  cleaned_session_.schema()->purge();

  string OTYPE = "Document1001";
  string ONAME = "Dcument-Name1001";
  string oid;
  {
    og::og_schema_object_ptr soptr = cleaned_session_.schema()->create_object(OTYPE,
                                     ONAME);
    soptr->set_comment("hogehoge");
    soptr->set_name("foobar");

    // get object id
    oid = soptr->get_id();
  }

  {
    og::og_session ses2;
    ses2.open(DBPATH);

    // get_object_id
    optional<og::og_schema_object_ptr> soptr2(ses2.schema()->get_object(oid));

    _check_schema_object(soptr2.get(), oid, OTYPE, "foobar");
    BOOST_REQUIRE_EQUAL(soptr2->get()->get_comment(), "hogehoge");
  }
}

// get object by type
BOOST_AUTO_TEST_CASE( schema_1002 )
{
#ifdef WINDOWS
  og::core::CrtCheckMemory __check__;
#endif

  // initialize db
  og::og_session cleaned_session_;
  cleaned_session_.open(DBPATH);
  cleaned_session_.purge();
  cleaned_session_.schema()->purge();

  string OTYPE1 = "Document1002a";
  string OTYPE2 = "Document1002b";
  string ONAME = "Dcument-Name1002";
  list<string> otype_list;
  otype_list.push_back(OTYPE1);
  otype_list.push_back(OTYPE2);
  {
    // get_object_id
    cleaned_session_.schema()->create_object(OTYPE1, ONAME);
    cleaned_session_.schema()->create_object(OTYPE2, ONAME);

    list<og::og_schema_object_ptr> obj_list1;
    cleaned_session_.schema()->get_object_by_type(otype_list, &obj_list1);

    BOOST_REQUIRE_EQUAL(obj_list1.size(), 2);

    list<string> types;
    cleaned_session_.schema()->get_object_type(&types);

    BOOST_REQUIRE(types.size() == 2);

    BOOST_FOREACH(string &t, types)
    {
      BOOST_REQUIRE(t.compare(OTYPE1) == 0 || t.compare(OTYPE2) == 0);;
    }
  }

  {
    og::og_session ses2;
    ses2.open(DBPATH);
    list<og::og_schema_object_ptr> obj_list1;
    ses2.schema()->get_object_by_type(otype_list, &obj_list1);

    BOOST_REQUIRE(obj_list1.size() == 2);

    BOOST_FOREACH(og::og_schema_object_ptr &o, obj_list1)
    {
      BOOST_REQUIRE(o->get_type() == OTYPE1 || o->get_type() == OTYPE2);
      BOOST_REQUIRE_EQUAL(o->get_name(), ONAME);
    }
  }
}

// get object by name
BOOST_AUTO_TEST_CASE( schema_1003 )
{
#ifdef WINDOWS
  og::core::CrtCheckMemory __check__;
#endif

  // initialize db
  og::og_session cleaned_session_;
  cleaned_session_.open(DBPATH);
  cleaned_session_.purge();
  cleaned_session_.schema()->purge();

  string OTYPE1 = "Document1003a";
  string OTYPE2 = "Document1003b";
  string ONAME1 = "Dcument-Name1003a";
  string ONAME2 = "Dcument-Name1003b";
  list<string> oname_list;
  oname_list.push_back(ONAME1);
  oname_list.push_back(ONAME2);
  {
    // get_object_id
    list<og::og_schema_object_ptr> obj_list2;

    cleaned_session_.schema()->create_object(OTYPE1, ONAME1);
    cleaned_session_.schema()->create_object(OTYPE2, ONAME2);

    cleaned_session_.schema()->get_object_by_name(oname_list, &obj_list2);

    BOOST_REQUIRE_EQUAL(obj_list2.size(), 2);
  }

  {
    og::og_session ses2;
    ses2.open(DBPATH);
    list<og::og_schema_object_ptr> obj_list2;
    ses2.schema()->get_object_by_name(oname_list, &obj_list2);

    BOOST_REQUIRE(obj_list2.size() == 2);

    BOOST_FOREACH(og::og_schema_object_ptr &o, obj_list2)
    {
      BOOST_REQUIRE((o->get_type() == OTYPE1 && o->get_name() == ONAME1) ||
                    (o->get_type() == OTYPE2 && o->get_name() == ONAME2));
    }
  }
}

// null/nothing test
BOOST_AUTO_TEST_CASE( schema_1005 )
{
#ifdef WINDOWS
  og::core::CrtCheckMemory __check__;
#endif

  // initialize db
  og::og_session cleaned_session_;
  cleaned_session_.open(DBPATH);
  cleaned_session_.purge();
  cleaned_session_.schema()->purge();

  string OTYPE = "Document";
  string ONAME = "Dcument-Name";
  string RELTYPE = "Document2000rel";
  string RELNAME = "Rel-Name2000";

  list<string> otype_list;
  list<string> rel_type_list;
  otype_list.push_back(OTYPE);
  rel_type_list.push_back(RELTYPE);

  og::og_schema_object_ptr obj_ptr = cleaned_session_.schema()->create_object(
                                       OTYPE,
                                       ONAME);

  string oid = obj_ptr->get_id();

  cleaned_session_.schema()->delete_object(oid);

  optional<og::og_schema_object_ptr> obj_ptr2 =
    cleaned_session_.schema()->get_object(
      oid);

  BOOST_CHECK(!obj_ptr2.is_initialized());
}

// relation object
BOOST_AUTO_TEST_CASE( schema_2000 )
{
#ifdef WINDOWS
  og::core::CrtCheckMemory __check__;
#endif

  // initialize db
  og::og_session cleaned_session_;
  cleaned_session_.open(DBPATH);
  cleaned_session_.purge();
  cleaned_session_.schema()->purge();

  string OTYPE1 = "Document2000from";
  string OTYPE2 = "Document2000to";
  string RELTYPE = "Document2000rel";
  string ONAME = "Dcument-Name2000";
  string RELNAME = "Rel-Name2000";
  list<string> otype_list;
  otype_list.push_back(OTYPE1);
  otype_list.push_back(OTYPE2);

  list<string> rel_type_list;
  rel_type_list.push_back(RELTYPE);

  string from_id, to_id, rel_id;

  {
    og::og_schema_object_ptr p1 = cleaned_session_.schema()->create_object(OTYPE1,
                                  ONAME);
    og::og_schema_object_ptr p2 = cleaned_session_.schema()->create_object(OTYPE2,
                                  ONAME);

    // store id
    from_id = p1->get_id();
    to_id = p2->get_id();
    og::og_schema_relation_ptr rel_ptr = p1->connect_to(p2, RELTYPE);

	BOOST_REQUIRE_EQUAL(rel_ptr->get_revision(), "0");

	rel_id = rel_ptr->get_id();

    rel_ptr->set_comment("hoge");
    rel_ptr->set_name(RELNAME);
	rel_ptr->revision_up();

	list<string> rels;
	cleaned_session_.schema()->get_relation_type(&rels);

	BOOST_REQUIRE_EQUAL(rels.size(), 1);
  }

  {
    list<og::og_schema_relation_ptr> rel_list;
    cleaned_session_.schema()->get_relation_by_type(rel_type_list, &rel_list);
    BOOST_REQUIRE(rel_list.size() == 1);

    BOOST_FOREACH(og::og_schema_relation_ptr &rp, rel_list)
    {
      BOOST_REQUIRE(rp->get_comment() == "hoge");
      BOOST_REQUIRE(rp->get_name() == RELNAME);
      BOOST_REQUIRE_EQUAL(rp->get_revision(), "1");
    }

    optional<og::og_schema_relation_ptr> rp1(
      cleaned_session_.schema()->get_relation(
        rel_id));

    BOOST_REQUIRE(rp1.is_initialized());
    BOOST_REQUIRE(rp1->get()->get_comment() == "hoge");
    BOOST_REQUIRE(rp1->get()->get_name() == RELNAME);

    optional<og::og_schema_object_ptr> o1(cleaned_session_.schema()->get_object(
                                            from_id));
    optional<og::og_schema_object_ptr> o2(cleaned_session_.schema()->get_object(
                                            to_id));

    list<og::og_schema_object_ptr> obj_list1;

    // get composite
    obj_list1.clear();
    o1->get()->get_connected_object(&obj_list1);
    BOOST_REQUIRE_MESSAGE(obj_list1.size() == 1, obj_list1.size());
    BOOST_FOREACH(og::og_schema_object_ptr &o, obj_list1)
    {
      BOOST_REQUIRE(o->get_id() == o2->get()->get_id());
    }

    // get composite
    obj_list1.clear();
    o1->get()->get_connected_object_to(&obj_list1);
    BOOST_REQUIRE_MESSAGE(obj_list1.size() == 1, obj_list1.size());
    BOOST_FOREACH(og::og_schema_object_ptr &o, obj_list1)
    {
      BOOST_REQUIRE(o->get_id() == o2->get()->get_id());
    }

    // get composite
    obj_list1.clear();
    o2->get()->get_connected_object_from(&obj_list1);
    BOOST_REQUIRE_MESSAGE(obj_list1.size() == 1, obj_list1.size());
    BOOST_FOREACH(og::og_schema_object_ptr &o, obj_list1)
    {
      BOOST_REQUIRE(o->get_id() == o1->get()->get_id());
    }

    // get composite
    obj_list1.clear();
    o1->get()->get_connected_object(rel_type_list, &obj_list1);
    BOOST_REQUIRE_MESSAGE(obj_list1.size() == 1, obj_list1.size());
    BOOST_FOREACH(og::og_schema_object_ptr &o, obj_list1)
    {
      BOOST_REQUIRE(o->get_id() == o2->get()->get_id());
    }

    obj_list1.clear();
    o1->get()->get_connected_object_to(rel_type_list, &obj_list1);
    BOOST_REQUIRE_MESSAGE(obj_list1.size() == 1, obj_list1.size());
    BOOST_FOREACH(og::og_schema_object_ptr &o, obj_list1)
    {
      BOOST_REQUIRE(o->get_id() == o2->get()->get_id());
    }

    // get composite
    obj_list1.clear();
    o2->get()->get_connected_object_from(rel_type_list, &obj_list1);
    BOOST_REQUIRE_MESSAGE(obj_list1.size() == 1, obj_list1.size());
    BOOST_FOREACH(og::og_schema_object_ptr &o, obj_list1)
    {
      BOOST_REQUIRE(o->get_id() == o1->get()->get_id());
    }

    // get composite but get nobody
    obj_list1.clear();
    o1->get()->get_connected_object_from(&obj_list1);
    BOOST_REQUIRE(obj_list1.size() == 0);

    // get composite but get nobody
    obj_list1.clear();
    o2->get()->get_connected_object_to(&obj_list1);
    BOOST_REQUIRE(obj_list1.size() == 0);

    // get composite but get nobody
    obj_list1.clear();
    o1->get()->get_connected_object_from(rel_type_list, &obj_list1);
    BOOST_REQUIRE(obj_list1.size() == 0);

    // get composite but get nobody
    obj_list1.clear();
    o2->get()->get_connected_object_to(rel_type_list, &obj_list1);
    BOOST_REQUIRE(obj_list1.size() == 0);

  }
}

// relation auto delete test
BOOST_AUTO_TEST_CASE( schema_2001 )
{
#ifdef WINDOWS
  og::core::CrtCheckMemory __check__;
#endif

  // initialize db
  og::og_session cleaned_session_;
  cleaned_session_.open(DBPATH);
  cleaned_session_.purge();
  cleaned_session_.schema()->purge();

  string OTYPE1 = "Document2001from";
  string OTYPE2 = "Document2001to";
  string RELTYPE = "Document2001rel";
  string ONAME = "Dcument-Name2001";
  string RELNAME = "Rel-Name2001";
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

  p2->delete_object();

  list<og::og_schema_object_ptr> obj_list1;

  // auto disconnect
  p1->get_connected_object_to(rel_type_list, &obj_list1);

  BOOST_REQUIRE(obj_list1.size() == 0);
}

// connected relation
BOOST_AUTO_TEST_CASE( schema_2002 )
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
  string RELTYPE2 = "Document2002rel2";
  string ONAME = "Dcument-Name2002";
  string RELNAME = "Rel-Name2002";
  list<string> otype_list;
  otype_list.push_back(OTYPE1);
  otype_list.push_back(OTYPE2);

  list<string> rel_type_list;
  rel_type_list.push_back(RELTYPE);

  og::og_schema_object_ptr p1 = cleaned_session_.schema()->create_object(OTYPE1,
                                ONAME);
  og::og_schema_object_ptr p2 = cleaned_session_.schema()->create_object(OTYPE2,
                                ONAME);
  og::og_schema_object_ptr p3 = cleaned_session_.schema()->create_object(OTYPE1,
                                ONAME);
  og::og_schema_object_ptr p4 = cleaned_session_.schema()->create_object(OTYPE2,
                                ONAME);

  og::og_schema_relation_ptr rel_ptr = p1->connect_to(p2, RELTYPE);
  og::og_schema_relation_ptr rel_ptr2 = p3->connect_to(p4, RELTYPE2);

  list<og::og_schema_relation_ptr> rel_list;
  cleaned_session_.schema()->get_connected_relation_to(
    p1->get_id(),
    rel_type_list,
    &rel_list);

  BOOST_REQUIRE(rel_list.size() == 1);
  BOOST_REQUIRE( (*(rel_list.begin()))->get_id() == rel_ptr->get_id() );

  rel_list.clear();
  cleaned_session_.schema()->get_connected_relation_from(
    p2->get_id(),
    rel_type_list,
    &rel_list);

  BOOST_REQUIRE(rel_list.size() == 1);
  BOOST_REQUIRE( (*(rel_list.begin()))->get_id() == rel_ptr->get_id() );

  rel_list.clear();
  cleaned_session_.schema()->get_connected_relation_to(
    p2->get_id(),
    rel_type_list,
    &rel_list);

  BOOST_REQUIRE(rel_list.size() == 0);


  rel_list.clear();
  cleaned_session_.schema()->get_connected_relation_from(
    p1->get_id(),
    rel_type_list,
    &rel_list);

  BOOST_REQUIRE(rel_list.size() == 0);
}

/*
BOOST_AUTO_TEST_CASE( schema_3001 )
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
  string RELTYPE2 = "Document2002rel2";
  string ONAME = "Dcument-Name2002";
  string RELNAME = "Rel-Name2002";
  list<string> otype_list;
  otype_list.push_back(OTYPE1);
  otype_list.push_back(OTYPE2);

  list<string> rel_type_list;
  rel_type_list.push_back(RELTYPE);

  og::og_schema_object_ptr p1 = cleaned_session_.schema()->create_object(OTYPE1,
                                ONAME);
  og::og_schema_object_ptr p2 = cleaned_session_.schema()->create_object(OTYPE2,
                                ONAME);
  og::og_schema_object_ptr p3 = cleaned_session_.schema()->create_object(OTYPE1,
                                ONAME);
  og::og_schema_object_ptr p4 = cleaned_session_.schema()->create_object(OTYPE2,
                                ONAME);

  og::og_schema_relation_ptr rel_ptr = p1->connect_to(p2, RELTYPE);
  og::og_schema_relation_ptr rel_ptr2 = p3->connect_to(p4, RELTYPE2);

  list<og::og_schema_relation_ptr> rel_list;
  cleaned_session_.schema()->get_connected_relation_to(
    p1->get_id(),
    rel_type_list,
    &rel_list);

  BOOST_REQUIRE(rel_list.size() == 1);
  BOOST_REQUIRE( (*(rel_list.begin()))->get_id() == rel_ptr->get_id() );

  rel_list.clear();
  cleaned_session_.schema()->get_connected_relation_from(
    p2->get_id(),
    rel_type_list,
    &rel_list);

  BOOST_REQUIRE(rel_list.size() == 1);
  BOOST_REQUIRE( (*(rel_list.begin()))->get_id() == rel_ptr->get_id() );

  rel_list.clear();
  cleaned_session_.schema()->get_connected_relation_to(
    p2->get_id(),
    rel_type_list,
    &rel_list);

  BOOST_REQUIRE(rel_list.size() == 0);


  rel_list.clear();
  cleaned_session_.schema()->get_connected_relation_from(
    p1->get_id(),
    rel_type_list,
    &rel_list);

  BOOST_REQUIRE(rel_list.size() == 0);
}
*/




BOOST_AUTO_TEST_SUITE_END()

#endif //TEST_OG_SCHEMA
