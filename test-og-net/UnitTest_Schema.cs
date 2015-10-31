using System;
using System.Collections.Generic;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using og.net;

namespace test_og_net
{
    [TestClass]
    public class UnitTest_Schema
    {
        void _check_OGSchemaObject(OGSchemaObject _soptr, string _oid, string _otype,
                                  string _oname)
        {
            // type check
            Assert.IsTrue(_soptr.get_id() == _oid);
            Assert.IsTrue(_soptr.get_name() == _oname);
            Assert.IsTrue(_soptr.get_type() == _otype);
        }

        [TestMethod]
        public void schema_1000()
        {
            OGSession cleaned_session_ = new TestInitializer().initialize();

            string OTYPE = "Document";
            string ONAME = "Dcument-Name";

            OGSchemaObject obj = cleaned_session_.schema().create_object(OTYPE, ONAME);
            Assert.AreEqual(obj.get_revision(), "0");
            obj.revision_up();

            // instance check
            _check_OGSchemaObject(obj, obj.get_id(), OTYPE, ONAME);

            // get_object_id
            OGSchemaObject soptr = cleaned_session_.schema().get_object(obj.get_id());
            Assert.AreEqual(soptr.get_revision(), "1");

            _check_OGSchemaObject(soptr, obj.get_id(), OTYPE, ONAME);

            List<string> type_list = new List<string>();
            type_list.Add(OTYPE);

            List<OGSchemaObject> soref_list1 = 
            cleaned_session_.schema().get_object_by_type(type_list);

            foreach (OGSchemaObject so in soref_list1)
            {
                Assert.AreEqual(so.get_type(), OTYPE);
                Assert.AreEqual(so.get_name(), ONAME);
            }

        }

        // over OGSession and get/set comment/revision
        [TestMethod]
        public void schema_1001()
        {
            OGSession cleaned_session_ = new TestInitializer().initialize();

            string OTYPE = "Document1001";
            string ONAME = "Dcument-Name1001";
            string oid;
            {
                OGSchemaObject soptr = cleaned_session_.schema().create_object(OTYPE, ONAME);
                soptr.set_comment("hogehoge");
                soptr.set_name("foobar");

                // get object id
                oid = soptr.get_id();
            }

            {
                OGSession ses2 = new OGSession();
                ses2.open(TestInitializer.DBPATH);

                // get_object_id
                OGSchemaObject soptr2 = ses2.schema().get_object(oid);

                _check_OGSchemaObject(soptr2, oid, OTYPE, "foobar");
                Assert.AreEqual(soptr2.get_comment(), "hogehoge");
            }
        }

        // get object by type
        [TestMethod]
        public void schema_1002()
        {
            OGSession cleaned_session_ = new TestInitializer().initialize();

            string OTYPE1 = "Document1002a";
            string OTYPE2 = "Document1002b";
            string ONAME = "Dcument-Name1002";
            List<string> otype_list = new List<string>();
            otype_list.Add(OTYPE1);
            otype_list.Add(OTYPE2);
            {
                // get_object_id
                cleaned_session_.schema().create_object(OTYPE1, ONAME);
                cleaned_session_.schema().create_object(OTYPE2, ONAME);

                List<OGSchemaObject> obj_list1 = 
                cleaned_session_.schema().get_object_by_type(otype_list);

                Assert.AreEqual(obj_list1.Count, 2);

                List<string> types = 
                cleaned_session_.schema().get_object_type();
                Assert.IsTrue(types.Contains(OTYPE1));
                Assert.IsTrue(types.Contains(OTYPE2));
            }

            {
                OGSession ses2 = new OGSession();
                ses2.open(TestInitializer.DBPATH);
                List<OGSchemaObject> obj_list1 = 
                ses2.schema().get_object_by_type(otype_list);

                Assert.IsTrue(obj_list1.Count == 2);

                foreach (OGSchemaObject o in obj_list1)
                {
                    Assert.IsTrue(o.get_type() == OTYPE1 || o.get_type() == OTYPE2);
                    Assert.AreEqual(o.get_name(), ONAME);
                }
            }
        }

        // get object by name
        [TestMethod]
        public void schema_1003()
        {
            OGSession cleaned_session_ = new TestInitializer().initialize();

            string OTYPE1 = "Document1003a";
            string OTYPE2 = "Document1003b";
            string ONAME1 = "Dcument-Name1003a";
            string ONAME2 = "Dcument-Name1003b";
            List<string> oname_list = new List<string>();
            oname_list.Add(ONAME1);
            oname_list.Add(ONAME2);
            {
                // get_object_id

                cleaned_session_.schema().create_object(OTYPE1, ONAME1);
                cleaned_session_.schema().create_object(OTYPE2, ONAME2);

                List<OGSchemaObject> obj_list2 =
                cleaned_session_.schema().get_object_by_name(oname_list);

                Assert.AreEqual(obj_list2.Count, 2);
            }

            {
                OGSession ses2 = new OGSession();
                ses2.open(TestInitializer.DBPATH);
                List<OGSchemaObject> obj_list2 = 
                ses2.schema().get_object_by_name(oname_list);

                Assert.IsTrue(obj_list2.Count == 2);

                foreach (OGSchemaObject o in obj_list2)
                {
                    Assert.IsTrue((o.get_type() == OTYPE1 && o.get_name() == ONAME1) ||
                                  (o.get_type() == OTYPE2 && o.get_name() == ONAME2));
                }
            }
        }

        // null/nothing test
        [TestMethod]
        public void schema_1005()
        {
            OGSession cleaned_session_ = new TestInitializer().initialize();
            
            string OTYPE = "Document";
            string ONAME = "Dcument-Name";
            string RELTYPE = "Document2000rel";

            List<string> otype_list = new List<string>();
            List<string> rel_type_list = new List<string>();
            otype_list.Add(OTYPE);
            rel_type_list.Add(RELTYPE);

            OGSchemaObject obj_ptr = cleaned_session_.schema().create_object(OTYPE, ONAME);

            string oid = obj_ptr.get_id();

            obj_ptr.delete_object();

            OGSchemaObject obj_ptr2 = cleaned_session_.schema().get_object(oid);

            Assert.IsTrue(obj_ptr2 == null);
        }

        // relation object
        [TestMethod]
        public void schema_2000()
        {
            OGSession cleaned_session_ = new TestInitializer().initialize();

            string OTYPE1 = "Document2000from";
            string OTYPE2 = "Document2000to";
            string RELTYPE = "Document2000rel";
            string ONAME = "Dcument-Name2000";
            string RELNAME = "Rel-Name2000";
            List<string> otype_list = new List<string>();
            otype_list.Add(OTYPE1);
            otype_list.Add(OTYPE2);

            List<string> rel_type_list = new List<string>();
            rel_type_list.Add(RELTYPE);

            string from_id, to_id, rel_id;

            {
                OGSchemaObject p1 = cleaned_session_.schema().create_object(OTYPE1, ONAME);
                OGSchemaObject p2 = cleaned_session_.schema().create_object(OTYPE2, ONAME);

                // store id
                from_id = p1.get_id();
                to_id = p2.get_id();
                OGSchemaRelation rel_ptr = p1.connect_to(p2, RELTYPE);
                Assert.AreEqual(rel_ptr.get_revision(), "0");

                rel_id = rel_ptr.get_id();

                rel_ptr.set_comment("hoge");
                rel_ptr.set_name(RELNAME);
                rel_ptr.revision_up();

                List<string> rels = 
                cleaned_session_.schema().get_relation_type();
                Assert.AreEqual(rels.Count, 1);
            }

            {
                List<OGSchemaRelation> rel_list = 
                cleaned_session_.schema().get_relation_by_type(rel_type_list);
                Assert.IsTrue(rel_list.Count == 1);

                foreach (OGSchemaRelation rp in rel_list)
                {
                    Assert.AreEqual(rp.get_comment(), "hoge");
                    Assert.AreEqual(rp.get_name(), RELNAME);
                    Assert.AreEqual(rp.get_revision(), "1");
                }

                OGSchemaRelation rp1 = cleaned_session_.schema().get_relation(rel_id);

                Assert.IsTrue(rp1 != null);
                Assert.AreEqual(rp1.get_comment(), "hoge");
                Assert.AreEqual(rp1.get_name(), RELNAME);

                OGSchemaObject o1 = cleaned_session_.schema().get_object(from_id);
                OGSchemaObject o2 = cleaned_session_.schema().get_object(to_id);

                List<OGSchemaObject> obj_list1 = o1.get_connected_object();

                // get composite
                
                Assert.IsTrue(obj_list1.Count == 1);
                foreach (OGSchemaObject o in obj_list1)
                {
                    Assert.IsTrue(o.get_id() == o2.get_id());
                }

                // get composite
                obj_list1 = o1.get_connected_object_to();
                Assert.IsTrue(obj_list1.Count == 1);
                foreach (OGSchemaObject o in obj_list1)
                {
                    Assert.IsTrue(o.get_id() == o2.get_id());
                }

                // get composite
                obj_list1 = o2.get_connected_object_from();

                Assert.IsTrue(obj_list1.Count == 1);

                foreach (OGSchemaObject o in obj_list1)
                {
                    Assert.IsTrue(o.get_id() == o1.get_id());
                }

                // get composite
                obj_list1 = o1.get_connected_object(rel_type_list);

                Assert.IsTrue(obj_list1.Count == 1);

                foreach (OGSchemaObject o in obj_list1)
                {
                    Assert.IsTrue(o.get_id() == o2.get_id());
                }

                obj_list1 = o1.get_connected_object_to(rel_type_list);
                Assert.IsTrue(obj_list1.Count == 1);
                foreach (OGSchemaObject o in obj_list1)
                {
                    Assert.IsTrue(o.get_id() == o2.get_id());
                }

                // get composite
                obj_list1 =
                o2.get_connected_object_from(rel_type_list);
                Assert.IsTrue(obj_list1.Count == 1);
                foreach (OGSchemaObject o in obj_list1)
                {
                    Assert.IsTrue(o.get_id() == o1.get_id());
                }

                // get composite but get nobody
                obj_list1 =
                o1.get_connected_object_from();
                Assert.IsTrue(obj_list1.Count == 0);

                // get composite but get nobody
                obj_list1 =
                o2.get_connected_object_to();
                Assert.IsTrue(obj_list1.Count == 0);

                // get composite but get nobody
                obj_list1 =
                o1.get_connected_object_from(rel_type_list);
                Assert.IsTrue(obj_list1.Count == 0);

                // get composite but get nobody
                obj_list1 =
                o2.get_connected_object_to(rel_type_list);
                Assert.IsTrue(obj_list1.Count == 0);
            }
        }

        // relation auto delete test
        [TestMethod]
        public void schema_2001()
        {
            OGSession cleaned_session_ = new TestInitializer().initialize();

            string OTYPE1 = "Document2001from";
            string OTYPE2 = "Document2001to";
            string RELTYPE = "Document2001rel";
            string ONAME = "Dcument-Name2001";
            //string RELNAME = "Rel-Name2001";
            List<string> otype_list = new List<string>();
            otype_list.Add(OTYPE1);
            otype_list.Add(OTYPE2);

            List<string> rel_type_list = new List<string>();
            rel_type_list.Add(RELTYPE);

            OGSchemaObject p1 = cleaned_session_.schema().create_object(OTYPE1, ONAME);
            OGSchemaObject p2 = cleaned_session_.schema().create_object(OTYPE2, ONAME);

            OGSchemaRelation rel_ptr = p1.connect_to(p2, RELTYPE);

            p2.delete_object();

            // auto disconnect
            List<OGSchemaObject> obj_list = 
            p1.get_connected_object_to(rel_type_list);

            Assert.IsTrue(obj_list.Count == 0);
        }

        // conected relation
        [TestMethod]
        public void schema_2002()
        {
            OGSession cleaned_session_ = new TestInitializer().initialize();

            string OTYPE1 = "Document2002from";
            string OTYPE2 = "Document2002to";
            string RELTYPE = "Document2002rel";
            string RELTYPE2 = "Document2002rel2";
            string ONAME = "Dcument-Name2002";
            //string RELNAME = "Rel-Name2002";
            List<string> otype_list = new List<string>();
            otype_list.Add(OTYPE1);
            otype_list.Add(OTYPE2);

            List<string> rel_type_list = new List<string>();
            rel_type_list.Add(RELTYPE);

            OGSchemaObject p1 = cleaned_session_.schema().create_object(OTYPE1, ONAME);
            OGSchemaObject p2 = cleaned_session_.schema().create_object(OTYPE2, ONAME);
            OGSchemaObject p3 = cleaned_session_.schema().create_object(OTYPE1, ONAME);
            OGSchemaObject p4 = cleaned_session_.schema().create_object(OTYPE2, ONAME);

            OGSchemaRelation rel_ptr = p1.connect_to(p2, RELTYPE);
            OGSchemaRelation rel_ptr2 = p3.connect_to(p4, RELTYPE2);

            List<OGSchemaRelation> rel_list2 =
            cleaned_session_.schema().get_connected_relation_to(
              p1.get_id(),
              rel_type_list);

            Assert.IsTrue(rel_list2.Count == 1);
            Assert.IsTrue(rel_list2[0].get_id() == rel_ptr.get_id());

            rel_list2 =
            cleaned_session_.schema().get_connected_relation_from(
              p2.get_id(),
              rel_type_list);


            Assert.IsTrue(rel_list2.Count == 1);
            Assert.IsTrue(rel_list2[0].get_id() == rel_ptr.get_id());

            rel_list2 =
            cleaned_session_.schema().get_connected_relation_to(
              p2.get_id(),
              rel_type_list);


            Assert.IsTrue(rel_list2.Count == 0);


            rel_list2 =
            cleaned_session_.schema().get_connected_relation_from(
              p1.get_id(),
              rel_type_list);

            Assert.IsTrue(rel_list2.Count == 0);
        }
    }
}
