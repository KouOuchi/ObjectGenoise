using System;
using System.Collections.Generic;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using og.net;

namespace test_og_net
{
    [TestClass]
    public class UnitTest_Session
    {
        og.net.OGSession cleaned_session_ = null;

        [TestInitialize]
        public void TestInitialize()
        {
            cleaned_session_ = new TestInitializer().initialize();
        }
        // basic
        [TestMethod]
        public void session_1000()
        {
            string OTYPE = "type_session_1000";
            string ONAME = "name_session_1000";
            List<string> oname_list = new List<string>(); oname_list.Add(ONAME);
            {
                OGSchemaObject schm_ptr =
                  cleaned_session_.schema().create_object(
                    OTYPE, ONAME);

                OGSessionObject sesn_ptr = cleaned_session_.create_object(
                                                       schm_ptr);

                Assert.IsTrue(sesn_ptr.get_schema_object_id() == schm_ptr.get_id());
                Assert.IsTrue(sesn_ptr.get_schema_object_type() == schm_ptr.get_type());
            }
        }

        // not found
        [TestMethod]
        public void session_1001()
        {
            string OTYPE = "type_session_1001";
            string ONAME = "name_session_1001";
            List<string> otype_list = new List<string>(); otype_list.Add(OTYPE);
            List<string> oname_list = new List<string>(); oname_list.Add(ONAME);
            {
                // create schema obj
                OGSchemaObject schm_obj = cleaned_session_.schema().create_object(
                                                      OTYPE,
                                                      ONAME);

                // create 2 session obj
                for (int i = 0; i < 2; i++)
                {
                    OGSessionObject sesn_ptr = cleaned_session_.create_object(schm_obj);
                    sesn_ptr.set_comment("hogehoge");
                    sesn_ptr.set_instance_name("v");
                    //sesn_ptr.set_revision("2");
                    //sesn_ptr.sync();
                }

                List<OGSessionObject> sesn_obj_list = new List<OGSessionObject>();
                cleaned_session_.get_object_by_type(otype_list, sesn_obj_list);

                Assert.IsTrue(sesn_obj_list.Count == 2);
                foreach (OGSessionObject r in sesn_obj_list)
                {
                    Assert.AreEqual(r.get_comment(), "hogehoge");
                    Assert.AreEqual(r.get_instance_name(), "v");
                    //Assert.IsTrue(r.get_revision() == "2");
                }
            }

            {
                // re open session
                OGSession ses2 = new OGSession();
                ses2.connect(TestInitializer.DBPATH);

                List<OGSessionObject> sesn_obj_list = new List<OGSessionObject>();
                ses2.get_object_by_type(otype_list, sesn_obj_list);

                Assert.IsTrue(sesn_obj_list.Count == 2);
                foreach (OGSessionObject r in sesn_obj_list)
                {
                    Assert.AreEqual(r.get_comment(), "hogehoge");
                    Assert.AreEqual(r.get_instance_name(), "v");
                    //Assert.IsTrue(r.get_revision() == "2");
                }
            }
        }

        // session relation basic test
        [TestMethod]
        public void session_1002()
        {
            string OTYPE1 = "type_session_1002_1";
            string ONAME1 = "name_session_1002_1";
            string OTYPE2 = "type_session_1002_2";
            string ONAME2 = "name_session_1002_2";
            string RELTYPE = "reltype_session_1002";
            string RELNAME = "relnamee_session_1002";
            List<string> otype_list1 = new List<string>(); otype_list1.Add(OTYPE1);
            List<string> oname_list1 = new List<string>(); oname_list1.Add(ONAME1);
            List<string> otype_list2 = new List<string>(); otype_list2.Add(OTYPE2);
            List<string> oname_list2 = new List<string>(); oname_list2.Add(ONAME2);
            List<string> reltype_list = new List<string>(); reltype_list.Add(RELTYPE);
            List<string> relname_list = new List<string>(); relname_list.Add(RELNAME);
            {
                // create schema obj
                OGSchemaObject schm_obj1 = cleaned_session_.schema().create_object(
                                                       OTYPE1,
                                                       ONAME1);
                OGSchemaObject schm_obj2 = cleaned_session_.schema().create_object(
                                                       OTYPE2,
                                                       ONAME2);
                schm_obj1.connect_to(schm_obj2, RELTYPE);

                OGSessionObject o1 = cleaned_session_.create_object(schm_obj1);
                OGSessionObject o2 = cleaned_session_.create_object(schm_obj2);

                Assert.IsTrue(o1.validate_connect_to(o2));
                Assert.IsTrue(o1.validate_connect_to(o2, RELTYPE));
                Assert.IsTrue(!o2.validate_connect_to(o1));
                Assert.IsTrue(!o2.validate_connect_to(o1, RELTYPE));
                Assert.IsTrue(!o1.validate_connect_to(o1));
                Assert.IsTrue(!o1.validate_connect_to(o1, RELTYPE));
                Assert.IsTrue(!o2.validate_connect_to(o2));
                Assert.IsTrue(!o2.validate_connect_to(o2, RELTYPE));

                Assert.IsTrue(!o1.validate_connect_from(o2));
                Assert.IsTrue(!o1.validate_connect_from(o2, RELTYPE));
                Assert.IsTrue(o2.validate_connect_from(o1));
                Assert.IsTrue(o2.validate_connect_from(o1, RELTYPE));
                Assert.IsTrue(!o1.validate_connect_from(o1));
                Assert.IsTrue(!o1.validate_connect_from(o1, RELTYPE));
                Assert.IsTrue(!o2.validate_connect_from(o2));
                Assert.IsTrue(!o2.validate_connect_from(o2, RELTYPE));

                o1.connect_to(o2, RELTYPE);

                List<OGSessionObject> sesn_obj_list = new List<OGSessionObject>();
                o1.get_connected_object_to(sesn_obj_list);
                Assert.IsTrue(sesn_obj_list.Count == 1);
                Assert.IsTrue(sesn_obj_list[0].get_id() == o2.get_id());

                sesn_obj_list.Clear();
                o1.get_connected_object_from(sesn_obj_list);
                Assert.IsTrue(sesn_obj_list.Count == 0);

                sesn_obj_list.Clear();
                o2.get_connected_object_from(sesn_obj_list);
                Assert.IsTrue(sesn_obj_list.Count == 1);
                Assert.IsTrue(sesn_obj_list[0].get_id() == o1.get_id());

                sesn_obj_list.Clear();
                o2.get_connected_object_to(sesn_obj_list);
                Assert.IsTrue(sesn_obj_list.Count == 0);

                sesn_obj_list.Clear();
                o2.get_connected_object(sesn_obj_list);
                Assert.IsTrue(sesn_obj_list.Count == 1);
                Assert.IsTrue(sesn_obj_list[0].get_id() == o1.get_id());

                sesn_obj_list.Clear();
                o2.get_connected_object(reltype_list, sesn_obj_list);
                Assert.IsTrue(sesn_obj_list.Count == 1);
                Assert.IsTrue(sesn_obj_list[0].get_id() == o1.get_id());

                sesn_obj_list.Clear();
                o1.get_connected_object(sesn_obj_list);
                Assert.IsTrue(sesn_obj_list.Count == 1);
                Assert.IsTrue(sesn_obj_list[0].get_id() == o2.get_id());

                sesn_obj_list.Clear();
                o1.get_connected_object(reltype_list, sesn_obj_list);
                Assert.IsTrue(sesn_obj_list.Count == 1);
                Assert.IsTrue(sesn_obj_list[0].get_id() == o2.get_id());

                List<OGSessionRelation> sesn_rel_list = new List<OGSessionRelation>();
                cleaned_session_.get_relation_by_type(reltype_list, sesn_rel_list);
                Assert.IsTrue(sesn_rel_list.Count == 1);

                sesn_rel_list.Clear();
                cleaned_session_.get_relation_by_name(relname_list, sesn_rel_list);
                Assert.IsTrue(sesn_rel_list.Count == 1);

                o1.disconnect(o2);

                sesn_obj_list.Clear();
                o1.get_connected_object_to(sesn_obj_list);
                Assert.IsTrue(sesn_obj_list.Count == 0);

                sesn_obj_list.Clear();
                o1.get_connected_object_from(sesn_obj_list);
                Assert.IsTrue(sesn_obj_list.Count == 0);

                sesn_obj_list.Clear();
                o2.get_connected_object_from(sesn_obj_list);
                Assert.IsTrue(sesn_obj_list.Count == 0);

                sesn_obj_list.Clear();
                o2.get_connected_object_to(sesn_obj_list);
                Assert.IsTrue(sesn_obj_list.Count == 0);

                sesn_obj_list.Clear();
                o2.get_connected_object(sesn_obj_list);
                Assert.IsTrue(sesn_obj_list.Count == 0);

                sesn_obj_list.Clear();
                o2.get_connected_object(reltype_list, sesn_obj_list);
                Assert.IsTrue(sesn_obj_list.Count == 0);

                sesn_obj_list.Clear();
                o1.get_connected_object(sesn_obj_list);
                Assert.IsTrue(sesn_obj_list.Count == 0);

                sesn_obj_list.Clear();
                o1.get_connected_object(reltype_list, sesn_obj_list);
                Assert.IsTrue(sesn_obj_list.Count == 0);

                sesn_rel_list.Clear();
                cleaned_session_.get_relation_by_type(reltype_list, sesn_rel_list);
                Assert.IsTrue(sesn_rel_list.Count == 0);

                sesn_rel_list.Clear();
                cleaned_session_.get_relation_by_name(relname_list, sesn_rel_list);
                Assert.IsTrue(sesn_rel_list.Count == 0);
            }
        }

#if TEST_REQUIRE_THROW
// session obj with no schema
        [TestMethod]
        public void session_1003()
{
  OGSchemaObject o(new og::og_schema_object(
	  cleaned_session_.schema().get()));
  BOOST_REQUIRE_THROW( cleaned_session_.create_object(o), og::core::exception );
}
#endif

        // connectable relation
        [TestMethod]
        public void session_1004()
        {
            string OTYPE1 = "type_session_1004_1";
            string ONAME1 = "name_session_1004_1";
            string OTYPE2 = "type_session_1004_2";
            string ONAME2 = "name_session_1004_2";
            string RELTYPE1 = "reltype_session_1004_1";
            string RELTYPE2 = "reltype_session_1004_2";
            List<string> otype_list1 = new List<string>(); otype_list1.Add(OTYPE1);
            List<string> oname_list1 = new List<string>(); oname_list1.Add(ONAME1);
            List<string> otype_list2 = new List<string>(); otype_list2.Add(OTYPE2);
            List<string> oname_list2 = new List<string>(); oname_list2.Add(ONAME2);
            List<string> reltype_list = new List<string>(); reltype_list.Add(RELTYPE1);

            // create schema obj
            OGSchemaObject schm_obj1 = cleaned_session_.schema().create_object(
                                                   OTYPE1,
                                                   ONAME1);
            OGSchemaObject schm_obj2 = cleaned_session_.schema().create_object(
                                                   OTYPE2,
                                                   ONAME2);
            OGSchemaObject schm_obj3 = cleaned_session_.schema().create_object(
                                                   OTYPE1,
                                                   ONAME1);
            OGSchemaObject schm_obj4 = cleaned_session_.schema().create_object(
                                                   OTYPE2,
                                                   ONAME2);
            schm_obj1.connect_to(schm_obj2, RELTYPE1);
            schm_obj1.connect_to(schm_obj4, RELTYPE1);
            schm_obj3.connect_to(schm_obj4, RELTYPE2);

            OGSessionObject o1 = cleaned_session_.create_object(schm_obj1);
            OGSessionObject o2 = cleaned_session_.create_object(schm_obj2);

            // check connectable rel type
            List<string> con_type1 = new List<string>();
            o1.get_connectable_relation_type_to(con_type1);
            Assert.IsTrue(con_type1.Count == 1);
            Assert.IsTrue(RELTYPE1.Equals(con_type1[0]));

            List<string> con_type2 = new List<string>();
            o1.get_connectable_relation_type_from(con_type2);
            Assert.IsTrue(con_type2.Count == 0);

            List<string> con_type3 = new List<string>();
            o2.get_connectable_relation_type_to(con_type3);
            Assert.IsTrue(con_type3.Count == 0);

            List<string> con_type4 = new List<string>();
            o2.get_connectable_relation_type_from(con_type4);
            Assert.IsTrue(con_type4.Count == 1);
            Assert.IsTrue(RELTYPE1.Equals(con_type4[0]));

            // check connectable rel type and schema obj
            Dictionary<string, List<OGSchemaObject>> con_pair1 = new Dictionary<string, List<OGSchemaObject>>();
            o1.get_connectable_to(con_pair1);
            Assert.IsTrue(con_pair1.Count == 1);
            Assert.IsTrue(con_pair1.ContainsKey(RELTYPE1));

            Assert.IsTrue(con_pair1[RELTYPE1].Count == 2);
            foreach (OGSchemaObject it in con_pair1[RELTYPE1])
            {
                Assert.IsTrue(it.get_id() == schm_obj2.get_id()
                               || it.get_id() == schm_obj4.get_id());
            }

            Dictionary<string, List<OGSchemaObject>> con_pair2
          = new Dictionary<string, List<OGSchemaObject>>();
            o1.get_connectable_from(con_pair2);
            Assert.IsTrue(con_pair2.Count == 0);

            Dictionary<string, List<OGSchemaObject>> con_pair3
                = new Dictionary<string, List<OGSchemaObject>>();
            o2.get_connectable_to(con_pair3);
            Assert.IsTrue(con_pair3.Count == 0);

            Dictionary<string, List<OGSchemaObject>> con_pair4
                = new Dictionary<string, List<OGSchemaObject>>();
            o2.get_connectable_from(con_pair4);
            Assert.IsTrue(con_pair4.Count == 1);
            Assert.IsTrue(con_pair4.ContainsKey(RELTYPE1));
            Assert.IsTrue(con_pair4[RELTYPE1].Count == 1);
            foreach (OGSchemaObject it in con_pair4[RELTYPE1])
            {
                //std::cout << (*it).get_id() << std::endl;
                //std::cout << schm_obj1.get_id() << std::endl;

                Assert.IsTrue(it.get_id() == schm_obj1.get_id());
            }
        }




        // 2 schema relation
        [TestMethod]
        public void session_1005()
        {
            string OTYPE1 = "type_session_1005_1";
            string ONAME1 = "name_session_1005_1";
            string OTYPE2 = "type_session_1005_2";
            string ONAME2 = "name_session_1005_2";
            string OTYPE3 = "type_session_1005_3";
            string ONAME3 = "name_session_1005_3";
            string RELTYPE1 = "reltype_session_1004_1";

            // create schema obj
            OGSchemaObject schm_obj1 = cleaned_session_.schema().create_object(
                                                   OTYPE1,
                                                   ONAME1);
            OGSchemaObject schm_obj2 = cleaned_session_.schema().create_object(
                                                   OTYPE2,
                                                   ONAME2);
            OGSchemaObject schm_obj3 = cleaned_session_.schema().create_object(
                                                   OTYPE3,
                                                   ONAME3);

            schm_obj1.connect_to(schm_obj2, RELTYPE1);
            schm_obj1.connect_to(schm_obj3, RELTYPE1);

            OGSessionObject o1 = cleaned_session_.create_object(schm_obj1);
            OGSessionObject o2 = cleaned_session_.create_object(schm_obj2);
            OGSessionObject o3 = cleaned_session_.create_object(schm_obj2);

            OGSessionRelation r1 = o1.connect_to(o2, RELTYPE1);
            OGSessionRelation r2 = o1.connect_to(o3, RELTYPE1);

            Assert.AreEqual(RELTYPE1, r1.get_schema_relation_type());
            Assert.AreEqual(RELTYPE1, r2.get_schema_relation_type());
        }

        // parameter test (integer)
        [TestMethod]
        public void session_1006()
        {
            string OTYPE1 = "Document2002from";
            string OTYPE2 = "Document2002to";
            string RELTYPE = "Document2002rel";
            string ONAME = "Dcument-Name2002";
            List<string> otype_list = new List<string>();
            otype_list.Add(OTYPE1);
            otype_list.Add(OTYPE2);

            List<string> rel_type_list = new List<string>();
            rel_type_list.Add(RELTYPE);

            OGSchemaObject p1 = cleaned_session_.schema().create_object(OTYPE1,
                                          ONAME);
            OGSchemaObject p2 = cleaned_session_.schema().create_object(OTYPE2,
                                          ONAME);

            OGSchemaRelation rel_ptr = p1.connect_to(p2, RELTYPE);

            OGInteger type1 = new OGInteger(1, 1, 1, 1, 1);
            OGInteger type2 = new OGInteger(2, 1, 1, 1, 1);

            OGSchemaParameter test1 =
              cleaned_session_.schema().create_parameter_interger("hoge_type1", "comment1",
                  type1);
            OGSchemaParameter test2 =
              cleaned_session_.schema().create_parameter_interger("hoge_type2", "comment2",
                  type2, 3, 2, 4);

            Assert.AreEqual(test1.get_comment(), "comment1");
            Assert.AreEqual(test1.get_type(), "hoge_type1");

            Assert.AreEqual(test2.get_comment(), "comment2");
            Assert.AreEqual(test2.get_type(), "hoge_type2");

            List<Tuple<string, OGSchemaParameter>> param_name_types0 = new List<Tuple<string, OGSchemaParameter>>();
            p1.get_parameters(param_name_types0);

            Assert.AreEqual(param_name_types0.Count, 0);

            // apply parameter to schema_object
            p1.add_parameter_definition("H1", test1);
            p1.add_parameter_definition("H2", test2);

            // apply parameter to schema_relation
            rel_ptr.add_parameter_definition("H3", test1);
            rel_ptr.add_parameter_definition("H4", test2);

            {
                List<Tuple<string, OGSchemaParameter>> param_name_types1
                    = new List<Tuple<string, OGSchemaParameter>>();
                p1.get_parameters(param_name_types1);

                Assert.AreEqual(param_name_types1.Count, 2);

                bool h1_hit = false;
                bool h2_hit = false;
                foreach (Tuple<string, OGSchemaParameter> it in param_name_types1)
                {
                    if (it.Item1.Equals("H1"))
                    {
                        h1_hit = true;
                    }
                    else if (it.Item1.Equals("H2"))
                    {
                        h2_hit = true;
                    }
                    else
                    {
                        Assert.Fail("test failed.");
                    }
                }
                Assert.AreEqual(h1_hit, true);
                Assert.AreEqual(h2_hit, true);
            }

            // session object test
            {
                OGSessionObject o1 = cleaned_session_.create_object(p1);
                o1.set_parameter_value_integer("H1", 3);
                //    o1.sync();

                OGSessionObject o1_2 = cleaned_session_.get_object(
                      o1.get_id());

                if (o1_2 == null) { Assert.Fail("null"); }

                int h1 =
                o1_2.get_parameter_value_integer("H2");
                Assert.AreEqual(h1, 2);

                List<int> h2 =
                o1_2.get_parameter_values_integer("H2");
                Assert.AreEqual(h2.Count, 3);

                foreach (int i in h2)
                {
                    Assert.AreEqual(i, 2);
                }

                h2.Add(1);

                o1_2.set_parameter_values_integer("H2", h2);
                Assert.AreEqual(h2.Count, 4);
                h2.Clear();

                h2 =
              o1_2.get_parameter_values_integer("H2");
                Assert.AreEqual(h2.Count, 4);
                int row = 0;
                foreach (int i in h2)
                {
                    if (row == 0) { Assert.AreEqual(i, 2); }
                    if (row == 1) { Assert.AreEqual(i, 2); }
                    if (row == 2) { Assert.AreEqual(i, 2); }
                    if (row == 3) { Assert.AreEqual(i, 1); }
                    row++;
                }
            }

            // session releation
            {
                OGSessionObject o1 = cleaned_session_.create_object(p1);
                OGSessionObject o2 = cleaned_session_.create_object(p2);

                OGSessionRelation o1o2 = o1.connect_to(o2, RELTYPE);
                //    o1o2.sync();

                OGSessionRelation o1_2 = cleaned_session_.get_relation(
                      o1o2.get_id());

                if (o1_2 == null) { Assert.Fail("null"); }

                int h1 =
                o1_2.get_parameter_value_integer("H3");
                Assert.AreEqual(h1, 1);

                List<int> h2 =
                o1_2.get_parameter_values_integer("H4");
                Assert.AreEqual(h2.Count, 3);

                foreach (int i in h2)
                {
                    Assert.AreEqual(i, 2);
                }

                h2.Add(3);

                o1_2.set_parameter_values_integer("H4", h2);
                Assert.AreEqual(h2.Count, 4);

                h2 =
                    o1_2.get_parameter_values_integer("H4");
                Assert.AreEqual(h2.Count, 4);
                int row = 0;
                foreach (int i in h2)
                {
                    if (row == 0) { Assert.AreEqual(i, 2); }
                    if (row == 1) { Assert.AreEqual(i, 2); }
                    if (row == 2) { Assert.AreEqual(i, 2); }
                    if (row == 3) { Assert.AreEqual(i, 3); }
                    row++;
                }
            }
        }



        // parameter test (real)
        [TestMethod]
        public void session_1007()
        {
            string OTYPE1 = "Document2002from";
            string OTYPE2 = "Document2002to";
            string RELTYPE = "Document2002rel";
            string ONAME = "Dcument-Name2002";
            List<string> otype_list = new List<string>();
            otype_list.Add(OTYPE1);
            otype_list.Add(OTYPE2);

            List<string> rel_type_list = new List<string>();
            rel_type_list.Add(RELTYPE);

            OGSchemaObject p1 = cleaned_session_.schema().create_object(OTYPE1,
                                          ONAME);
            OGSchemaObject p2 = cleaned_session_.schema().create_object(OTYPE2,
                                          ONAME);

            OGSchemaRelation rel_ptr = p1.connect_to(p2, RELTYPE);

            OGReal type1 = new OGReal(1.141, 1, 1, 1, 1);
            OGReal type2 = new OGReal(2.236, 1, 1, 1, 1);

            OGSchemaParameter ptest1 =
              cleaned_session_.schema().create_parameter_real("hoge_type1", "comment1",
                  type1);
            OGSchemaParameter ptest2 =
              cleaned_session_.schema().create_parameter_real("hoge_type2", "comment2",
                  type2, 3, 2, 4);

            Assert.AreEqual(ptest1.get_comment(), "comment1");
            Assert.AreEqual(ptest1.get_type(), "hoge_type1");

            Assert.AreEqual(ptest2.get_comment(), "comment2");
            Assert.AreEqual(ptest2.get_type(), "hoge_type2");

            List<Tuple<string, OGSchemaParameter>> param_name_types0 = new List<Tuple<string, OGSchemaParameter>>();
            p1.get_parameters(param_name_types0);

            Assert.AreEqual(param_name_types0.Count, 0);

            // apply parameter to schema_object
            p1.add_parameter_definition("H1", ptest1);
            p1.add_parameter_definition("H2", ptest2);

            // apply parameter to schema_relation
            rel_ptr.add_parameter_definition("H3", ptest1);
            rel_ptr.add_parameter_definition("H4", ptest2);

            {
                List<Tuple<string, OGSchemaParameter>> param_name_types1 = new List<Tuple<string, OGSchemaParameter>>();
                p1.get_parameters(param_name_types1);

                Assert.AreEqual(param_name_types1.Count, 2);

                bool h1_hit = false;
                bool h2_hit = false;
                foreach (Tuple<string, OGSchemaParameter> it in
                    param_name_types1)
                {
                    if (it.Item1.Equals("H1"))
                    {
                        h1_hit = true;
                    }
                    else if (it.Item1.Equals("H2"))
                    {
                        h2_hit = true;
                    }
                    else
                    {
                        Assert.Fail("test failed.");
                    }
                }
                Assert.AreEqual(h1_hit, true);
                Assert.AreEqual(h2_hit, true);
            }

            // session object test
            {
                OGSessionObject o1 = cleaned_session_.create_object(p1);
                o1.set_parameter_value_real("H1", 1.1234);
                //o1.sync();

                OGSessionObject o1_2 = cleaned_session_.get_object(
                      o1.get_id());

                if (o1_2 == null) { Assert.Fail("null"); }

                double h1 =
                o1_2.get_parameter_value_real("H1");
                Assert.AreEqual(h1, 1.1234);

                List<double> h2 =
                o1_2.get_parameter_values_real("H2");

                Assert.AreEqual(h2.Count, 3);

                foreach (double i in h2)
                {
                    Assert.AreEqual(i, 2.236);
                }

                h2.Add(1.1234);

                o1_2.set_parameter_values_real("H2", h2);

                h2 =
                o1_2.get_parameter_values_real("H2");
                Assert.AreEqual(h2.Count, 4);
                int row = 0;
                foreach (double i in h2)
                {
                    if (row == 0) { Assert.AreEqual(i, 2.236); }
                    if (row == 1) { Assert.AreEqual(i, 2.236); }
                    if (row == 2) { Assert.AreEqual(i, 2.236); }
                    if (row == 3) { Assert.AreEqual(i, 1.1234); }
                    row++;
                }

            }

            // session releation
            {
                OGSessionObject o1 = cleaned_session_.create_object(p1);
                OGSessionObject o2 = cleaned_session_.create_object(p2);

                OGSessionRelation o1o2 = o1.connect_to(o2, RELTYPE);
                //o1o2.sync();

                OGSessionRelation o1_2 = cleaned_session_.get_relation(
                      o1o2.get_id());

                if (o1_2 == null) { Assert.Fail("null"); }

                double h1 =
                o1_2.get_parameter_value_real("H3");
                Assert.AreEqual(h1, 1.141);

                List<double> h2 =
                o1_2.get_parameter_values_real("H4");

                Assert.AreEqual(h2.Count, 3);

                foreach (double i in h2)
                {
                    Assert.AreEqual(i, 2.236);
                }

                h2.Add(1.1234);

                o1_2.set_parameter_values_real("H4", h2);

                h2 =
                o1_2.get_parameter_values_real("H4");
                Assert.AreEqual(h2.Count, 4);
                int row = 0;
                foreach (double i in h2)
                {
                    if (row == 0) { Assert.AreEqual(i, 2.236); }
                    if (row == 1) { Assert.AreEqual(i, 2.236); }
                    if (row == 2) { Assert.AreEqual(i, 2.236); }
                    if (row == 3) { Assert.AreEqual(i, 1.1234); }
                    row++;
                }
            }
        }



        // parameter test text
        [TestMethod]
        public void session_1008()
        {
            string OTYPE1 = "Document2002from";
            string OTYPE2 = "Document2002to";
            string RELTYPE = "Document2002rel";
            string ONAME = "Dcument-Name2002";
            List<string> otype_list = new List<string>();
            otype_list.Add(OTYPE1);
            otype_list.Add(OTYPE2);

            List<string> rel_type_list = new List<string>();
            rel_type_list.Add(RELTYPE);

            OGSchemaObject p1 = cleaned_session_.schema().create_object(OTYPE1,
                                          ONAME);
            OGSchemaObject p2 = cleaned_session_.schema().create_object(OTYPE2,
                                          ONAME);

            OGSchemaRelation rel_ptr = p1.connect_to(p2, RELTYPE);

            OGText ptype1 = new OGText("foo", 1, 1, 1, 1);

            OGText ptype2 = new OGText("bar", 1, 1, 1, 1);

            // array size is set to 1 implicitly
            OGSchemaParameter ptest1 =
              cleaned_session_.schema().create_parameter_text("hoge_type1", "comment1",
                  ptype1);
            // array size is set to 3 explicitly
            OGSchemaParameter ptest2 =
              cleaned_session_.schema().create_parameter_text("hoge_type2", "comment2",
                  ptype2, 3, 2, 4);

            Assert.AreEqual(ptest1.get_comment(), "comment1");
            Assert.AreEqual(ptest1.get_type(), "hoge_type1");

            Assert.AreEqual(ptest2.get_comment(), "comment2");
            Assert.AreEqual(ptest2.get_type(), "hoge_type2");

            List<Tuple<string, OGSchemaParameter>> param_name_types0 = new List<Tuple<string, OGSchemaParameter>>();
            p1.get_parameters(param_name_types0);

            Assert.AreEqual(param_name_types0.Count, 0);

            // apply parameter to schema_object
            p1.add_parameter_definition("H1", ptest1);
            p1.add_parameter_definition("H2", ptest2);

            // apply parameter to schema_relation
            rel_ptr.add_parameter_definition("H3", ptest1);
            rel_ptr.add_parameter_definition("H4", ptest2);

            // test definition
            {
                List<Tuple<string, OGSchemaParameter>> param_name_types1
                    = new List<Tuple<string, OGSchemaParameter>>();
                p1.get_parameters(param_name_types1);

                Assert.AreEqual(param_name_types1.Count, 2);

                bool h1_hit = false;
                bool h2_hit = false;
                foreach (Tuple<string, OGSchemaParameter> it in param_name_types1)
                {
                    if (it.Item1.Equals("H1"))
                    {
                        h1_hit = true;
                    }
                    else if (it.Item1.Equals("H2"))
                    {
                        h2_hit = true;
                    }
                    else
                    {
                        Assert.Fail("test failed.");
                    }
                }
                Assert.AreEqual(h1_hit, true);
                Assert.AreEqual(h2_hit, true);
            }

            // session object test
            {
                OGSessionObject o1 = cleaned_session_.create_object(p1);
                o1.set_parameter_value_text("H1", "aaa");
                //    o1.sync();

                OGSessionObject o1_2 = cleaned_session_.get_object(
                      o1.get_id());

                if (o1_2 == null) { Assert.Fail("null"); }

                string h1 =
                o1_2.get_parameter_value_text("H1");
                Assert.AreEqual(h1, "aaa");

                List<string> h2 =
                o1_2.get_parameter_values_text("H2");

                foreach (string i in h2)
                {
                    Assert.AreEqual(i, "bar");
                }

                h2.Add("foo");

                o1_2.set_parameter_values_text("H2", h2);

                h2.Clear();
                h2 = o1_2.get_parameter_values_text("H2");
                Assert.AreEqual(h2.Count, 4);
                int row = 0;
                foreach (string i in h2)
                {
                    if (row == 0) { Assert.AreEqual(i, "bar"); }
                    if (row == 1) { Assert.AreEqual(i, "bar"); }
                    if (row == 2) { Assert.AreEqual(i, "bar"); }
                    if (row == 3) { Assert.AreEqual(i, "foo"); }
                    row++;
                }
            }

            // session releation
            {
                OGSessionObject o1 = cleaned_session_.create_object(p1);
                OGSessionObject o2 = cleaned_session_.create_object(p2);

                OGSessionRelation o1o2 = o1.connect_to(o2, RELTYPE);
                //    o1o2.sync();

                OGSessionRelation o1_2 = cleaned_session_.get_relation(
                      o1o2.get_id());

                if (o1_2 == null) { Assert.Fail("null"); }

                string h1 =
                o1_2.get_parameter_value_text("H3");
                Assert.AreEqual(h1, "foo");

                List<string> h2 =
                o1_2.get_parameter_values_text("H4");

                Assert.AreEqual(h2.Count, 3);

                foreach (string i in h2)
                {
                    Assert.AreEqual(i, "bar");
                }

                h2.Add("foo");

                o1_2.set_parameter_values_text("H4", h2);

                h2 =
                o1_2.get_parameter_values_text("H4");
                Assert.AreEqual(h2.Count, 4);
                int row = 0;
                foreach (string i in h2)
                {
                    if (row == 0) { Assert.AreEqual(i, "bar"); }
                    if (row == 1) { Assert.AreEqual(i, "bar"); }
                    if (row == 2) { Assert.AreEqual(i, "bar"); }
                    if (row == 3) { Assert.AreEqual(i, "foo"); }
                    row++;
                }
            }
        }

        // parameter test (bool)
        [TestMethod]
        public void session_1009()
        {
            string OTYPE1 = "Document2002from";
            string OTYPE2 = "Document2002to";
            string RELTYPE = "Document2002rel";
            string ONAME = "Dcument-Name2002";
            List<string> otype_list = new List<string>();
            otype_list.Add(OTYPE1);
            otype_list.Add(OTYPE2);

            List<string> rel_type_list = new List<string>();
            rel_type_list.Add(RELTYPE);

            OGSchemaObject p1 = cleaned_session_.schema().create_object(OTYPE1,
                                          ONAME);
            OGSchemaObject p2 = cleaned_session_.schema().create_object(OTYPE2,
                                          ONAME);

            OGSchemaRelation rel_ptr = p1.connect_to(p2, RELTYPE);

            OGBool type1 = new OGBool(true);
            OGBool type2 = new OGBool(false);

            OGSchemaParameter test1 =
              cleaned_session_.schema().create_parameter_bool("hoge_type1", "comment1",
                  type1);
            OGSchemaParameter test2 =
              cleaned_session_.schema().create_parameter_bool("hoge_type2", "comment2",
                  type2, 3, 2, 4);

            Assert.AreEqual(test1.get_comment(), "comment1");
            Assert.AreEqual(test1.get_type(), "hoge_type1");

            Assert.AreEqual(test2.get_comment(), "comment2");
            Assert.AreEqual(test2.get_type(), "hoge_type2");

            List<Tuple<string, OGSchemaParameter>> param_name_types0 = new List<Tuple<string, OGSchemaParameter>>();
            p1.get_parameters(param_name_types0);

            Assert.AreEqual(param_name_types0.Count, 0);

            // apply parameter to schema_object
            p1.add_parameter_definition("H1", test1);
            p1.add_parameter_definition("H2", test2);

            // apply parameter to schema_relation
            rel_ptr.add_parameter_definition("H3", test1);
            rel_ptr.add_parameter_definition("H4", test2);

            {
                List<Tuple<string, OGSchemaParameter>> param_name_types1
                    = new List<Tuple<string, OGSchemaParameter>>();
                p1.get_parameters(param_name_types1);

                Assert.AreEqual(param_name_types1.Count, 2);

                bool h1_hit = false;
                bool h2_hit = false;
                foreach (Tuple<string, OGSchemaParameter> it in param_name_types1)
                {
                    if (it.Item1.Equals("H1"))
                    {
                        h1_hit = true;
                    }
                    else if (it.Item1.Equals("H2"))
                    {
                        h2_hit = true;
                    }
                    else
                    {
                        Assert.Fail("test failed.");
                    }
                }
                Assert.AreEqual(h1_hit, true);
                Assert.AreEqual(h2_hit, true);
            }

            // session object test
            {
                OGSessionObject o1 = cleaned_session_.create_object(p1);
                o1.set_parameter_value_integer("H1", 3);
                //    o1.sync();

                OGSessionObject o1_2 = cleaned_session_.get_object(
                      o1.get_id());

                if (o1_2 == null) { Assert.Fail("null"); }

                bool h1 =
                o1_2.get_parameter_value_bool("H2");
                Assert.AreEqual(h1, false);

                List<bool> h2 =
                o1_2.get_parameter_values_bool("H2");
                Assert.AreEqual(h2.Count, 3);

                foreach (bool i in h2)
                {
                    Assert.AreEqual(i, false);
                }

                h2.Add(true);

                o1_2.set_parameter_values_bool("H2", h2);
                Assert.AreEqual(h2.Count, 4);
                h2.Clear();

                h2 =
              o1_2.get_parameter_values_bool("H2");
                Assert.AreEqual(h2.Count, 4);
                int row = 0;
                foreach (bool i in h2)
                {
                    if (row == 0) { Assert.AreEqual(i, false); }
                    if (row == 1) { Assert.AreEqual(i, false); }
                    if (row == 2) { Assert.AreEqual(i, false); }
                    if (row == 3) { Assert.AreEqual(i, true); }
                    row++;
                }
            }

            // session releation
            {
                OGSessionObject o1 = cleaned_session_.create_object(p1);
                OGSessionObject o2 = cleaned_session_.create_object(p2);

                OGSessionRelation o1o2 = o1.connect_to(o2, RELTYPE);
                //    o1o2.sync();

                OGSessionRelation o1_2 = cleaned_session_.get_relation(
                      o1o2.get_id());

                if (o1_2 == null) { Assert.Fail("null"); }

                bool h1 =
                o1_2.get_parameter_value_bool("H3");
                Assert.AreEqual(h1, true);

                List<bool> h2 =
                o1_2.get_parameter_values_bool("H4");
                Assert.AreEqual(h2.Count, 3);

                foreach (bool i in h2)
                {
                    Assert.AreEqual(i, false);
                }

                h2.Add(true);

                o1_2.set_parameter_values_bool("H4", h2);
                Assert.AreEqual(h2.Count, 4);

                h2 =
                    o1_2.get_parameter_values_bool("H4");
                Assert.AreEqual(h2.Count, 4);
                int row = 0;
                foreach (bool i in h2)
                {
                    if (row == 0) { Assert.AreEqual(i, false); }
                    if (row == 1) { Assert.AreEqual(i, false); }
                    if (row == 2) { Assert.AreEqual(i, false); }
                    if (row == 3) { Assert.AreEqual(i, true); }
                    row++;
                }
            }
        }

        // parameter test (bool)
        [TestMethod]
        public void session_1100()
        {
            string OTYPE1 = "Documentt1";
            string ONAME1 = "Documentn1";
            string OTYPE2 = "Documentt2";
            string ONAME2 = "Documentn2";
            string RELTYPE = "Documentrel";
            List<string> otype_list = new List<string>();
            otype_list.Add(OTYPE1);
            otype_list.Add(OTYPE2);

            List<string> rel_type_list = new List<string>();
            rel_type_list.Add(RELTYPE);

            OGSchemaObject p1 = cleaned_session_.schema().create_object(
                OTYPE1,
                ONAME1);
            OGSchemaObject p2 = cleaned_session_.schema().create_object(
                OTYPE1,
                ONAME2);

            OGSchemaRelation rel_ptr = p1.connect_to(p2, RELTYPE);

            OGSessionObject o1 = cleaned_session_.create_object(p1);
            OGSessionObject o2 = cleaned_session_.create_object(p2);

            OGSessionRelation r1 = o1.connect_to(o2, RELTYPE);
            String did = r1.get_id();

            o1.disconnect(o2);
            OGSessionRelation rr1 = cleaned_session_.get_relation(did);

            Assert.AreEqual(rr1, null);
        }

        // copy test
        [TestMethod]
        public void session_1112()
        {
            string OTYPE1 = "type_session_1002_1";
            string ONAME1 = "name_session_1002_1";
            string OTYPE2 = "type_session_1002_2";
            string ONAME2 = "name_session_1002_2";
            string RELTYPE = "reltype_session_1002";
            string RELNAME = "relnamee_session_1002";
            List<string> otype_list1 = new List<string>(); otype_list1.Add(OTYPE1);
            List<string> oname_list1 = new List<string>(); oname_list1.Add(ONAME1);
            List<string> otype_list2 = new List<string>(); otype_list2.Add(OTYPE2);
            List<string> oname_list2 = new List<string>(); oname_list2.Add(ONAME2);
            List<string> reltype_list = new List<string>(); reltype_list.Add(RELTYPE);
            List<string> relname_list = new List<string>(); relname_list.Add(RELNAME);
            {
                // create schema obj
                OGSchemaObject schm_obj1 = cleaned_session_.schema().create_object(
                                                       OTYPE1,
                                                       ONAME1);
                OGSchemaObject schm_obj2 = cleaned_session_.schema().create_object(
                                                       OTYPE2,
                                                       ONAME2);
                schm_obj1.connect_to(schm_obj2, RELTYPE);

                OGSessionObject o1 = cleaned_session_.create_object(schm_obj1);
                OGSessionObject o2 = cleaned_session_.create_object(schm_obj2);

                o1.connect_to(o2, RELTYPE);
                o1.connect_to(o2, RELTYPE);

                OGSessionObject o1copy = o1.copy_object();
                Assert.AreNotEqual(o1copy.get_id(), o1.get_id());
                Assert.AreEqual(o1copy.get_schema_object_id(),
                    o1.get_schema_object_id());

                // copy recursively
                {
                    OGSessionObject o1copy2 = o1.copy_object(OGConnectionDirection.Direction_To);
                    List<OGSessionObject> child = new List<OGSessionObject>();
                    o1copy2.get_connected_object_to(child);

                    Assert.AreEqual(2, child.Count);
                }
                // copy recursively
                {
                    OGSessionObject o1copy2 = o1.copy_object(OGConnectionDirection.Direction_From);
                    List<OGSessionObject> child = new List<OGSessionObject>();
                    o1copy2.get_connected_object_to(child);

                    Assert.AreEqual(0, child.Count);
                }
                // copy recursively
                {
                    OGSessionObject o1copy2 = o2.copy_object(OGConnectionDirection.Direction_From);
                    List<OGSessionObject> child = new List<OGSessionObject>();
                    o1copy2.get_connected_object_from(child);

                    Assert.AreEqual(2, child.Count);
                }
            } 
        }
    }
}

