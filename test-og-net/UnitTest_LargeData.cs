using System;
using System.Collections.Generic;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using og.net;
using System.IO;

namespace test_og_net
{
    [TestClass]
    public class UnitTest_LargeData
    {
        [TestMethod]
        public void large_data_1001()
        {
            OGSession cleaned_session_ = new TestInitializer().initialize();

            string OTYPE1 = "Document2002from";
            string OTYPE2 = "Document2002to";
            string RELTYPE = "Document2002rel";
            string ONAME = "Dcument-Name2002";
            //string RELNAME = "Rel-Name2002";
            List<string> otype_list = new List<string>();
            otype_list.Add(OTYPE1);
            otype_list.Add(OTYPE2);

            List<string> rel_type_list= new List<string>();
            rel_type_list.Add(RELTYPE);

            OGSchemaObject p1 = cleaned_session_.schema().create_object(OTYPE1, ONAME);
            OGSchemaObject p2 = cleaned_session_.schema().create_object(OTYPE2, ONAME);


            for (int i = 0; i < 500; i++)
            {
                cleaned_session_.schema().create_object(OTYPE2, ONAME);
            }

            List<string> oname_list = new List<string>(); 
            oname_list.Add(ONAME);
            List<OGSchemaObject> olist = new List<OGSchemaObject>();
            cleaned_session_.schema().get_object_by_name(oname_list, olist);
            Assert.AreEqual(olist.Count, 502);
            olist.Clear();

            cleaned_session_.schema().get_object_by_type(otype_list, olist);
            Assert.AreEqual(olist.Count, 502);

            OGSchemaRelation rel_ptr = p1.connect_to(p2, RELTYPE);

            OGInteger type1 = new OGInteger(100, 0,200,0,200);

            OGSchemaParameter booltest1 =
              cleaned_session_.schema().create_parameter_interger("hoge_type1", "comment1",
                  type1, 1000, 2, 2000);

            // apply parameter to schema_object
            p1.add_parameter_definition("H1", booltest1);

            // apply parameter to schema_relation
            rel_ptr.add_parameter_definition("H2", booltest1);

            // session rollback
            {
                OGSessionObject o1 = cleaned_session_.create_object(p1);
                //o1.sync();

                OGSessionObject o1_2 = cleaned_session_.get_object(o1.get_id());
                if (o1_2 == null) { Assert.Fail("null"); }

                List<int> h1 =
                o1_2.get_parameter_values_integer("H1");

                Assert.AreEqual(h1.Count, 1000);
                foreach (int i in h1)
                {
                    Assert.AreEqual(i, 100);
                }

                List<int> h2 = new List<int>();
                for (int i = 0; i < 888; i++)
                {
                    h2.Add(200);
                }

                o1_2.set_parameter_values_integer("H1", h2);

                List<int> h3 =
                o1_2.get_parameter_values_integer("H1");
                Assert.AreEqual(h3.Count, 888);
                foreach (int i in h3)
                {
                    Assert.AreEqual(i, 200);
                }
            }
        }
    }
}

