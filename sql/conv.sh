#!/bin/sh

echo
echo '-=< Convert Schema to Header(schema_sql.h) >='
echo
echo
echo dos2unix...
dos2unix OG-Schema.sql

echo
echo format sql...
sed -e 's/.*/"&"/g' OG-Schema.sql | sed -e 's/REAL/FLOAT/g' > _tmp
echo -n ";" >> _tmp
echo "" >> _tmp
echo "}" >> _tmp
echo "}" >> _tmp

echo 
echo create header...
echo "#pragma once" > _tmp2
echo "#include <string>" >> _tmp2
echo "namespace og {" >> _tmp2
echo "namespace core {" >> _tmp2
echo "std::string schema_sql =" >> _tmp2

echo 
echo merge header...
cat _tmp2 _tmp > schema_sql.h

echo
echo copy header to include directory...
cp schema_sql.h ../core/include/og

echo
echo clean up...
rm schema_sql.h _tmp _tmp2

echo
echo done.
echo

