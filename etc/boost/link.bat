@ECHO OFF

CD /D %~dp0

IF NOT EXIST stage (
	MKDIR stage
)

echo mkdir
CD stage

echo create link
MKLINK /D lib "..\lib64-msvc-12.0"
