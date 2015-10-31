using System;
using System.Collections.Generic;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using og.net;

namespace test_og_net
{
    [TestClass]
    public class UnitTest_Transaction
    {
        [TestMethod]
        public void transaction_1000()
        {
            OGSession cleaned_session_ = new TestInitializer().initialize();

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
            string id;
            {
                // create transaction
                OGTransaction tran = new OGTransaction(cleaned_session_);

                // create schema obj
                OGSchemaObject schm_obj1 = cleaned_session_.schema().create_object(OTYPE1,
                                              ONAME1);
                id = schm_obj1.get_id();

                OGSchemaObject schm_obj2 = cleaned_session_.schema().create_object(OTYPE2,
                                              ONAME2);
                // connect
                schm_obj1.connect_to(schm_obj2, RELTYPE);
                tran.commit();
            }

            {
                // check
                OGSchemaObject schm_obj1 = cleaned_session_.schema().get_object(
                    id);
                Assert.IsTrue(schm_obj1 != null);

                List<OGSchemaObject> schm_children =
                schm_obj1.get_connected_object_to(reltype_list);
                Assert.IsTrue(schm_children.Count == 1);
            }
        }

        [TestMethod]
        public void transaction_1001()
        {
            OGSession cleaned_session_ = new TestInitializer().initialize();

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
            string id;
            string child_id;

            // create transaction. in .Net, implicit transaction doesn't work
            using (OGTransaction tran = new OGTransaction(cleaned_session_))
            {

                // create schema obj
                OGSchemaObject schm_obj1 = cleaned_session_.schema().create_object(OTYPE1,
                                              ONAME1);
                id = schm_obj1.get_id();

                OGSchemaObject schm_obj2 = cleaned_session_.schema().create_object(OTYPE2,
                                              ONAME2);
                // connect
                schm_obj1.connect_to(schm_obj2, RELTYPE);
                child_id = schm_obj2.get_id();
            }

            {
                // check
                OGSchemaObject schm_obj1 = cleaned_session_.schema().get_object(
                    id);
                Assert.IsTrue(schm_obj1 == null);

                OGSchemaObject schm_obj2 = cleaned_session_.schema().get_object(
                    child_id);
                Assert.IsTrue(schm_obj2 == null);
            }
        }

        // parameter and transaction
        [TestMethod]
        public void transaction_1002()
        {
            OGSession cleaned_session_ = new TestInitializer().initialize();

            string OTYPE1 = "Document2002from";
            string OTYPE2 = "Document2002to";
            string RELTYPE = "Document2002rel";
            string ONAME = "Dcument-Name2002";
            List<string> otype_list = new List<string>();
            otype_list.Add(OTYPE1);
            otype_list.Add(OTYPE2);

            List<string> rel_type_list = new List<string>();
            rel_type_list.Add(RELTYPE);

            OGSchemaObject p1 = cleaned_session_.schema().create_object(OTYPE1, ONAME);
            OGSchemaObject p2 = cleaned_session_.schema().create_object(OTYPE2, ONAME);

            OGSchemaRelation rel_ptr = p1.connect_to(p2, RELTYPE);

            OGReal type1 = new OGReal(1.414, 1, 3, 1, 3);
            OGReal type2 = new OGReal(2.236, 1, 3, 1, 3);

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

            List<Tuple<string, OGSchemaParameter>> param_name_types0
                = new List<Tuple<string, OGSchemaParameter>>();
            p1.get_parameters(param_name_types0);

            Assert.AreEqual(param_name_types0.Count, 0);

            // apply parameter to schema_object
            p1.add_parameter_definition("H1", ptest1);
            p1.add_parameter_definition("H2", ptest2);

            // apply parameter to schema_relation
            rel_ptr.add_parameter_definition("H3", ptest1);
            rel_ptr.add_parameter_definition("H4", ptest2);

            // session object test (explicit)
            using(OGTransaction tran = new OGTransaction(cleaned_session_))
            {
                OGSessionObject o1 = cleaned_session_.create_object(p1);
                //o1.sync();
                string oid = o1.get_id();

                tran.rollback();

                OGSessionObject o1_2 = cleaned_session_.get_object(oid);

                // confirm rollback(discard object creation)
                Assert.AreEqual(o1_2 != null, false);
            }


            // session object test (implicit doesn't work)
            {
                OGSessionObject o1 = cleaned_session_.create_object(p1);
                string oid = o1.get_id();
                using(    OGTransaction tran = new OGTransaction(cleaned_session_))
                {


                    o1.set_parameter_value_real("H1", 1.1234);

                } // rollback

                {
                    OGSessionObject o1_2 = cleaned_session_.get_object(oid);

                    Assert.AreEqual(o1_2 != null, true);

                    double h1 =
                    o1_2.get_parameter_value_real("H2");
                    // confirm rollback
                    Assert.AreEqual(h1, 2.236);
                }
            }

            // session object test (commit)
            {
                OGSessionObject o1 = cleaned_session_.create_object(p1);
                string oid = o1.get_id();
                {
                    OGTransaction tran = new OGTransaction(cleaned_session_);

                    o1.set_parameter_value_real("H1", 1.1234);

                    tran.commit();
                } // commit;

                {
                    OGSessionObject o1_2 = cleaned_session_.get_object(oid);

                    Assert.AreEqual(o1_2 != null, true);

                    double h1 = o1_2.get_parameter_value_real("H2");
                    // confirm commit
                    Assert.AreEqual(h1, 2.236);
                }
            }

            // session relation test (explicit)
            {
                OGSessionObject o1 = cleaned_session_.create_object(p1);
                OGSessionObject o2 = cleaned_session_.create_object(p2);

                OGTransaction tran = new OGTransaction(cleaned_session_);
                OGSessionRelation o1o2 = o1.connect_to(o2, RELTYPE);
                string relid = o1o2.get_id();


                tran.rollback();

                List<OGSessionObject> connected =                o1.get_connected_object();
                Assert.AreEqual(connected.Count, 0);
            }

            // session relation test (implicit doesn't work)
            {
                OGSessionObject o1 = cleaned_session_.create_object(p1);
                OGSessionObject o2 = cleaned_session_.create_object(p2);

                using(    OGTransaction tran = new OGTransaction(cleaned_session_))
                {

                    OGSessionRelation o1o2 = o1.connect_to(o2, RELTYPE);
                    //relid = o1o2.get_id();

                } //explicit rollback;

                List<OGSessionObject> connected = 
                o1.get_connected_object();
                Assert.AreEqual(connected.Count, 0);
            }

            // session relation test (explicit)
            {
                OGSessionObject o1 = cleaned_session_.create_object(p1);
                OGSessionObject o2 = cleaned_session_.create_object(p2);

                OGTransaction tran = new OGTransaction(cleaned_session_);
                OGSessionRelation o1o2 = o1.connect_to(o2, RELTYPE);
                //string relid = o1o2.get_id();
                //o1o2.sync();

                tran.commit();

                List<OGSessionObject> connected = 
                o1.get_connected_object();
                Assert.AreEqual(connected.Count, 1);
            }

            // session object test (implicit doesn't work)
            {
                OGSessionObject o1 = cleaned_session_.create_object(p1);
                OGSessionObject o2 = cleaned_session_.create_object(p2);
                OGSessionRelation o1o2 = o1.connect_to(o2, RELTYPE);

                string relid = o1o2.get_id();
                {
                    OGTransaction tran = new OGTransaction(cleaned_session_);

                    o1o2.set_parameter_value_real("H3", 2.2345);
                    //o1o2.sync();
                    tran.rollback();
                } // explicit rollback

                {
                    OGSessionRelation o1o2_2 = cleaned_session_.get_relation(relid);

                    double h1 = o1o2_2.get_parameter_value_real("H3");
                    // confirm rollback
                    Assert.AreEqual(h1, 1.414);
                }

                using(                    OGTransaction tran = new OGTransaction(cleaned_session_))
                {
                    o1o2.set_parameter_value_real("H3", 2.2345);
                    //o1o2.sync();
                } // implicit rollback

                {
                    
                    OGSessionRelation o1o2_2 = cleaned_session_.get_relation(relid);

                    double h1 = o1o2_2.get_parameter_value_real("H3");
                    // confirm rollback
                    Assert.AreEqual(h1, 1.414);
                }

                {
                    OGTransaction tran = new OGTransaction(cleaned_session_);

                    o1o2.set_parameter_value_real("H3", 1.1234);
                    //o1o2.sync();
                    tran.commit();
                } // commit;

                {
                    double h1 = o1o2.get_parameter_value_real("H3");
                    // confirm commit
                    Assert.AreEqual(h1, 1.1234);
                }
            }
        }
    }
}