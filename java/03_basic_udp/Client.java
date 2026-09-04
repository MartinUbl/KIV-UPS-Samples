import java.io.*;
import java.net.*;

class Client
{
    public static void main(String argv[]) throws Exception
    {
        DatagramSocket ds;

        ds = new DatagramSocket();

        InetAddress serverAddress = InetAddress.getByName("127.0.0.1");
        int serverPort = 10000;

        for (char inputChar = 'A'; inputChar <= 'C'; inputChar++)
        {
            byte[] buffer = new byte[1];
            buffer[0] = (byte)inputChar;
            DatagramPacket packet = new DatagramPacket(buffer, buffer.length, serverAddress, serverPort);
            ds.send(packet);

            packet = new DatagramPacket(buffer, buffer.length);
            ds.receive(packet);
            char outputChar = (char)packet.getData()[0];
            System.out.println("Received: " + outputChar);

            Thread.sleep(1000);
        }

        ds.close();
	}
}
