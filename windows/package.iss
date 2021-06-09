#define MyAppName "writernote"
#define MyAppVersion "_"
#define MyAppPublisher "Writernote"
#define MyAppURL "https://github.com/giacomogroppi/writernote-qt"
#define MyAppExeName "writernote.exe"
#define MyAppAssocName "Writernote"
#define MyAppAssocExt ".writer"
#define MyAppAssocKey StringChange(MyAppAssocName, " ", "") + MyAppAssocExt
#define IconPath "..\images\icon-writernote.ico"

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{36213220-64DC-43B3-9019-B5B18DE05E89}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
ChangesAssociations=yes
DisableProgramGroupPage=yes

LicenseFile=..\LICENSE

SetupIconFile={#IconPath}

; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest
OutputBaseFilename=writernote_setup
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

; [Tasks]
; Name: desktopicon; Description: "{cm:CreateDesktopIcon}"; Flags: checkedonce

[Files]
Source: "..\build\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs                        
Source: "..\updater\build\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\updater\build\updater.exe"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs      
Source: "{#IconPath}"; DestDir: "{app}"
Source: "C:\msys64\mingw64\lib\libssl.dll.a"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Registry]
Root: HKCR; Subkey: "{#MyAppAssocExt}"; ValueType: string; ValueName: ""; ValueData: "Writernote file"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "{#MyAppName}"; ValueType: string; ValueName: ""; ValueData: "My Program File"; Flags: uninsdeletekey
Root: HKCR; Subkey: "{#MyAppName}\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\writernote.exe"
Root: HKCR; Subkey: "{#MyAppName}\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\writernote.exe"" ""%1"""

[Icons]
Name: "{userdesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; 
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; 
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; IconFilename: "{src}\{#IconPath}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\writernote.exe"; WorkingDir: "{app}"

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

