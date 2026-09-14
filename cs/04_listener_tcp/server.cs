using System;
using System.Net;
using System.Net.Sockets;
using System.Threading.Tasks;

class Server
{
    static async Task Main()
    {
        // TcpListener je vyssi abstrakce nad sockety pro TCP server, analogie socket(), bind() a listen() z C
        TcpListener listener = new(IPAddress.Parse("127.0.0.1"), 10000);
        listener.Start();

        Console.WriteLine("Server listening on localhost:10000");

        while (true)
        {
            // analogie accept() z C, tentokrat v C# svete realizovane pomoci asynchronniho programovani (await/async)
            TcpClient client = await listener.AcceptTcpClientAsync();
            _ = HandleClientAsync(client);
        }
    }

    static async Task HandleClientAsync(TcpClient client)
    {
        using (client)
        {
            try
            {
                NetworkStream stream = client.GetStream();
                byte[] buffer = new byte[1];

                while (true)
                {
                    int received = await stream.ReadAsync(buffer);

                    if (received == 0)
                        break;

                    char input = (char)buffer[0];

                    Console.WriteLine($"Received: {input}");

                    char result = input;

                    if (input >= 'A' && input <= 'Z')
                        result = (char)('Z' - (input - 'A'));

                    buffer[0] = (byte)result;

                    await stream.WriteAsync(buffer);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error: {ex.Message}");
            }
        }
    }
}
