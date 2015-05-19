@ECHO OFF

CD /D %~dp0

SET __TARGET__=%~dp0

echo Setting Env OG_HOME "%__TARGET__:~0,-1%"

SETX OG_HOME "%__TARGET__:~0,-1%"

