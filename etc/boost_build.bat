REM ### avoid crash while c++/clr using boost_log ###
b2 --clean-all
b2 --toolset=msvc-12.0 -s ZLIB_SOURCE=C:\Apps\lib\zlib-1.2.8 --stagedir=lib64-msvc-12.0 address-model=64 -j 4 define=BOOST_LOG_USE_CHAR define=BOOST_LOG_USE_WINNT5_API --build-type=complete threading=single link=shared stage
b2 --clean-all
b2 --toolset=msvc-12.0 -s ZLIB_SOURCE=C:\Apps\lib\zlib-1.2.8 --stagedir=lib64-msvc-12.0 address-model=64 -j 4 define=BOOST_LOG_USE_CHAR define=BOOST_LOG_USE_WINNT5_API --build-type=complete threading=multi link=shared stage
b2 --clean-all
b2 --clean-all
b2 --toolset=msvc-12.0 -s ZLIB_SOURCE=C:\Apps\lib\zlib-1.2.8 --stagedir=lib64-msvc-12.0 address-model=64 -j 4 define=BOOST_LOG_USE_CHAR define=BOOST_LOG_USE_WINNT5_API --build-type=complete threading=single link=static stage
b2 --clean-all
b2 --toolset=msvc-12.0 -s ZLIB_SOURCE=C:\Apps\lib\zlib-1.2.8 --stagedir=lib64-msvc-12.0 address-model=64 -j 4 define=BOOST_LOG_USE_CHAR define=BOOST_LOG_USE_WINNT5_API --build-type=complete threading=multi link=static stage
