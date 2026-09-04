using System;
using System.Net;
using System.Net.Sockets;

class Server
{
    static void Main()
    {
        Socket listener = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        listener.Bind(new IPEndPoint(IPAddress.Loopback, 10000));
        listener.Listen(10);
        Console.WriteLine("Server listening on localhost:10000");

        while (true)
        {
            Socket client = listener.Accept();

            System.Threading.Thread clientThread = new System.Threading.Thread(HandleClient);
            clientThread.Start(client);
        }
    }
    static void HandleClient(object? obj)
    {
        using (Socket client = (Socket)(obj ?? throw new ArgumentNullException(nameof(obj))))
        {
            using (NetworkStream stream = new NetworkStream(client))
            {
                int input;
                while (true)
                {
                    input = stream.ReadByte();
                    if (input == -1)
                        break;

                    char c = (char)input;

                    Console.WriteLine($"Received: {c}");

                    char result = c;

                    if (c >= 'A' && c <= 'Z')
                        result = (char)('Z' - (c - 'A'));

                    stream.WriteByte((byte)result);
                }
            }
        }
    }
}
