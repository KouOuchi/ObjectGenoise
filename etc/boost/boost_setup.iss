#define MyAppSetupName 'Boost1.60.0'
#define MyAppVersion '0.1' 

[Setup]
AppName="boost_1_60_0"
AppVersion={#MyAppVersion}
AppVerName={#MyAppSetupName} {#MyAppVersion}
AppCopyright=Copyright ｩ Division Engineering 2015
VersionInfoCompany=Division Engineering
AppPublisher=Division Engineering
;AppPublisherURL=http://...
;AppSupportURL=http://...
;AppUpdatesURL=http://...
OutputBaseFilename={#MyAppSetupName}-{#MyAppVersion}
;DefaultGroupName="Lib"
DefaultDirName={pf}\boost_1_60_0
;UninstallDisplayIcon={app}\MyProgram.exe
;OutputDir=bin
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

Source: "C:\mnt\lib2\Boost1.57_Ogre1.9.0 DevKit2\boost_1_60_0\*"; DestDir: "{app}\"; Flags: ignoreversion recursesubdirs createallsubdirs; 
;Source: "C:\mnt\lib2\Boost1.57_Ogre1.9.0 DevKit\Ogre-v1-9-0\*"; DestDir: "{app}\Ogre-v1-9-0\"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: Ogre

;Source: "setup-splash.bmp"; DestName: "setup-splash.bmp"; Flags: dontcopy nocompression

[Icons]

[Registry]

[Components]
Name: "UpdateEnv"; Description: "Create or Update BOOST environment value"; Types: custom full

[Run]
Filename: "{app}\link.bat"
Filename: "{app}\env.bat"; Components: UpdateEnv

[CustomMessages]



