namespace PacketGenerator
{
    internal class Program
    {
        static string packetEnums;

        //  ClientPacketHandler -> GameServer
        //  C_PACKET
        static string clientPacketFunction;   
        static string clientPacketReigster;
        //  S_PACKET
        static string clientMakeSendBuffer;

        //  ClientPacketHandler -> loginServer
        //  L_C_Packet
        static string loginClientPacketFunction;
        static string loginClientPacketReigster;
        //  L_S_Packet
        static string loginClientMakeSendBuffer;

        //  ServerPacketHandler -> GameClient
        //  S_PACKET
        static string serverPacketFunction;
        static string serverPacketReigster;
        //  C_PACKET
        static string serverMakeSendBuffer;

        static string serverPacketHandler;
        static string clientPacketHandler;
        static string loginClientPacketHandler;

        static void Main(string[] args)
        {
            List<String> totalPackets = new List<String>();   
            List<String> gamePackets = new List<String>();   
            List<String> loginPackets = new List<String>();   

            List<String> S_Packet = new List<String>();  
            List<String> C_Packet = new List<String>();  
            List<String> L_S_Packet = new List<String>();  
            List<String> L_C_Packet = new List<String>();  
           
            string protoPath = "./Protocol.proto";
            string[] text = File.ReadAllLines(protoPath);

            if (text.Length > 0)
            {
                for(int i = 0; i < text.Length; i++) 
                {
                    if (text[i].Contains("message") == false)
                        continue;
                    else
                    {
                        string[] temp = text[i].Split('\x020');
                        string packetId = temp[1].ToUpper();
                        if (packetId.FirstOrDefault() == 'L')
                        {
                            if (packetId[2] == 'C')
                                L_C_Packet.Add(packetId);
                            else
                                L_S_Packet.Add(packetId);

                            loginPackets.Add(packetId);
                        }
                        else
                        {
                            if (packetId.FirstOrDefault() == 'C')
                                C_Packet.Add(packetId);
                            else
                                S_Packet.Add(packetId);

                            gamePackets.Add(packetId);
                        }
                    }
                }
            }

            ParsePacketEnums(loginPackets, gamePackets);
            ParseClientPacketHandler(S_Packet, C_Packet);
            ParseLoginClientPacketHandler(L_S_Packet, L_C_Packet);
            ParseServerPacketHandler(S_Packet, C_Packet, L_S_Packet, L_C_Packet);

            serverPacketHandler = string.Format(PacketFormat.fileFormat, packetEnums, serverPacketFunction, "ServerPacketHandler", serverPacketReigster, serverMakeSendBuffer);
            clientPacketHandler = string.Format(PacketFormat.fileFormat, packetEnums, clientPacketFunction, "ClientPacketHandler", clientPacketReigster, clientMakeSendBuffer);
            loginClientPacketHandler = string.Format(PacketFormat.fileFormat, packetEnums, loginClientPacketFunction, "ClientPacketHandler", loginClientPacketReigster, loginClientMakeSendBuffer);

            File.WriteAllText("ServerPacketHandler.h", serverPacketHandler);
            File.WriteAllText("ClientPacketHandler.h", clientPacketHandler);
            File.WriteAllText("LoginClientPacketHandler.h", loginClientPacketHandler);
        }

        public static void ParsePacketEnums(List<String> loginPackets, List<String> gamePackets)
        {
            int loginPacketStartNumber = 100;
            for (int i = 0; i < loginPackets.Count; i++)
            {
                packetEnums += '\t';
                packetEnums += string.Format(PacketFormat.packetEnumFormat, loginPackets[i], loginPacketStartNumber++);
                packetEnums += Environment.NewLine;
            }

            int gamePacketStartNumber = 200;
            for (int i = 0; i < gamePackets.Count; i++)
            {
                packetEnums += '\t';
                packetEnums += string.Format(PacketFormat.packetEnumFormat, gamePackets[i], gamePacketStartNumber++);
                packetEnums += Environment.NewLine;
            }
        }

        //  Game Server Generate
        public static void ParseClientPacketHandler(List<String> S_Packet, List<String> C_Packet)
        {
            //  C_PACKET
            foreach(String packet in C_Packet)
            {
                clientPacketFunction += string.Format(PacketFormat.packetProcessingFunctionFormat, packet, packet);
                clientPacketFunction += Environment.NewLine;

                clientPacketReigster += "\t\t";
                clientPacketReigster += string.Format(PacketFormat.packetProcessingFunctionRegisterFormat, packet, packet, packet);
                clientPacketReigster += Environment.NewLine;
            }

            //  S_PACKET
            foreach (String packet in S_Packet)
            {
                clientMakeSendBuffer += '\t';
                clientMakeSendBuffer += string.Format(PacketFormat.makePacketToSendBufferFormat, packet, packet);
                clientMakeSendBuffer += Environment.NewLine;
            }
        }

        //  Login Server Generate
        public static void ParseLoginClientPacketHandler(List<String> L_S_Packet, List<String> L_C_Packet)
        {
            //  C_PACKET
            foreach (String packet in L_C_Packet)
            {
                loginClientPacketFunction += string.Format(PacketFormat.packetProcessingFunctionFormat, packet, packet);
                loginClientPacketFunction += Environment.NewLine;

                loginClientPacketReigster += "\t\t";
                loginClientPacketReigster += string.Format(PacketFormat.packetProcessingFunctionRegisterFormat, packet, packet, packet);
                loginClientPacketReigster += Environment.NewLine;
            }

            //  S_PACKET
            foreach (String packet in L_S_Packet)
            {
                loginClientMakeSendBuffer += '\t';
                loginClientMakeSendBuffer += string.Format(PacketFormat.makePacketToSendBufferFormat, packet, packet);
                loginClientMakeSendBuffer += Environment.NewLine;
            }
        }

        //  Clinet Generate
        public static void ParseServerPacketHandler(List<String> S_Packet, List<String> C_Packet, List<String> L_S_Packet, List<String> L_C_Packet)
        {
            //  S_PACKET
            foreach (String packet in S_Packet)
            {
                serverPacketFunction += string.Format(PacketFormat.packetProcessingFunctionFormat, packet, packet);
                serverPacketFunction += Environment.NewLine;

                serverPacketReigster += "\t\t";
                serverPacketReigster += string.Format(PacketFormat.packetProcessingFunctionRegisterFormat, packet, packet, packet);
                serverPacketReigster += Environment.NewLine;
            }

            //  S_PACKET
            foreach (String packet in L_S_Packet)
            {
                serverPacketFunction += string.Format(PacketFormat.packetProcessingFunctionFormat, packet, packet);
                serverPacketFunction += Environment.NewLine;

                serverPacketReigster += "\t\t";
                serverPacketReigster += string.Format(PacketFormat.packetProcessingFunctionRegisterFormat, packet, packet, packet);
                serverPacketReigster += Environment.NewLine;
            }

            //  C_PACKET
            foreach (String packet in C_Packet)
            {
                serverMakeSendBuffer += '\t';
                serverMakeSendBuffer += string.Format(PacketFormat.makePacketToSendBufferFormat, packet, packet);
                serverMakeSendBuffer += Environment.NewLine;
            }

            //  C_PACKET
            foreach (String packet in L_C_Packet)
            {
                serverMakeSendBuffer += '\t';
                serverMakeSendBuffer += string.Format(PacketFormat.makePacketToSendBufferFormat, packet, packet);
                serverMakeSendBuffer += Environment.NewLine;
            }
        }
    }
}