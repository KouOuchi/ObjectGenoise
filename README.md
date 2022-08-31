# ObjectGenoise

ObjectGenoise is an ORM, SQLite3 middle ware and C++/.Net library using [SOCI]. 

## Technical Overview

### ObjectGenoise has 2 features:
- schema : a definition of class.
- session : an instance of class.
```
   features
       | Schema                | Session
       +---                    +---
       | SchemaObject     ===> SessionObject
       | SchemaRelation   ===> SessionRelation
       | SchemaParamaeter ===> SessionParameter

    notation_of_session
       +-------------------+
       |<SessionObject>    | 0..*
       +-------------------+<----+
       |<SessionParameters>|     |
       +---------+---------+     |
                 | 0..*          |
                 +---------------+
                                  \
                             +-------------------+
                             |<SessionRelation>  |
                             +-------------------+
                             |<SessionParameters>|
                             +-------------------+
```

### Relation Operatoins : connect, disconnect and copy_object.
- SessionObject holds parameters.
- SessionObject can have relationship of the other classes. The relationship is described as SessionRelation.
- SessionRelation also holds parameters.
- SessionRelation has an direction. The direction is used while retrieving SessionObjects.
```
usage of connect and copy
      Precondition :
       +-------------------+
       |<SchemaObject> SO_A|
       +-------------------+----+
                                _\/
                  |          +----------------------+
                             |<SchemaRelation> SR_X |
                  |          +----------------------+----+
                                                         _\/
                  |                                   +-------------------+
                                                      |<SchemaObject> SO_B|
                  |                                   +-------------------+
       +---------------------+
       |<SessionObject> EO_A1| 	       	       	       	       	|
       +---------------------+
                                 	                              |
                                                      +---------------------+
                                                      |<SessoinObject> EO_B1|
                                                      +---------------------+

      After creating 2 sessoin objects:
      > EO_A1->connect_to(EO_B1, "<type of SR_X>");

      If you test the connection :
      > EO_A1->validate_connect_to(EO_B1, "<type of ER_X>");

      Otherwise, you can connect in the other way:
      > EO_B1->connect_from(EO_A1, "<type of ER_X>");

       +-------------------+
       |<SchemaObject> SO-A|
       +-------------------+----+
                                _\/
                  |          +----------------------+
                             |<SchemaRelation> SR-X |
                  |          +----------------------+----+
                                         |               _\/
                  |                                   +-------------------+
                                         |            |<SchemaObject> SO-B|
                  |                                   +-------------------+
       +---------------------+		       |
       |<SessionObject> EO-A1|                                    |
       +---------------------+----+	     |
                                  _\/                             |
                             +-----------------------+
                             |<SessionRelation> ER-X1|            |
                             +-----------------------+----+
                                                          _\/     |
                                                      +---------------------+
                                                      |<SessoinObject> EO-B1|
                                                      +---------------------+

      And more, You can also connect again:
      ===> EO_A1->connect_to(EO_B1, "<type of SR_X>");

       +-------------------+
       |<SchemaObject> SO-A|
       +-------------------+----+
                                _\/
                  |          +----------------------+
                             |<SchemaRelation> SR-X |
                  |          +----------------------+----+
                                         |               _\/
                  |                                   +-------------------+
                                         |            |<SchemaObject> SO-B|
                  |                                   +-------------------+
       +---------------------+           |
       |<SessionObject> EO-A1|                                    |
       +---------+-----------+----+      |
                  \               _\/                             |
                   \         +-----------------------+
                    \        |<SessionRelation> ER-X1|            |
                     \       +-----------------------+----+
                      \                                    \      |
                       \   \ +-----------------------+      \
                        +--- |<SessionRelation> ER-X2|       \    |
                             +-----------------------+----+   \
                                                          _\/ _\/ |
                                                      +---------------------+
                                                      |<SessoinObject> EO-B1|
                                                      +---------------------+
      And then, if you copy object:
      ===> EO_A1->copy_object();

       +-------------------+
       |<SchemaObject> SO-A|
       +-------------------+----+
                                _\/
                  |          +----------------------+
                             |<SchemaRelation> SR-X |
                  |          +----------------------+----+
                                         |               _\/
                  |                                   +-------------------+
                                         |            |<SchemaObject> SO-B|
                  |                                   +-------------------+
                                         |                        |
       +---------------------+
       |<SessionObject> EO-A1|           |                        |
       +---------+-----------+----+
                  \               _\/    |                        |
                   \         +-----------------------+
                    \        |<SessionRelation> ER-X1|            |
                     \       +-----------------------+----+
                      \                                    \      |
                       \   \ +-----------------------+      \
                        +--- |<SessionRelation> ER-X2|       \    |
                             +-----------------------+----+   \
                                                          _\/ _\/ |
                                                      +---------------------+
                                                      |<SessoinObject> EO-B1|
                                                      +---------------------+

       +---------------------+
       |<SessionObject> EO-A2|
       +---------+-----------+

      If you like recursive copy, do :
      ===> EO_A1->copy_object(og::core::connection_direction::to);

       +-------------------+
       |<SchemaObject> SO-A|
       +-------------------+----+
                                _\/
                  |          +----------------------+
                             |<SchemaRelation> SR-X |
                  |          +----------------------+----+
                                         |               _\/
                  |                                   +-------------------+
                                         |            |<SchemaObject> SO-B|
                  |                                   +-------------------+
                                         |                        |
       +---------------------+
       |<SessionObject> EO-A1|           |                        |
       +---------+-----------+----+
                  \               _\/    |                        |
                   \         +-----------------------+
                    \        |<SessionRelation> ER-X1|            |
                     \       +-----------------------+----+
                      \                                    \      |
                       \   \ +-----------------------+      \
                        +--- |<SessionRelation> ER-X2|       \    |
                             +-----------------------+----+   \
                                                          _\/ _\/ |
                                                      +---------------------+
                                                      |<SessoinObject> EO-B1|
                                                      +---------------------+
       +---------------------+
       |<SessionObject> EO-A2|
       +---------+-----------+

       +---------------------+
       |<SessionObject> EO-A3|
       +---------+-----------+----+
                  \               _\/
                   \         +-----------------------+
                    \        |<SessionRelation> ER-X3|
                     \       +-----------------------+----+
                      \                                    \
                       \   \ +-----------------------+      \
                        +--- |<SessionRelation> ER-X4|       \
                             +-----------------------+----+   \
                                                          _\/ _\/
                                                      +---------------------+
                                                      |<SessoinObject> EO-B2|
                                                      +---------------------+
```

### Example : CAM system schema
![internal schema pdf](https://github.com/KouOuchi/ObjectGenoise/blob/master/sql/example.png)

### Internal : Tables, primary keys and foreign keys.
![internal schema pdf](https://github.com/KouOuchi/ObjectGenoise/blob/master/sql/OG-Schema.png)
[Internal Schema PDF] and [InternalSchema SQL]

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
### [boost]
- version 1.80
### [SOCI]
- version 4.0.3
- CMake variables in SOCI secsion:
```
SOCI_SQLITE3
```
- CMake variables in WITH secsion:
```
WITH_BOOST
WITH_SQLITE3
```

### [SQLite3]
- version sqlite-amalgamation-3220000
- use CMakefile.txt from https://github.com/snikulov/sqlite.cmake.build
- Preprocessers
```
SQLITE_ENABLE_COLUMN_METADATA
SQLITE_ENABLE_RTREE
SQLITE_ENABLE_FTS4
SQLITE_ENABLE_FTS5
SQLITE_ENABLE_JSON1
```

## Building

*** Step.1 Configure CMake Parameters

If you get headers and libraries about dependencies, you can modify belows with cmake :
```
- SOCI_SOURCE_DIR / SOCI_BUILD_DIR
- SQLITE3_SOURCE_DIR / SQLITE3_BUILD_DIR
- Boost_INCLUDE_DIR
```

*** Step.2 run CMake

*** Step.3 Set Debug or Release in Visuals Studio Configuration

and then, build solution.

*** Step.4 Testing

**** C++ tests
Written with boost unittest framework in test-core project.
Sets the projet to startup and run.

**** C# tests

Written with microsoft unittest framework in test-og-net project.
Sets test architecture to x64. Open test explorer and click Run All.
And then rebuild the project, you can see test items in test explorer.

## Version and History
- 0.89  * use SOCI 4.0.3 and BOOST 1.80.0. Add c++17 option.
- 0.88  * use SOCI 4.0. 
        * Add some pragma after connect to sqllite database.
- 0.87  * Export/Import from top-level session object and its children recursively.
        * Set gzip filter to default
- 0.85  * Fix CMakefile.txt
- 0.84  * Update all CMakefile.txt to improve link boost's libraries.
- 0.83  * Add CMake scripts.
- 0.81  * runtime version is changed from 8.1 to 10.xxx
- 0.80  * boost 1.64 + VS2015 support.
- 0.77  * add global object, schema property.
      * enhancement : catchup_schema checks revision of schema property.
      * catched up SOCI 3_2_3
- 0.6.1 add : schema catchup
      add : copy_object
- 0.4.0 internal release
- 0.0.1 initial revision

## Licenses and Authors
  * Licensed under the [Boost Software License 1.0]
  * Copyright &copy; 2014-2016 Kou Ouchi <kou.ouchi@division-engineering.com>

## Related Information
- [Enterprise Architect] : Data Modeling editor. "CodeEngineering" function can generates DDL of various DataBase Systems.
- [PupSQLite] : Cool and lightweight SQLite3 viewer.
- [Astyle] : A Free, Fast, and Small Automatic Formatter
for C, C++, C++/CLI, Objective?C, C#, and Java Source Code
- [Google C++ Style Guid]

[SOCI]:http://soci.sourceforge.net/
[boost]:http://www.boost.org/
[Enterprise Architect]:http://www.sparxsystems.com/products/ea/index.html
[PupSQLite]:https://www.eonet.ne.jp/~pup/software.html
[Boost Software License 1.0]:http://www.boost.org/LICENSE_1_0.txt
[SQLite3]:https://sqlite.org/amalgamation.html
[Astyle]:http://astyle.sourceforge.net/
[Google C++ Style Guid]:http://google-styleguide.googlecode.com/svn/trunk/cppguide.html
[Internal Schema PDF]:https://github.com/KouOuchi/ObjectGenoise/blob/master/sql/OG-Schema.pdf
[Internal Schema SQL]:https://github.com/KouOuchi/ObjectGenoise/blob/master/sql/OG-Schema.sql
