using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;

class Client
{
    static void Main()
    {
        // analogie socket(AF_INET, SOCK_STREAM, 0) z C
        Socket client = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        // analogie connect() z C
        client.Connect(IPAddress.Parse("127.0.0.1"), 10000);
        Console.WriteLine("Connected to server on localhost:10000");

        // v C# lze pouzit napriklad NetworkStream pro cteni a zapis dat pres socket
        using (NetworkStream stream = new NetworkStream(client))
        {
            for (char inputChar = 'A'; inputChar <= 'C'; inputChar++)
            {
                // analogie write() nebo send() z C
                stream.WriteByte((byte)inputChar);

                // analogie read() nebo recv() z C
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
