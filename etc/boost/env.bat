@ECHO OFF

CD /D %~dp0

SET __TARGET__=%~dp0

echo Setting Env BOOST "%__TARGET__:~0,-1%"

SETX BOOST "%__TARGET__:~0,-1%"

