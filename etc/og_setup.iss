#define MyAppSetupName 'ObjectGenoise'
#define MyAppVersion '0.65' 

[Setup]
AppName={#MyAppSetupName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppSetupName} {#MyAppVersion}
AppCopyright=Copyright ｩ Division Engineering 2015
VersionInfoCompany=Division Engineering
AppPublisher=Division Engineering
;AppPublisherURL=http://...
;AppSupportURL=http://...
;AppUpdatesURL=http://...
OutputBaseFilename={#MyAppSetupName}-{#MyAppVersion}
DefaultGroupName=Lib
DefaultDirName={pf}\ObjectGenoise
;UninstallDisplayIcon={app}\MyProgram.exe
;OutputDir=
SourceDir=.
AllowNoIcons=yes
;SetupIconFile=MyProgramIcon
SolidCompression=yes
Compression=lzma2/normal

;MinVersion default value: "0,5.0 (Windows 2000+) if Unicode Inno Setup, else 4.0,4.0 (Windows 95+)"
;MinVersion=0,5.0
PrivilegesRequired=admin
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
;InfoBeforeFile=Eula.rtf
DirExistsWarning=no

[Languages]
;Name: "en"; MessagesFile: "compiler:Default.isl"
;Name: "de"; MessagesFile: "compiler:Languages\German.isl"
;Name: "ja"; MessagesFile: "compiler:Languages\Japanese.isl"

[Tasks]
;Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"
;Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
;Source: "src\MyProgram-x64.exe"; DestDir: "{app}"; DestName: "MyProgram.exe"; Check: IsX64
;Source: "src\MyProgram-IA64.exe"; DestDir: "{app}"; DestName: "MyProgram.exe"; Check: IsIA64
;Source: "src\MyProgram.exe"; DestDir: "{app}"; Check: not Is64BitInstallMode

;Source: "C:\mnt\lib2\Boost1.57_Ogre1.9.0 DevKit\boost_1_57_0\*"; DestDir: "{app}\boost_1_57_0\"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "C:\mnt\git2\ObjectGenoise\core\include\*"; DestDir: "{app}\include"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: UpdateEnv
Source: "C:\mnt\git2\ObjectGenoise\test-og-net\bin\x64\Debug\*.dll"; DestDir: "{app}\debug\bin"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: UpdateEnv
Source: "C:\mnt\git2\ObjectGenoise\test-og-net\bin\x64\Debug\*.pdb"; DestDir: "{app}\debug\bin"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: UpdateEnv
;Source: "C:\Program Files\SOCI\debug\bin\*.dll"; DestDir: "{app}\debug\bin"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: UpdateEnv
Source: "C:\mnt\git2\ObjectGenoise\x64\Debug\*.lib"; DestDir: "{app}\debug\lib64"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: UpdateEnv
;Source: "C:\Program Files\SOCI\debug\lib64\*"; DestDir: "{app}\debug\lib64"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: UpdateEnv

Source: "C:\mnt\git2\ObjectGenoise\test-og-net\bin\x64\Release\*.dll"; DestDir: "{app}\release\bin"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: UpdateEnv
Source: "C:\mnt\git2\ObjectGenoise\test-og-net\bin\x64\Release\*.pdb"; DestDir: "{app}\release\bin"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: UpdateEnv
;Source: "C:\Program Files\SOCI\release\bin\*.dll"; DestDir: "{app}\release\bin"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: UpdateEnv
Source: "C:\mnt\git2\ObjectGenoise\x64\Release\*.lib"; DestDir: "{app}\release\lib64"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: UpdateEnv
;Source: "C:\Program Files\SOCI\release\lib64\*"; DestDir: "{app}\release\lib64"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: UpdateEnv



Source: "C:\mnt\git2\ObjectGenoise\etc\env.bat"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: UpdateEnv

;Source: "setup-splash.bmp"; DestName: "setup-splash.bmp"; Flags: dontcopy nocompression

[Icons]

[Registry]

[Components]
Name: "UpdateEnv"; Description: "Create or Update OG_HOME environment value"; Types: custom full

[Run]
Filename: "{app}\env.bat"; Components: UpdateEnv

[CustomMessages]



