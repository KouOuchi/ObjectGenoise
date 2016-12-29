cd /d %~dp0

mkdir debug
mkdir release

xcopy.exe /Y "%BOOST%"\stage\lib\boost_chrono-vc140-mt-gd-1_63.dll debug
xcopy.exe /Y "%BOOST%"\stage\lib\boost_date_time-vc140-mt-gd-1_63.dll debug
xcopy.exe /Y "%BOOST%"\stage\lib\boost_filesystem-vc140-mt-gd-1_63.dll debug
xcopy.exe /Y "%BOOST%"\stage\lib\boost_iostreams-vc140-mt-gd-1_63.dll debug
xcopy.exe /Y "%BOOST%"\stage\lib\boost_locale-vc140-mt-gd-1_63.dll debug
xcopy.exe /Y "%BOOST%"\stage\lib\boost_log_setup-vc140-mt-gd-1_63.dll debug
xcopy.exe /Y "%BOOST%"\stage\lib\boost_log-vc140-mt-gd-1_63.dll debug
xcopy.exe /Y "%BOOST%"\stage\lib\boost_regex-vc140-mt-gd-1_63.dll debug
xcopy.exe /Y "%BOOST%"\stage\lib\boost_system-vc140-mt-gd-1_63.dll debug
xcopy.exe /Y "%BOOST%"\stage\lib\boost_thread-vc140-mt-gd-1_63.dll debug
xcopy.exe /Y "%BOOST%"\stage\lib\boost_timer-vc140-mt-gd-1_63.dll debug
xcopy.exe /Y "%BOOST%"\stage\lib\boost_unit_test_framework-vc140-mt-gd-1_63.dll debug
xcopy.exe /Y "%BOOST%"\stage\lib\boost_zlib-vc140-mt-gd-1_63.dll debug

xcopy.exe /Y "%BOOST%"\stage\lib\boost_chrono-vc140-mt-1_63.dll release
xcopy.exe /Y "%BOOST%"\stage\lib\boost_date_time-vc140-mt-1_63.dll release
xcopy.exe /Y "%BOOST%"\stage\lib\boost_filesystem-vc140-mt-1_63.dll release
xcopy.exe /Y "%BOOST%"\stage\lib\boost_iostreams-vc140-mt-1_63.dll release
xcopy.exe /Y "%BOOST%"\stage\lib\boost_locale-vc140-mt-1_63.dll release
xcopy.exe /Y "%BOOST%"\stage\lib\boost_log_setup-vc140-mt-1_63.dll release
xcopy.exe /Y "%BOOST%"\stage\lib\boost_log-vc140-mt-1_63.dll release
xcopy.exe /Y "%BOOST%"\stage\lib\boost_regex-vc140-mt-1_63.dll release
xcopy.exe /Y "%BOOST%"\stage\lib\boost_system-vc140-mt-1_63.dll release
xcopy.exe /Y "%BOOST%"\stage\lib\boost_thread-vc140-mt-1_63.dll release
xcopy.exe /Y "%BOOST%"\stage\lib\boost_timer-vc140-mt-1_63.dll release
xcopy.exe /Y "%BOOST%"\stage\lib\boost_unit_test_framework-vc140-mt-1_63.dll release
xcopy.exe /Y "%BOOST%"\stage\lib\boost_zlib-vc140-mt-1_63.dll release
