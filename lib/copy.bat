cd /d %~dp0

mkdir debug
mkdir release

xcopy.exe /Y "%BOOST%"\lib64-msvc-12.0\boost_chrono-vc120-mt-gd-1_60.dll debug
xcopy.exe /Y "%BOOST%"\lib64-msvc-12.0\boost_date_time-vc120-mt-gd-1_60.dll debug
xcopy.exe /Y "%BOOST%"\lib64-msvc-12.0\boost_filesystem-vc120-mt-gd-1_60.dll debug
xcopy.exe /Y "%BOOST%"\lib64-msvc-12.0\boost_iostreams-vc120-mt-gd-1_60.dll debug
xcopy.exe /Y "%BOOST%"\lib64-msvc-12.0\boost_locale-vc120-mt-gd-1_60.dll debug
xcopy.exe /Y "%BOOST%"\lib64-msvc-12.0\boost_log_setup-vc120-mt-gd-1_60.dll debug
xcopy.exe /Y "%BOOST%"\lib64-msvc-12.0\boost_log-vc120-mt-gd-1_60.dll debug
xcopy.exe /Y "%BOOST%"\lib64-msvc-12.0\boost_regex-vc120-mt-gd-1_60.dll debug
xcopy.exe /Y "%BOOST%"\lib64-msvc-12.0\boost_system-vc120-mt-gd-1_60.dll debug
xcopy.exe /Y "%BOOST%"\lib64-msvc-12.0\boost_thread-vc120-mt-gd-1_60.dll debug
xcopy.exe /Y "%BOOST%"\lib64-msvc-12.0\boost_timer-vc120-mt-gd-1_60.dll debug
xcopy.exe /Y "%BOOST%"\lib64-msvc-12.0\boost_unit_test_framework-vc120-mt-gd-1_60.dll debug
xcopy.exe /Y "%BOOST%"\lib64-msvc-12.0\boost_zlib-vc120-mt-gd-1_60.dll debug
xcopy.exe /Y "%SOCI%"\debug\bin64\*.* debug

xcopy.exe /Y "%BOOST%"\lib64-msvc-12.0\boost_chrono-vc120-mt-1_60.dll release
xcopy.exe /Y "%BOOST%"\lib64-msvc-12.0\boost_date_time-vc120-mt-1_60.dll release
xcopy.exe /Y "%BOOST%"\lib64-msvc-12.0\boost_filesystem-vc120-mt-1_60.dll release
xcopy.exe /Y "%BOOST%"\lib64-msvc-12.0\boost_iostreams-vc120-mt-1_60.dll release
xcopy.exe /Y "%BOOST%"\lib64-msvc-12.0\boost_locale-vc120-mt-1_60.dll release
xcopy.exe /Y "%BOOST%"\lib64-msvc-12.0\boost_log_setup-vc120-mt-1_60.dll release
xcopy.exe /Y "%BOOST%"\lib64-msvc-12.0\boost_log-vc120-mt-1_60.dll release
xcopy.exe /Y "%BOOST%"\lib64-msvc-12.0\boost_regex-vc120-mt-1_60.dll release
xcopy.exe /Y "%BOOST%"\lib64-msvc-12.0\boost_system-vc120-mt-1_60.dll release
xcopy.exe /Y "%BOOST%"\lib64-msvc-12.0\boost_thread-vc120-mt-1_60.dll release
xcopy.exe /Y "%BOOST%"\lib64-msvc-12.0\boost_timer-vc120-mt-1_60.dll release
xcopy.exe /Y "%BOOST%"\lib64-msvc-12.0\boost_unit_test_framework-vc120-mt-1_60.dll release
xcopy.exe /Y "%BOOST%"\lib64-msvc-12.0\boost_zlib-vc120-mt-1_60.dll release
xcopy.exe /Y "%SOCI%"\release\bin64\*.* release
