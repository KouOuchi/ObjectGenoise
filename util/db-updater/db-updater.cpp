#include <iostream>
#include <sstream>
#include <exception>

#include "og/og_session.h"

using namespace std;
using namespace System;
using namespace System::Security;
using namespace System::IO;

void PrintUsage(string mes)
{
  cout << "db-updater(20221103)" << endl;
  cout << "usage: [db file] [schema file]" << endl;
  cout << endl;
  cout << mes << endl;
  cout << endl;
}

int main(int argc, char* argv[])
{
  if (!(argc == 3 || argc == 4))
  {
    PrintUsage("number of argument is not 2 or not 4.");
    exit(1);
  }

  System::String^ db = gcnew System::String(argv[1]);
  System::String^ schema = gcnew System::String(argv[2]);
  System::Text::Encoding^ u8 = System::Text::Encoding::UTF8;

  if (!File::Exists(db))
  {
    PrintUsage("arg 0 is not exists.");
    exit(1);
  }
  if (!File::Exists(schema))
  {
    PrintUsage("arg 1 is not exists.");
    exit(1);
  }

  og::og_session session;

  try
  {
    System::String^ arg1str = gcnew System::String(argv[1]);
    cli::array<unsigned char>^ bytes_arg1 = u8->GetBytes(arg1str);
    pin_ptr<unsigned char> args1Ptr = &bytes_arg1[0];

    session.open(argv[1]);
  }
  catch (exception e)
  {
    stringstream ss;
    ss << "Exception.1 ";
    ss << e.what();
    PrintUsage(ss.str());
    exit(1);
  }

  try
  {
    System::String^ arg2str = gcnew System::String(argv[2]);
    cli::array<unsigned char>^ bytes_arg2 = u8->GetBytes(arg2str);
    pin_ptr<unsigned char> args2Ptr = &bytes_arg2[0];

    if (argc == 3)
    {
      System::String^ tempdir = Path::GetTempPath();

      cli::array<unsigned char>^ bytes_temp = u8->GetBytes(tempdir);
      pin_ptr<unsigned char> tmpdir_Ptr = &bytes_temp[0];

      if (session.catchup_schema((const char*)args2Ptr, (const char*)tmpdir_Ptr))
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
    else
    {
      System::String^ arg3str = gcnew System::String(argv[3]);
      cli::array<unsigned char>^ bytes_arg3 = u8->GetBytes(arg3str);
      pin_ptr<unsigned char> args3Ptr = &bytes_arg3[0];

      if (session.reload_schema((const char*)args2Ptr, (const char*)args3Ptr))
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
  }
  catch (exception e)
  {
    stringstream ss;
    ss << "Exception.2 ";
    ss << e.what();
    PrintUsage(ss.str());
    exit(1);
  }
}

