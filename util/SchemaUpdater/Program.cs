using og.net;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.XPath;

namespace SchemaUpdater
{
    class Program
    {
        /// <summary>
        /// exit 0: Updated. normally exited.
        /// exit 1: Not updated because it is latest. 
        /// exit 2: Not updated because schema file is old.
        /// exit -1: Not updated because something is wrong. 
        /// </summary>
        /// <param name="args"></param>
        static void Main(string[] args)
        {
            if (args.Length != 2)
            {
                PrintUsage("number of argument is not 2.");
                Environment.Exit(1);
            }

            if (!new FileInfo(args[0]).Exists)
            {
                PrintUsage("arg 0 is not exists.");
                Environment.Exit(1);
            }
            if (!new FileInfo(args[1]).Exists)
            {
                PrintUsage("arg 1 is not exists.");
                Environment.Exit(1);
            }

            OGSession session = null;

            try
            {
                session = new OGSession();
                session.open(args[0]);
            }
            catch (Exception e)
            {
                PrintUsage("Can't open ObjectGenoise sessoin. :" + e.Message);
                Environment.Exit(1);
            }

            try
            {
                if (session.catchup_schema(args[1]))
                {
                    PrintUsage("Catchup schemea does.");
                    Environment.Exit(0);
                }
                else
                {
                    PrintUsage("Chatchup schema doesn't do.");
                    Environment.Exit(1);
                }
            }
            catch (Exception e)
            {
                PrintUsage("Something wrong. :" + e.Message);
                Environment.Exit(1);
            }
        }

        static void PrintUsage(string mes)
        {
                Console.WriteLine("usage: [db file] [schema file]");
                Console.WriteLine("");
                Console.WriteLine(mes);
                Console.WriteLine("");
        }
    }
}
