# ObjectGenoise

ObjectGenoise is a ORM, database middle ware and C++/.Net library.
  - Pseudo dynamic class definition. Its serialize and deserialize.
  - Composite and Relation Class Association are supported. 

## Technical Overview

![schema pdf](https://github.com/KouOuchi/ObjectGenoise/blob/master/sql/OG-Schema.png)
[Schema PDF] and [Schema SQL]


## Development
This package includes 4 projects:
- core : C++ : namespace "og.core" has a main functions. namespace "og" has a public headers.
- og.net : C++/CLI : wrapper for core.
- test.og.net : C# test code.
- test-core : C++ boost test code.

And support files :
- db.eap : ER-Model for EnterpriseArchitect(EA). EA can generates SQL for various DBMS.
- conv.sh : Convert SQL code to C++ header.

## Prerequisite
ObjectGenoise depends on the following package
* [boost] 1.57
	Note: while using boost_log from c++/cli, I'm faced with crash.
	So, i use this [buildscript]. 

* [soci] 3.2 
* [SQLite3] 3.8.7

## Building
You need 3 environment variable as follows : 
- BOOST
- SOCI
- SQLITE3

If you get headers and libraries and set the variables to environment, you can see them :
```sh
> set BOOST
BOOST=c:\path\to\boost
> set SOCI
SOCI=c:\path\to\soci
> set SQLITE3
SQLITE3=c:\path\to\sqlite3
```

And then, create lib directory and copy libraries as follows:
```
> $ find lib -type f
> 
> lib/debug/boost_chrono-vc120-mt-gd-1_57.dll
> lib/debug/boost_date_time-vc120-mt-gd-1_57.dll
> lib/debug/boost_filesystem-vc120-mt-gd-1_57.dll
> lib/debug/boost_iostreams-vc120-mt-gd-1_57.dll
> lib/debug/boost_locale-vc120-mt-gd-1_57.dll
> lib/debug/boost_log-vc120-mt-gd-1_57.dll
> lib/debug/boost_log_setup-vc120-mt-gd-1_57.dll
> lib/debug/boost_regex-vc120-mt-gd-1_57.dll
> lib/debug/boost_system-vc120-mt-gd-1_57.dll
> lib/debug/boost_thread-vc120-mt-gd-1_57.dll
> lib/debug/boost_timer-vc120-mt-gd-1_57.dll
> lib/debug/boost_unit_test_framework-vc120-mt-gd-1_57.dll
> lib/debug/boost_zlib-vc120-mt-gd-1_57.dll
> lib/release/boost_chrono-vc120-mt-1_57.dll
> lib/release/boost_date_time-vc120-mt-1_57.dll
> lib/release/boost_filesystem-vc120-mt-1_57.dll
> lib/release/boost_iostreams-vc120-mt-1_57.dll
> lib/release/boost_locale-vc120-mt-1_57.dll
> lib/release/boost_log-vc120-mt-1_57.dll
> lib/release/boost_log_setup-vc120-mt-1_57.dll
> lib/release/boost_regex-vc120-mt-1_57.dll
> lib/release/boost_system-vc120-mt-1_57.dll
> lib/release/boost_thread-vc120-mt-1_57.dll
> lib/release/boost_timer-vc120-mt-1_57.dll
> lib/release/boost_unit_test_framework-vc120-mt-1_57.dll
> lib/release/boost_zlib-vc120-mt-1_57.dll
> lib/soci_core_3_2.dll
> lib/soci_empty_3_2.dll
> lib/soci_sqlite3_3_2.dll
```

*** Windows
use VisualStudio2013. Set platform to x64.

## Version and History
0.0.1 initial revision 
## Todo's
 - Code : Write CMake build script.
 - Code : Add Code Comments.
 - Code : Add Postgresql and Mysql test.
 - Code : Add tests on *nix.
 - Function : Search Criteria
 - Function : Add embedded type. Now i have 3 basic types: integer, real and text.
 - Function : User or Domain control
 - Function : Multiprexity validation.
 - Function : "Auto Revision Up" and "Catch Up schema revision"
 - Function : (under investigtoin) Attach something UML like editor.
 - Function : (under investigtoin) Multiple Revision management both schema and session. 
 - Tool : Session instances viewer.
 - Site : Upload and share your schema. 

## Licenses and Authors
  * Licensed under the [Boost Software License 1.0]
  * Copyright &copy; 2015 Kou Ouchi <kou.ouchi@division-engineering.com>
  
## Related Information
- [Enterprise Architect] : Data Modeling editor. "CodeEngineering" function can generates DDL of various DataBase Systems. 
- [PupSQLite] : Cool and lightweight SQLite3 viewer.
- [Astyle] : A Free, Fast, and Small Automatic Formatter
for C, C++, C++/CLI, Objective?C, C#, and Java Source Code
- [Google C++ Style Guid]

[soci]:http://soci.sourceforge.net/
[boost]:http://www.boost.org/
[Enterprise Architect]:http://www.sparxsystems.com/products/ea/index.html
[PupSQLite]:https://www.eonet.ne.jp/~pup/software.html
[Boost Software License 1.0]:http://www.boost.org/LICENSE_1_0.txt
[SQLite3]:http://soci.sourceforge.net/
[Astyle]:http://astyle.sourceforge.net/
[Google C++ Style Guid]:http://google-styleguide.googlecode.com/svn/trunk/cppguide.html
[Schema PDF]:https://github.com/KouOuchi/ObjectGenoise/blob/master/sql/OG-Schema.pdf
[Schema SQL]:https://github.com/KouOuchi/ObjectGenoise/blob/master/sql/OG-Schema.sql
[buildscript]:https://github.com/KouOuchi/ObjectGenoise/blob/master/etc/boost_build.bat
