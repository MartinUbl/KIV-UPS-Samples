import java.io.*;
import java.net.*;

class Client
{
    public static void main(String argv[]) throws Exception
    {
        // analogie socket(AF_INET, SOCK_DGRAM, 0) z C
        DatagramSocket ds = new DatagramSocket();

        InetAddress serverAddress = InetAddress.getByName("127.0.0.1");
        int serverPort = 10000;

        for (char inputChar = 'A'; inputChar <= 'C'; inputChar++)
        {
            byte[] buffer = new byte[1];
            buffer[0] = (byte)inputChar;

            // kazdy datagramovy paket je samostatna instance tridy (kombinuje data a cilovou adresu)
            DatagramPacket packet = new DatagramPacket(buffer, buffer.length, serverAddress, serverPort);
            // odeslani dat skrz datagramovy socket (jako sendto() v C)
            ds.send(packet);

            // priprava paketu pro prijem dat (jako recvfrom() v C)
            packet = new DatagramPacket(buffer, buffer.length);
            // prijem dat skrz datagramovy socket (jako recvfrom() v C)
            ds.receive(packet);

            char outputChar = (char)packet.getData()[0];
            System.out.println("Received: " + outputChar);

            Thread.sleep(1000);
        }

        ds.close();
	}
}
