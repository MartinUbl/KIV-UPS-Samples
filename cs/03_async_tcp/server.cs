using System;
using System.Net;
using System.Net.Sockets;
using System.Threading.Tasks;

class Server
{
    static async Task Main()
    {
        Socket listener = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        listener.Bind(new IPEndPoint(IPAddress.Loopback, 10000));
        listener.Listen(10);
        Console.WriteLine("Server listening on localhost:10000");

        while (true)
        {
            Socket client = await listener.AcceptAsync();
            _ = HandleClientAsync(client);
        }
    }

    static async Task HandleClientAsync(Socket client)
    {
        try
        {
            int input;
            while (true)
            {
                byte[] buffer = new byte[1];
                int received = await client.ReceiveAsync(buffer, SocketFlags.None);
                if (received == 0)
                    break;

                input = buffer[0];

                char c = (char)input;

                Console.WriteLine($"Received: {c}");

                char result = c;

                if (c >= 'A' && c <= 'Z')
                    result = (char)('Z' - (c - 'A'));

                await client.SendAsync(new byte[] { (byte)result }, SocketFlags.None);
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error: {ex.Message}");
        }
        finally
        {
            client.Close();
        }
    }
}
