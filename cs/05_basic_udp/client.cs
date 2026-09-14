using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;

class Client
{
    static void Main()
    {
        // analogie socket(AF_INET, SOCK_DGRAM, 0) z C
        Socket client = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
        // v UDP musime explicitne specifikovat adresu serveru pri kazdem odeslani a prijmu
        IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 10000);

        byte[] buffer = new byte[1];
        for (char inputChar = 'A'; inputChar <= 'C'; inputChar++)
        {
            buffer[0] = (byte)inputChar;
            // analogie sendto() z C
            client.SendTo(buffer, serverEndPoint);

            EndPoint remoteEndPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 10000);

            // analogie recvfrom() z C
            int received = client.ReceiveFrom(buffer, ref remoteEndPoint); // remoteEndPoint bude obsahovat adresu odesilatele po prijmu
            if (received > 0)
            {
                char outputChar = (char)buffer[0];
                Console.WriteLine($"Received: {outputChar}");
            }

            Thread.Sleep(1000);
        }

        client.Close();
    }
}
