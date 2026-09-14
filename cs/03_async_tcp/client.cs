using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;

class Client
{
    static void Main()
    {
        Socket client = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        client.Connect(IPAddress.Parse("127.0.0.1"), 10000);
        Console.WriteLine("Connected to server on localhost:10000");

        using (NetworkStream stream = new NetworkStream(client))
        {
            for (char inputChar = 'A'; inputChar <= 'C'; inputChar++)
            {
                stream.WriteByte((byte)inputChar);

                int response = stream.ReadByte();
                if (response != -1)
                {
                    char outputChar = (char)response;
                    Console.WriteLine($"Received: {outputChar}");
                }

                Thread.Sleep(1000);
            }
        }

        client.Close();
    }
}
