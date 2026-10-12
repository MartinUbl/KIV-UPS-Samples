using System;
using System.Net;
using System.Net.Sockets;

class Server
{
    static void Main()
    {
        // analogie socket(AF_INET, SOCK_STREAM, 0) z C
        Socket listener = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        // analogie bind() z C
        listener.Bind(new IPEndPoint(IPAddress.Loopback, 10000));
        // analogie listen() z C
        listener.Listen(10);
        Console.WriteLine("Server listening on localhost:10000");

        while (true)
        {
            // analogie accept() z C
            using (Socket client = listener.Accept())
            {
                // v C# lze pouzit napriklad NetworkStream pro cteni a zapis dat pres socket
                using (NetworkStream stream = new NetworkStream(client))
                {
                    StreamReader reader = new StreamReader(stream);
                    StreamWriter writer = new StreamWriter(stream) { AutoFlush = true };

                    int input;
                    while (true) {
                        // analogie read() nebo recv() z C
                        string line = reader.ReadLine();
                        if (line == null)
                            break;

                        input = line[0];

                        char c = (char)input;

                        Console.WriteLine($"Received: {c}");

                        char result = c;

                        if (c >= 'A' && c <= 'Z')
                            result = (char)('Z' - (c - 'A'));

                        // analogie write() nebo send() z C
                        writer.WriteLine(result);
                    }
                }
            }
        }
    }
}
