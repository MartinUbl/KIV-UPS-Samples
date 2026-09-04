using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;

class Client
{
    static void Main()
    {
        Socket client = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
        IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Loopback, 10000);

        byte[] buffer = new byte[1];
        for (char inputChar = 'A'; inputChar <= 'C'; inputChar++)
        {
            buffer[0] = (byte)inputChar;
            client.SendTo(buffer, serverEndPoint);

            EndPoint remoteEndPoint = new IPEndPoint(IPAddress.Loopback, 10000);
            int received = client.ReceiveFrom(buffer, ref remoteEndPoint);
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
