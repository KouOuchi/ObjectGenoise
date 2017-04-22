REM ### avoid crash while c++/clr using boost_log ###
b2 toolset=msvc -s ZLIB_SOURCE="C:\Apps\lib\zlib-1.2.8" address-model=64 -j 4 define=BOOST_LOG_USE_CHAR define=BOOST_LOG_USE_WINNT5_API define=_HAS_ITERATOR_DEBUGGING=0 define=_ITERATOR_DEBUG_LEVEL=0  define=BOOST_LOG_USE_WINNT5_API threading=single,multi link=static,shared --build-type=complete stage
