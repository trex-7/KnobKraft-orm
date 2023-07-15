; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "KnobKraftOrm"
#define MyAppVersion "${KnobKraftOrm_VERSION}"
#define MyAppPublisher "Christof Ruch Beratungs UG (haftungsbeschraenkt)"
#define MyAppURL ""
#define MyAppExeName "KnobKraftOrm.exe"
#define VCRedistFileName "vc_redist.x64.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{63BCEADD-A477-4C18-A4C4-D84CB6101906}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
DisableProgramGroupPage=yes
LicenseFile=${CMAKE_CURRENT_SOURCE_DIR}/redist/agpl-3.0.txt
; Remove the following line to run in administrative install mode (install for all users.)
PrivilegesRequired=lowest
OutputDir=${CMAKE_CURRENT_BINARY_DIR}
OutputBaseFilename=knobkraft_orm_setup_${KnobKraftOrm_VERSION}
Compression=lzma
SolidCompression=yes
WizardStyle=modern
DisableFinishedPage=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "${CMAKE_CURRENT_BINARY_DIR}\RelWithDebInfo\{#MyAppExeName}"; DestDir: "{app}"; 
Source: "${CMAKE_CURRENT_BINARY_DIR}\RelWithDebInfo\{#MyAppName}.pdb"; DestDir: "{app}"; 
Source: "${VCREDIST_PATH}\{#VCRedistFileName}"; DestDir: {tmp}; Flags: dontcopy deleteafterinstall
Source: "${pythonembedded_SOURCE_DIR}\*.*"; DestDir: "{app}"; Flags: ignoreversion
Source: "${CMAKE_CURRENT_BINARY_DIR}\RelWithDebInfo\icuuc67.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "${CMAKE_CURRENT_BINARY_DIR}\RelWithDebInfo\icudt67.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "${CMAKE_CURRENT_BINARY_DIR}\RelWithDebInfo\sentry.dll"; DestDir: "{app}"; Flags: skipifsourcedoesntexist ignoreversion
Source: "${CMAKE_CURRENT_BINARY_DIR}\RelWithDebInfo\crashpad_handler.exe"; DestDir: "{app}"; Flags: skipifsourcedoesntexist ignoreversion
Source: "${CMAKE_CURRENT_BINARY_DIR}\RelWithDebInfo\WinSparkle.dll"; DestDir: "{app}"; Flags: skipifsourcedoesntexist ignoreversion
Source: "${CMAKE_SOURCE_DIR}\adaptions\sequential\*.*"; DestDir: "{app}\sequential";Flags: ignoreversion
Source: "${CMAKE_SOURCE_DIR}\adaptions\roland\*.*"; DestDir: "{app}\roland";Flags: ignoreversion
Source: "${CMAKE_SOURCE_DIR}\adaptions\knobkraft\*.*"; DestDir: "{app}\knobkraft";Flags: ignoreversion
Source: "${CMAKE_SOURCE_DIR}\adaptions\testing\*.*"; DestDir: "{app}\testing;Flags": ignoreversion
#include "adaptations.iss"
; NOTE: Don't use "Flags: ignoreversion" on any shared system files
; VC++ redistributable runtime. Extracted by VC2017RedistNeedsInstall(), if needed.

[UninstallDelete]
Type: files; Name: "{app}\sequential\__pycache__\*.pyc"
Type: dirifempty; Name: "{app}\sequential\__pycache__"
Type: dirifempty; Name: "{app}\sequential"
Type: files; Name: "{app}\roland\__pycache__\*.pyc"
Type: dirifempty; Name: "{app}\roland\__pycache__"
Type: dirifempty; Name: "{app}\roland"
Type: files; Name: "{app}\knobkraft\__pycache__\*.pyc"
Type: dirifempty; Name: "{app}\knobkraft\__pycache__"
Type: dirifempty; Name: "{app}\knobkraft"

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall
Filename: "{tmp}\{#VCRedistFileName}"; StatusMsg: "Installing required VS2017 C++ Runtime Libraries"; Parameters: "/quiet"; Check: VC2017RedistNeedsInstall ; Flags: waituntilterminated

[Code]
function VC2017RedistNeedsInstall: Boolean;
var 
  Version: String;
begin
  if (RegQueryStringValue(HKEY_LOCAL_MACHINE, 'SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x64', 'Version', Version)) then
  begin
    // Is the installed version at least the same I used for development?
    Log('VC Redist Version check : found ' + Version);
    Result := (CompareStr(Version, 'v14.16.27012.1')<0);
    if (not Result) then
    begin
      Log('Skipping installation of new VC Redist Version');
    end
  end
  else 
  begin
    // Not even an old version installed
    Result := True;
  end;
  if (Result) then
  begin
    ExtractTemporaryFile('{#VCRedistFileName}');
  end;
end;
