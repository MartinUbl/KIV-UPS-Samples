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
            StreamReader reader = new StreamReader(stream);
            StreamWriter writer = new StreamWriter(stream) { AutoFlush = true };

            for (char inputChar = 'A'; inputChar <= 'C'; inputChar++)
            {
                // analogie write() nebo send() z C
                writer.WriteLine(inputChar);

                // analogie read() nebo recv() z C
                string response = reader.ReadLine();
                if (response != null)
                {
                    char outputChar = response[0];
                    Console.WriteLine($"Received: {outputChar}");
                }

                Thread.Sleep(1000);
            }

            reader.Dispose();
            writer.Dispose();
        }

        client.Close();
    }
}
