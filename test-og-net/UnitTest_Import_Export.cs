using System;
using System.Collections.Generic;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using og.net;
using System.IO;

namespace test_og_net
{
    [TestClass]
    public class UnitTest_ImportExport
    {
        [TestInitialize]
        public void setpath()
        {
            InitilizeOnce.GetInstance().SetPath();
        }

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

        [TestMethod]
        public void export_1000()
        {
            og.net.OGSession cleaned_session_ = new OGSession();
            cleaned_session_.open(TestInitializer.DBPATH);
            cleaned_session_.purge();
            cleaned_session_.schema().purge();

            List<string> otype_list = new List<string>();
            otype_list.Add(OTYPE1);
            otype_list.Add(OTYPE2);

            List<string> rel_type_list = new List<string>();
            rel_type_list.Add(RELTYPE);

            OGSchemaObject p1 = cleaned_session_.schema().create_object(OTYPE1,
                                          ONAME);
            OGSchemaObject p2 = cleaned_session_.schema().create_object(OTYPE2,
                                          ONAME);
            OGSchemaObject p2a = cleaned_session_.schema().create_object(OTYPE2a,
                                          ONAMEa);
            OGSchemaObject p2b = cleaned_session_.schema().create_object(OTYPE2b,
                                          ONAMEb);

            OGSchemaRelation rel_ptr = p1.connect_to(p2, RELTYPE);
            OGSchemaRelation rel_ptr_a = p2.connect_to(p2a, RELTYPEa);
            OGSchemaRelation rel_ptr_b = p2.connect_to(p2b, RELTYPEb);

            OGInteger type1 = new OGInteger(1, 1, 3, 1, 3);
            OGInteger type2 = new OGInteger(2, 1, 3, 1, 3);
            OGReal type3 = new OGReal(1.141, 1.0, 3.0, 1.0, 3.0);
            OGReal type4 = new OGReal(2.236, 1.0, 3.0, 1.0, 3.0);
            OGText type5 = new OGText("foo", 1, 1, 1, 1);
            OGText type6 = new OGText("bar", 1, 1, 1, 1);

            // array size is set to 1 implicitly

            OGSchemaParameter ptest1 =
              cleaned_session_.schema().create_parameter_interger("hoge_type1", "comment1",
                  type1);
            OGSchemaParameter ptest3 =
              cleaned_session_.schema().create_parameter_real("hoge_type3", "comment1",
                  type3);
            OGSchemaParameter ptest5 =
              cleaned_session_.schema().create_parameter_text("hoge_type5", "comment1",
                  type5);

            // array size is set to 3 explicitly
            OGSchemaParameter ptest2 =
              cleaned_session_.schema().create_parameter_interger("hoge_type2", "comment2",
                  type2, 3, 2, 4);
            OGSchemaParameter ptest4 =
              cleaned_session_.schema().create_parameter_real("hoge_type4", "comment2",
                  type4, 3, 2, 4);
            OGSchemaParameter ptest6 =
              cleaned_session_.schema().create_parameter_text("hoge_type6", "comment2",
                  type6, 3, 2, 4);

            // apply parameter to schema_object
            p1.add_parameter_definition("H1", ptest1);
            p1.add_parameter_definition("H3", ptest3);
            p1.add_parameter_definition("H5", ptest5);

            p2.add_parameter_definition("H2", ptest2);
            p2a.add_parameter_definition("H4", ptest4);
            p2b.add_parameter_definition("H6", ptest6);

            // apply parameter to schema_relation
            rel_ptr.add_parameter_definition("I1", ptest1);
            rel_ptr.add_parameter_definition("I3", ptest3);
            rel_ptr.add_parameter_definition("I5", ptest5);

            rel_ptr_a.add_parameter_definition("I2", ptest2);
            rel_ptr_b.add_parameter_definition("I4", ptest4);

            OGSessionObject o1 = cleaned_session_.create_object(p1);

            OGSessionObject o2 = cleaned_session_.create_object(p2);
            OGSessionRelation o1o2 = o1.connect_to(o2, RELTYPE);
            //o1o2.sync();

            OGSessionObject o2a = cleaned_session_.create_object(p2a);
            OGSessionObject o2b = cleaned_session_.create_object(p2b);
            OGSessionRelation o2o2a = o2.connect_to(o2a, RELTYPEa);
            OGSessionRelation o2o2b = o2.connect_to(o2b, RELTYPEb);

            o1.set_parameter_value_integer("H1", 99);
            o1.set_parameter_value_real("H3", 3.14);
            o1.set_parameter_value_text("H5", "x");
            o2.set_parameter_value_integer("H2", 100);
            o2a.set_parameter_value_real("H4", 9.99);
            o2b.set_parameter_value_text("H6", "y");

            o1.export_to_recursively("exp1.aaa");
        }

        [TestMethod]
        public void import_1000()
        {
            og.net.OGSession cleaned_session_ = new OGSession();
            cleaned_session_.open(TestInitializer.DBPATH);
            cleaned_session_.purge();
            cleaned_session_.schema().purge();

            List<string> otype_list = new List<string>();
            otype_list.Add(OTYPE1);
            otype_list.Add(OTYPE2);

            List<string> rel_type_list = new List<string>();
            rel_type_list.Add(RELTYPE);

            OGSchemaObject p1 = cleaned_session_.schema().create_object(OTYPE1,
                                          ONAME);
            OGSchemaObject p2 = cleaned_session_.schema().create_object(OTYPE2,
                                          ONAME);
            OGSchemaObject p2a = cleaned_session_.schema().create_object(OTYPE2a,
                                          ONAMEa);
            OGSchemaObject p2b = cleaned_session_.schema().create_object(OTYPE2b,
                                          ONAMEb);

            OGSchemaRelation rel_ptr = p1.connect_to(p2, RELTYPE);
            OGSchemaRelation rel_ptr_a = p2.connect_to(p2a, RELTYPEa);
            OGSchemaRelation rel_ptr_b = p2.connect_to(p2b, RELTYPEb);

            OGInteger type1 = new OGInteger(1, 1, 3, 1, 3);
            OGInteger type2 = new OGInteger(2, 1, 3, 1, 3);
            OGReal type3 = new OGReal(1.141, 1.0, 3.0, 1.0, 3.0);
            OGReal type4 = new OGReal(2.236, 1.0, 3.0, 1.0, 3.0);
            OGText type5 = new OGText("foo", 1, 1, 1, 1);
            OGText type6 = new OGText("bar", 1, 1, 1, 1);

            // array size is set to 1 implicitly

            OGSchemaParameter ptest1 =
              cleaned_session_.schema().create_parameter_interger("hoge_type1", "comment1",
                  type1);
            OGSchemaParameter ptest3 =
              cleaned_session_.schema().create_parameter_real("hoge_type3", "comment1",
                  type3);
            OGSchemaParameter ptest5 =
              cleaned_session_.schema().create_parameter_text("hoge_type5", "comment1",
                  type5);

            // array size is set to 3 explicitly
            OGSchemaParameter ptest2 =
              cleaned_session_.schema().create_parameter_interger("hoge_type2", "comment2",
                  type2, 3, 2, 4);
            OGSchemaParameter ptest4 =
              cleaned_session_.schema().create_parameter_real("hoge_type4", "comment2",
                  type4, 3, 2, 4);
            OGSchemaParameter ptest6 =
              cleaned_session_.schema().create_parameter_text("hoge_type6", "comment2",
                  type6, 3, 2, 4);

            // apply parameter to schema_object
            p1.add_parameter_definition("H1", ptest1);
            p1.add_parameter_definition("H3", ptest3);
            p1.add_parameter_definition("H5", ptest5);

            p2.add_parameter_definition("H2", ptest2);
            p2a.add_parameter_definition("H4", ptest4);
            p2b.add_parameter_definition("H6", ptest6);

            // apply parameter to schema_relation
            rel_ptr.add_parameter_definition("I1", ptest1);
            rel_ptr.add_parameter_definition("I3", ptest3);
            rel_ptr.add_parameter_definition("I5", ptest5);

            rel_ptr_a.add_parameter_definition("I2", ptest2);
            rel_ptr_b.add_parameter_definition("I4", ptest4);

            OGSessionObject res =
                cleaned_session_.import_object_from_file("exp1.aaa");

            Assert.IsTrue(res != null);
            Assert.AreEqual(res.get_id(), "SESN_O_3");

            {
                List<string> types = new List<string>() { OTYPE1 };

                var objs1 = cleaned_session_.get_object_by_type(types);

                Assert.AreEqual(objs1.Count, 1);
                Assert.AreEqual(objs1[0].get_parameter_value_integer("H1"), 99);
                Assert.AreEqual(objs1[0].get_parameter_value_real("H3"), 3.14);
                Assert.AreEqual(objs1[0].get_parameter_value_text("H5"), "x");
            }

            {
                List<string> types = new List<string>() { OTYPE2 };

                var objs1 = cleaned_session_.get_object_by_type(types);
                Assert.AreEqual(objs1.Count, 1);
                Assert.AreEqual(objs1[0].get_parameter_value_integer("H2"), 100);
            }

            {
                List<string> types = new List<string>() { OTYPE2a };

                var objs1 = cleaned_session_.get_object_by_type(types);

                foreach(var o in objs1)
                {
                    System.Diagnostics.Debug.WriteLine(o.get_id());
                }

                Assert.AreEqual(objs1.Count, 1);
                Assert.AreEqual(objs1[0].get_parameter_value_real("H4"), 9.99);
            }


            {
                List<string> types = new List<string>() { OTYPE2b };

                var objs1 = cleaned_session_.get_object_by_type(types);
                Assert.AreEqual(objs1.Count, 1);
                Assert.AreEqual(objs1[0].get_parameter_value_text("H6"), "y");
            }

            // transaction
            OGTransaction tran1 = new OGTransaction(cleaned_session_);

            OGSessionObject res1 =
                cleaned_session_.import_object_from_file("exp1.aaa");

            Assert.IsTrue(res1 != null);

            tran1.commit();

            {
                List<string> types = new List<string>() { OTYPE2b };

                var objs1 = cleaned_session_.get_object_by_type(types);
                Assert.AreEqual(objs1.Count, 2);
                Assert.AreEqual(objs1[0].get_parameter_value_text("H6"), "y");
                Assert.AreEqual(objs1[1].get_parameter_value_text("H6"), "y");
            }

            // transaction
            OGTransaction tran2 = new OGTransaction(cleaned_session_);
            OGSessionObject res2 =
                cleaned_session_.import_object_from_file("exp1.aaa");

            Assert.IsTrue(res2 != null);
            tran2.rollback();

            {
                List<string> types = new List<string>() { OTYPE2b };

                var objs1 = cleaned_session_.get_object_by_type(types);
                Assert.AreEqual(objs1.Count, 2);
                Assert.AreEqual(objs1[0].get_parameter_value_text("H6"), "y");
                Assert.AreEqual(objs1[1].get_parameter_value_text("H6"), "y");

            }
        }
    }
}