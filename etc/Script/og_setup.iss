#define MyAppSetupName 'ObjectGenoise'
#define MyAppVersion '0.85' 

[Setup]
AppId=ObjectGenoise
AppName={#MyAppSetupName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppSetupName} {#MyAppVersion}
AppCopyright=Copyright ｩ Division Engineering 2017
VersionInfoCompany=Division Engineering
AppPublisher=Division Engineering
;AppPublisherURL=http://...
;AppSupportURL=http://...
;AppUpdatesURL=http://...
OutputBaseFilename={#MyAppSetupName}-{#MyAppVersion}-lib64-msvc-14.0
DefaultGroupName=Lib
DefaultDirName={pf}\ObjectGenoise
;UninstallDisplayIcon={app}\MyProgram.exe
OutputDir=..\SDKInstaller
SourceDir=..\..\build\SDK
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

Source: "*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs ; Components: UpdateEnv

;Source: "C:\mnt\git2\ObjectGenoise\util\SchemaUpdater\bin\Release"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: UpdateEnv
;Source: "setup-splash.bmp"; DestName: "setup-splash.bmp"; Flags: dontcopy nocompression

[Icons]

[Registry]

[Components]
Name: "UpdateEnv"; Description: "Create or Update OG_HOME environment value"; Types: custom full

[Run]
Filename: "{app}\env.bat"; Components: UpdateEnv

[CustomMessages]

[Code]
function GetNumber(var temp: String): Integer;
var
  part: String;
  pos1: Integer;
begin
  if Length(temp) = 0 then
  begin
    Result := -1;
    Exit;
  end;
    pos1 := Pos('.', temp);
    if (pos1 = 0) then
    begin
      Result := StrToInt(temp);
    temp := '';
    end
    else
    begin
    part := Copy(temp, 1, pos1 - 1);
      temp := Copy(temp, pos1 + 1, Length(temp));
      Result := StrToInt(part);
    end;
end;

function CompareInner(var temp1, temp2: String): Integer;
var
  num1, num2: Integer;
begin
    num1 := GetNumber(temp1);
  num2 := GetNumber(temp2);
  if (num1 = -1) or (num2 = -1) then
  begin
    Result := 0;
    Exit;
  end;
      if (num1 > num2) then
      begin
        Result := 1;
      end
      else if (num1 < num2) then
      begin
        Result := -1;
      end
      else
      begin
        Result := CompareInner(temp1, temp2);
      end;
end;

function CompareVersion(str1, str2: String): Integer;
var
  temp1, temp2: String;
begin
    temp1 := str1;
    temp2 := str2;
    Result := CompareInner(temp1, temp2);
end;

function InitializeSetup(): Boolean;
var
  oldVersion: String;
  uninstaller: String;
  ErrorCode: Integer;
  vCurID      :String;
  vCurAppName :String;
begin
  vCurID:= '{#SetupSetting("AppId")}';
  vCurAppName:= '{#SetupSetting("AppName")}';
  //remove first "{" of ID
  //vCurID:= Copy(vCurID, 2, Length(vCurID) - 1);
  //
  if RegKeyExists(HKEY_LOCAL_MACHINE,
    'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\' + vCurID + '_is1') then
  begin
    RegQueryStringValue(HKEY_LOCAL_MACHINE,
      'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\' + vCurID + '_is1',
      'DisplayVersion', oldVersion);
    if (CompareVersion(oldVersion, '{#SetupSetting("AppVersion")}') < 0) then      
    begin
      if MsgBox('Version ' + oldVersion + ' of ' + vCurAppName + ' is already installed. Are you sure that uninstall old version?',
        mbConfirmation, MB_YESNO) = IDNO then
      begin
        Result := False;
      end
      else
      begin
          RegQueryStringValue(HKEY_LOCAL_MACHINE,
            'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\' + vCurID + '_is1',
            'UninstallString', uninstaller);
          ShellExec('runas', uninstaller, '/SILENT', '', SW_HIDE, ewWaitUntilTerminated, ErrorCode);
          Result := True;
      end;
    end
    else
    begin
      MsgBox('Version ' + oldVersion + ' of ' + vCurAppName + ' is already installed. This installer will exit.',
        mbInformation, MB_OK);
      Result := False;
    end;
  end
  else
  begin
    Result := True;
  end;
end;



