using System;
using System.Collections.Generic;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using og.net;
using System.IO;

namespace test_og_net
{
    public class InitilizeOnce
    {
        private static InitilizeOnce instance = null;
        public static InitilizeOnce GetInstance()
        {
            if(instance == null)
            {
                instance = new InitilizeOnce();
            }
            return instance;
        }

        public InitilizeOnce()
        {
            new FileInfo(TestInitializer.DBPATH_SRC).CopyTo(TestInitializer.DBPATH, true);

            og.net.OGSession cleaned_session_ = new OGSession();
            cleaned_session_.open(TestInitializer.DBPATH);
            cleaned_session_.build();
        }
        public void SetPath()
        {
#if DEBUG
            string config = "debug";
#else
            string config = "release";
#endif
            string boost =
               string.Format("{0}/stage/lib",
                System.Environment.GetEnvironmentVariable("BOOST"));

            string soci =
               string.Format("{0}/{1}/bin64",
                System.Environment.GetEnvironmentVariable("SOCI"),
                config);

            string path = System.Environment.GetEnvironmentVariable("PATH");
            System.Environment.SetEnvironmentVariable(
                "PATH",
                path + ";" + boost + ";" + soci,
                    EnvironmentVariableTarget.Process);
        }
    }

    public class TestInitializer
    {
        public const string DBPATH = "../../../../sql/og.db";
        public const string DBPATH_SRC = "../../../../sql/og_src.db";
        public og.net.OGSession initialize()
        {
            InitilizeOnce.GetInstance();

            og.net.OGSession cleaned_session_ = new OGSession();
            cleaned_session_.open(TestInitializer.DBPATH);
            cleaned_session_.purge();
            cleaned_session_.schema().purge();
            cleaned_session_.close();

            cleaned_session_.open(TestInitializer.DBPATH);

            return cleaned_session_;
        }

    }

    [TestClass]
    public class UnitTest_Etc
    {
        [TestInitialize]
        public void setpath()
        {
            InitilizeOnce.GetInstance().SetPath();
        }

        [TestMethod]
        public void etc()
        {
            etc_1000();
            etc_1000a();
            etc_1000b();
            etc_1000c();
        }
        
        public void etc_1000()
        {
            og.net.OGSession cleaned_session_ = new OGSession();
            cleaned_session_.open(TestInitializer.DBPATH);
            cleaned_session_.purge();
            cleaned_session_.schema().purge();

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

            p1.add_parameter_definition("H2", ptest2);
            p1.add_parameter_definition("H4", ptest4);
            p1.add_parameter_definition("H6", ptest6);

            // apply parameter to schema_relation
            rel_ptr.add_parameter_definition("I1", ptest1);
            rel_ptr.add_parameter_definition("I3", ptest3);
            rel_ptr.add_parameter_definition("I5", ptest5);

            rel_ptr.add_parameter_definition("I2", ptest2);
            rel_ptr.add_parameter_definition("I4", ptest4);
            rel_ptr.add_parameter_definition("I6", ptest6);

            OGSessionObject o1 = cleaned_session_.create_object(p1);
            OGSessionObject o2 = cleaned_session_.create_object(p2);
            OGSessionRelation o1o2 = o1.connect_to(o2, RELTYPE);
            //o1o2.sync();

            //cleaned_session_.schema_.export_to_file("schema.xml");
            cleaned_session_.schema().export_to_file("schema.xml.gz");
            //cleaned_session_.export_to_file("session.xml");
            cleaned_session_.export_to_file("session.xml.gz");
            cleaned_session_.purge();
            cleaned_session_.import_from_file("session.xml.gz");

            List<OGSessionObject> sesn_objs = 
            cleaned_session_.get_object_by_type(otype_list);

            Assert.AreEqual(sesn_objs.Count, 2);

            foreach (OGSessionObject it in sesn_objs)
            {
                if (it.get_schema_object_type() == OTYPE1)
                {
                    List<OGSessionObject> sesn_cons = 
                    it.get_connected_object();
                    Assert.AreEqual(sesn_cons.Count, 1);
                }
            }
        }

        public void etc_1000a()
        {
            og.net.OGSession cleaned_session_ = new OGSession();
            cleaned_session_.open(TestInitializer.DBPATH);
            cleaned_session_.purge();
            cleaned_session_.schema().purge();
            cleaned_session_.close();
            cleaned_session_.open(TestInitializer.DBPATH);

            string OTYPE1 = "Document2002from";
            string OTYPE2 = "Document2002to";
            string RELTYPE = "Document2002rel";
            //string ONAME = "Dcument-Name2002";
            List<string> otype_list = new List<string>();
            otype_list.Add(OTYPE1);
            otype_list.Add(OTYPE2);

            List<string> rel_type_list = new List<string>();
            rel_type_list.Add(RELTYPE);

            cleaned_session_.schema().import_from_file("schema.xml.gz");
            cleaned_session_.import_from_file("session.xml.gz");


            List<OGSessionObject> sesn_objs = 
            cleaned_session_.get_object_by_type(otype_list);

            Assert.AreEqual(sesn_objs.Count, 2);

            foreach (OGSessionObject it in sesn_objs)
            {
                if (it.get_schema_object_type() == OTYPE1)
                {
                    List<OGSessionObject> sesn_cons = 
                    it.get_connected_object();
                    Assert.AreEqual(sesn_cons.Count, 1);
                }
            }
        }


        // xml import
        public void etc_1000b()
        {
            // initialize db
            og.net.OGSession cleaned_session_ = new OGSession();
            cleaned_session_.open(TestInitializer.DBPATH);
            cleaned_session_.purge();
            cleaned_session_.schema().purge();
            cleaned_session_.close();
            cleaned_session_.open(TestInitializer.DBPATH);

            string OTYPE1 = "Document2002from";
            string OTYPE2 = "Document2002a to";
            string OTYPE3 = "Document2002b to";
            string RELTYPE1 = "Document2002rel";
            string RELTYPE2 = "Document2002rel";
            string ONAME = "Dcument-Name2002";
            List<string> otype_list = new List<string>();
            otype_list.Add(OTYPE1);
            otype_list.Add(OTYPE2);
            otype_list.Add(OTYPE3);

            OGSchemaObject p1 = cleaned_session_.schema().create_object(OTYPE1,
                                          ONAME);
            OGSchemaObject p2 = cleaned_session_.schema().create_object(OTYPE2,
                                          ONAME);
            OGSchemaObject p3 = cleaned_session_.schema().create_object(OTYPE3,
                                          ONAME);

            OGSchemaRelation rel_ptr12 = p1.connect_to(p2, RELTYPE1);
            OGSchemaRelation rel_ptr13 = p1.connect_to(p3, RELTYPE2);

            OGInteger type1 = new OGInteger(
             1,
             3,
             3,
             1,
             1);

            OGInteger type2 = new OGInteger(
             2,
             3,
             3,
             1,
             1);

            OGReal type3 = new OGReal(
             1.141,
             200.0,
             200.0,
             1.0,
             1.0);

            OGReal type4 = new OGReal(
              2.236,
              3,
              3,
              1,
              1);

            OGText type5 = new OGText(
             "foo",
             10,
             10,
             1,
             1);

            OGText type6 = new OGText(
             "bar",
             10,
             10,
             1,
             1);

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

            p1.add_parameter_definition("H2", ptest2);
            p1.add_parameter_definition("H4", ptest4);
            p1.add_parameter_definition("H6", ptest6);

            p2.add_parameter_definition("V1", ptest1);
            p2.add_parameter_definition("V3", ptest3);
            p2.add_parameter_definition("V5", ptest5);

            p3.add_parameter_definition("V2", ptest2);
            p3.add_parameter_definition("V4", ptest4);
            p3.add_parameter_definition("V6", ptest6);

            // apply parameter to schema_relation
            rel_ptr12.add_parameter_definition("I1", ptest1);
            rel_ptr12.add_parameter_definition("I3", ptest3);
            rel_ptr12.add_parameter_definition("I5", ptest5);

            rel_ptr12.add_parameter_definition("I2", ptest2);
            rel_ptr12.add_parameter_definition("I4", ptest4);
            rel_ptr12.add_parameter_definition("I6", ptest6);

            rel_ptr13.add_parameter_definition("I1", ptest1);
            rel_ptr13.add_parameter_definition("I3", ptest3);
            rel_ptr13.add_parameter_definition("I5", ptest5);

            rel_ptr13.add_parameter_definition("I2", ptest2);
            rel_ptr13.add_parameter_definition("I4", ptest4);
            rel_ptr13.add_parameter_definition("I6", ptest6);

            OGSessionObject o1 = cleaned_session_.create_object(p1);
            OGSessionObject o2 = cleaned_session_.create_object(p2);
            OGSessionObject o3 = cleaned_session_.create_object(p3);
            OGSessionRelation o1o2 = o1.connect_to(o2, RELTYPE1);
            OGSessionRelation o1o3 = o1.connect_to(o3, RELTYPE2);

            o2.set_parameter_value_integer("V1", 100);
            o2.set_parameter_value_real("V3", 100.0);
            o2.set_parameter_value_text("V5", "100");

            o3.set_parameter_value_integer("V2", 101);
            o3.set_parameter_value_real("V4", 101.0);
            o3.set_parameter_value_text("V6", "101");

            cleaned_session_.schema().export_to_file("schema_xml_catchup0.xml.gz");
            cleaned_session_.export_to_file("session_xml_catchup0.xml.gz");

            // catchup schema
            List<string> prop_type = new List<string>();
            prop_type.Add(OGSchema.schema_property_object_type);

            List<OGSessionObject> prop_objs =
            cleaned_session_.get_object_by_type(prop_type);

            Assert.AreEqual(prop_objs.Count, 1);

            string rev1 = prop_objs[0].get_id();
            OGSessionObject ooo = cleaned_session_.get_property_object();

            string rev2 = ooo.get_id();
            Assert.AreEqual(rev1, rev2);

            //"delete param definition" deletes all session's parameters
            p2.add_parameter_definition(("add int"), ptest1);
            p2.add_parameter_definition(("add real"), ptest3);
            p2.add_parameter_definition(("add text"), ptest5);

            //"delete param definition" deletes all session's parameters
            p3.delete_parameter_definition("V6", ptest6);
            rel_ptr13.delete_parameter_definition("I6", ptest6);

            cleaned_session_.schema().get_property_object().set_comment("set property @@@");
            cleaned_session_.schema().get_property_object().revision_up();
            cleaned_session_.schema().export_to_file("schema_xml_catchup1.xml.gz");

            cleaned_session_.purge();
            Assert.AreEqual(true, cleaned_session_.import_from_file("session_xml_catchup0.xml.gz"));

            List<OGSessionObject> sesn_objs = 
            cleaned_session_.get_object_by_type(otype_list);

            Assert.AreEqual(sesn_objs.Count, 3);

            foreach (var it in sesn_objs)
            {
                if (it.get_schema_object_type() == OTYPE1)
                {
                    List<OGSessionObject> sesn_cons = 
                    it.get_connected_object();
                    Assert.AreEqual(sesn_cons.Count, 2);

                    {
                        string sid = it.get_schema_object_id();
                        OGSchemaObject ssidob =
                          cleaned_session_.schema().get_object(sid);

                        List<Tuple<string, OGSchemaParameter>> __params = new List<Tuple<string, OGSchemaParameter>>();
                        ssidob.get_parameters(__params);

                        Assert.AreEqual(__params.Count, 6);
                    }

                    foreach (var it2 in sesn_cons)
                    {
                        if (it2.get_schema_object_type() == OTYPE2)
                        {
                            List<Tuple<string, OGSchemaParameter>> __params2 = new List<Tuple<string, OGSchemaParameter>>();
                            cleaned_session_.schema().get_object(
                              it2.get_schema_object_id()).get_parameters(__params2);

                            Assert.AreEqual(6, __params2.Count);
                            it2.set_parameter_value_integer("add int", 10);
                            it2.set_parameter_value_real("add real", 1.4142);
                            it2.set_parameter_value_text("add text", "hoge");

                            int i = it2.get_parameter_value_integer("add int");
                            double ii = it2.get_parameter_value_real("add real");
                            string iii = it2.get_parameter_value_text("add text");

                        }
                        if (it2.get_schema_object_type() == OTYPE3)
                        {
                            List<Tuple<string, OGSchemaParameter>> __params3 = new List<Tuple<string, OGSchemaParameter>>();
                            cleaned_session_.schema().get_object(
                              it2.get_schema_object_id()).get_parameters(__params3);

                            Assert.AreEqual(2, __params3.Count);
                        }
                    }
                }
            }
        }

        public void etc_1000c()
        {

            og.net.OGSession cleaned_session_ = new OGSession();
            cleaned_session_.open(TestInitializer.DBPATH);
            cleaned_session_.purge();
            cleaned_session_.schema().purge();
            cleaned_session_.close();
            cleaned_session_.open(TestInitializer.DBPATH);

            string OTYPE1 = "Document2002from";
            string OTYPE2 = "Document2002a to";
            string OTYPE3 = "Document2002b to";
            string RELTYPE1 = "Document2002rel";
            string RELTYPE2 = "Document2002rel";
            string ONAME = "Dcument-Name2002";
            List<string> otype_list = new List<string>();
            otype_list.Add(OTYPE1);
            otype_list.Add(OTYPE2);
            otype_list.Add(OTYPE3);

            // pre condition
            cleaned_session_.schema().import_from_file("schema_xml_catchup0.xml.gz");
            cleaned_session_.import_from_file("session_xml_catchup0.xml.gz");

            bool res = cleaned_session_.catchup_schema("schema_xml_catchup1.xml.gz");

            Assert.AreEqual(true, res);

            List<OGSessionObject> sesn_objs =
            cleaned_session_.get_object_by_type(otype_list);

            Assert.AreEqual(sesn_objs.Count, 3);

            foreach (var it in sesn_objs)
            {
                if (it.get_schema_object_type() == OTYPE1)
                {
                    List<OGSessionObject> sesn_cons =
                    it.get_connected_object();
                    Assert.AreEqual(sesn_cons.Count, 2);

                    {
                        string sid = it.get_schema_object_id();
                        OGSchemaObject ssidob =
                          cleaned_session_.schema().get_object(sid);

                        List<Tuple<string, OGSchemaParameter>> __params = new List<Tuple<string, OGSchemaParameter>>();
                        ssidob.get_parameters(__params);

                        Assert.AreEqual(6, __params.Count);
                    }

                    foreach (var it2 in sesn_cons)
                    {
                        if (it2.get_schema_object_type() == OTYPE2)
                        {
                            List<Tuple<string, OGSchemaParameter>> __params2 = new List<Tuple<string, OGSchemaParameter>>();
                            cleaned_session_.schema().get_object(
                              it2.get_schema_object_id()).get_parameters(__params2);

                            Assert.AreEqual(6, __params2.Count);
                            it2.set_parameter_value_integer("add int", 10);
                            it2.set_parameter_value_real("add real", 1.4142);
                            it2.set_parameter_value_text("add text", "hoge");

                            int i = it2.get_parameter_value_integer("add int");
                            double ii = it2.get_parameter_value_real("add real");
                            string iii = it2.get_parameter_value_text("add text");
                        }

                        if (it2.get_schema_object_type() == OTYPE3)
                        {
                            List<Tuple<string, OGSchemaParameter>> __params3 = new List<Tuple<string, OGSchemaParameter>>();
                            cleaned_session_.schema().get_object(
                              it2.get_schema_object_id()).get_parameters(__params3);

                            Assert.AreEqual(2, __params3.Count);
                        }
                    }
                }
            }


            /// property tests
            // initialize db
            OGSchemaObject schm_prop =
              cleaned_session_.schema().get_property_object();

            schm_prop.set_revision("100");
            cleaned_session_.schema().export_to_file("rev100.xml.gz");

            schm_prop.set_revision("99");
            cleaned_session_.catchup_schema("rev100.xml.gz");

            OGSchemaObject schm_prop2 =
              cleaned_session_.schema().get_property_object();
            Assert.AreEqual(schm_prop2.get_revision(), "100");

            schm_prop2.set_revision("101");
            cleaned_session_.catchup_schema("rev100.xml.gz");

            OGSchemaObject schm_prop3 =
              cleaned_session_.schema().get_property_object();
            Assert.AreEqual(schm_prop3.get_revision(), "101");
        }

        // xml import
        //[TestMethod]
        //public void etc_schema_update()
        //{
        //    OGSession session = null;

        //    session = new OGSession();
        //    session.open("project.db");

        //    if (session.catchup_schema("GhostProject.schema.xml.gz"))
        //    {
        //    }
        //    else
        //    {
        //        Assert.Fail();
        //    }
        //}
    }
}
