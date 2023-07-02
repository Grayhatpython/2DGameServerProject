pushd %~dp0
protoc.exe -I=./ --cpp_out=./ ./Enum.proto
protoc.exe -I=./ --cpp_out=./ ./Struct.proto
protoc.exe -I=./ --cpp_out=./ ./Protocol.proto
IF ERRORLEVEL 1 PAUSE

PacketGenerator.exe

XCOPY /Y Enum.pb.h "../../../GameServer"
XCOPY /Y Enum.pb.cc "../../../GameServer"
XCOPY /Y Struct.pb.h "../../../GameServer"
XCOPY /Y Struct.pb.cc "../../../GameServer"
XCOPY /Y Protocol.pb.h "../../../GameServer"
XCOPY /Y Protocol.pb.cc "../../../GameServer"
XCOPY /Y ClientPacketHandler.h "../../../GameServer"

XCOPY /Y Enum.pb.h "../../../LoginServer"
XCOPY /Y Enum.pb.cc "../../../LoginServer"
XCOPY /Y Struct.pb.h "../../../LoginServer"
XCOPY /Y Struct.pb.cc "../../../LoginServer"
XCOPY /Y Protocol.pb.h "../../../LoginServer"
XCOPY /Y Protocol.pb.cc "../../../LoginServer"
MOVE LoginClientPacketHandler.h ../../../LoginServer/ClientPacketHandler.h

XCOPY /Y Enum.pb.h "../../../GameClient"
XCOPY /Y Enum.pb.cc "../../../GameClient"
XCOPY /Y Struct.pb.h "../../../GameClient"
XCOPY /Y Struct.pb.cc "../../../GameClient"
XCOPY /Y Protocol.pb.h "../../../GameClient"
XCOPY /Y Protocol.pb.cc "../../../GameClient"
XCOPY /Y ServerPacketHandler.h "../../../GameClient"

XCOPY /Y Enum.pb.h "../../../DummyClient"
XCOPY /Y Enum.pb.cc "../../../DummyClient"
XCOPY /Y Struct.pb.h "../../../DummyClient"
XCOPY /Y Struct.pb.cc "../../../DummyClient"
XCOPY /Y Protocol.pb.h "../../../DummyClient"
XCOPY /Y Protocol.pb.cc "../../../DummyClient"
XCOPY /Y ServerPacketHandler.h "../../../DummyClient"

DEL /Q /F *.pb.h
DEL /Q /F *.pb.cc
DEL /Q /F *.h

PAUSE