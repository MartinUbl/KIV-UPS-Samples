using System;
using System.Net;
using System.Net.Sockets;

class Server
{
    static void Main()
    {
        // analogie socket(AF_INET, SOCK_DGRAM, 0) z C
        Socket listener = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
        // analogie bind() z C
        listener.Bind(new IPEndPoint(IPAddress.Parse("127.0.0.1"), 10000));
        Console.WriteLine("Server listening on localhost:10000");

        while (true)
        {
            byte[] buffer = new byte[1];
            EndPoint remoteEndPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 10000);
            while (true)
            {
                // analogie recvfrom() z C
                int received = listener.ReceiveFrom(buffer, ref remoteEndPoint);
                if (received > 0)
                {
                    char c = (char)buffer[0];

                    Console.WriteLine($"Received: {c}");

                    char result = c;

                    if (c >= 'A' && c <= 'Z')
                        result = (char)('Z' - (c - 'A'));

                    buffer[0] = (byte)result;
                    // analogie sendto() z C
                    listener.SendTo(buffer, remoteEndPoint);
                }
            }
        }
    }
}
