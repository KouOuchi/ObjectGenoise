#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>
#include <exception>

#include "og/og_session.h"

using namespace std;
using namespace boost::filesystem;

void PrintUsage(string mes)
{
  cout << "usage: [db file] [schema file]" << endl;
  cout << endl;
  cout << mes << endl;
  cout << endl;
}

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    PrintUsage("number of argument is not 2.");
    exit(1);
  }

  path db(argv[1]);
  path schema(argv[2]);

  if (!exists(db))
  {
    PrintUsage("arg 0 is not exists.");
    exit(1);
  }
  if (!exists(schema))
  {
    PrintUsage("arg 1 is not exists.");
    exit(1);
  }

  og::og_session session;

  try
  {
    session.open(argv[1]);
  }
  catch (exception e)
  {
    PrintUsage(e.what());
    exit(1);
  }

  try
  {
    if (session.catchup_schema(argv[2]))
    {
      PrintUsage("Catchup schemea success.");
      exit(0);
    }
    else
    {
      PrintUsage("Chatchup schema failure.");
      exit(1);
    }
  }
  catch (exception e)
  {
    PrintUsage(e.what());
    exit(1);
  }
}

