pushd %~dp0
XCOPY /Y .\bin\Debug\PacketGenerator.exe ..\..\Common\Protocol\bin
XCOPY /Y .\bin\Debug\PacketGenerator.dll ..\..\Common\Protocol\bin
XCOPY /Y .\bin\Debug\PacketGenerator.deps.json ..\..\Common\Protocol\bin
XCOPY /Y .\bin\Debug\PacketGenerator.runtimeconfig.json ..\..\Common\Protocol\bin