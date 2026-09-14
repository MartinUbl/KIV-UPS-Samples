import java.io.*;
import java.net.*;

public class Server {
    public static void main(String[] args) {

        // analogie socket(AF_INET, SOCK_DGRAM, 0) z C
        DatagramSocket ds;
        try {
            // analogie bind() z C
            ds = new DatagramSocket(10000);
            System.out.println("Server listening on localhost:10000");
        } catch (SocketException e) {
            e.printStackTrace();
            return;
        }

        byte[] buffer = new byte[1];
        // priprava paketu pro prijem dat (jako recvfrom() v C)
        DatagramPacket packet = new DatagramPacket(buffer, buffer.length);

        while (true) {
            try {
                // prijem dat skrz datagramovy socket (jako recvfrom() v C)
                ds.receive(packet);
                char c = (char) buffer[0];
                System.out.println("Received: " + c);

                char result = c;
                if (c >= 'A' && c <= 'Z') {
                    result = (char) ('Z' - (c - 'A'));
                }

                buffer[0] = (byte) result;
                // odeslani dat skrz datagramovy socket (jako sendto() v C)
                ds.send(packet);
            } catch (IOException e) {
                e.printStackTrace();
                break;
            }
        }

        ds.close();
    }
}
