C++ 2D Simple Game Server
- PacketGenerator was Made C#

Use External Library 
Client : SDL2
Server : Protobuf

DB : MSSQL

아래 영상 실행 로직
1. Client -> Login Server ( Auth )
2. Success -> Login Server ( Token )-> Client
3. Client -> Game Server ( Token Check )
4. Succes -> Game Start
5. Players obtain items by killing monsters ( DB Saved ) 
6. Players can equip items and change stats ( DB Saved )
7. Another Client Connect
8. Players Move Sync Check
9. DummyClient Connected and Move Sync Check
- monsters pathfinding : astar algorithm

Debug 실행 영상
https://www.youtube.com/watch?v=QGb2nIVwZpw
