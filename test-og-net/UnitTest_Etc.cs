using System;
using System.Collections.Generic;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using og.net;
using System.IO;

namespace test_og_net
{
    public class TestInitializer
    {
        public const string DBPATH = "../../../../sql/og.db";
        const string DBPATH_SRC = "../../../../sql/og_src.db";
        public og.net.OGSession initialize()
        {
            new FileInfo(DBPATH_SRC).CopyTo(DBPATH, true);

            og.net.OGSession cleaned_session_ = null;
            cleaned_session_ = new OGSession();
            cleaned_session_.connect(DBPATH);
            cleaned_session_.purge();
            cleaned_session_.schema().purge();
            cleaned_session_.build();
            return cleaned_session_;
        }
    }


    [TestClass]
    public class UnitTest_Etc
    {
        og.net.OGSession cleaned_session_ = null;

        [TestInitialize]
        public void TestInitialize()
        {
            cleaned_session_ = new TestInitializer().initialize();
        }

        [TestMethod]
        public void etc_1000()
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

            OGInteger type1 = new OGInteger(1,1,3,1,3);
            OGInteger type2 = new OGInteger(2,1,3,1,3);
            OGReal type3 = new OGReal(1.141, 1.0,3.0,1.0,3.0);
            OGReal type4 = new OGReal(2.236, 1.0,3.0,1.0,3.0);
            OGText type5 = new OGText("foo", 1,1,1,1);
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

            List<OGSessionObject> sesn_objs = new List<OGSessionObject>();
            cleaned_session_.get_object_by_type(otype_list, sesn_objs);

            Assert.AreEqual(sesn_objs.Count, 2);

            foreach (OGSessionObject it in sesn_objs)
            {
                if (it.get_schema_object_type() == OTYPE1)
                {
                    List<OGSessionObject> sesn_cons = new List<OGSessionObject>();
                    it.get_connected_object(sesn_cons);
                    Assert.AreEqual(sesn_cons.Count, 1);
                }
            }
        }

        // xml import
        [TestMethod]
        public void etc_1001()
        {
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


            List<OGSessionObject> sesn_objs = new List<OGSessionObject>();
            cleaned_session_.get_object_by_type(otype_list, sesn_objs);

            Assert.AreEqual(sesn_objs.Count, 2);

            foreach (OGSessionObject it in sesn_objs)
            {
                if (it.get_schema_object_type() == OTYPE1)
                {
                    List<OGSessionObject> sesn_cons = new List<OGSessionObject>();
                    it.get_connected_object(sesn_cons);
                    Assert.AreEqual(sesn_cons.Count, 1);
                }
            }
        }
    }
}



