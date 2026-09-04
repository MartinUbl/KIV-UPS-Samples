import java.io.*;
import java.net.*;

public class Server {
    public static void main(String[] args) {
        DatagramSocket ds;
        try {
            ds = new DatagramSocket(10000);
            System.out.println("Server listening on localhost:10000");
        } catch (SocketException e) {
            e.printStackTrace();
            return;
        }

        byte[] buffer = new byte[1];
        DatagramPacket packet = new DatagramPacket(buffer, buffer.length);

        while (true) {
            try {
                ds.receive(packet);
                char c = (char) buffer[0];
                System.out.println("Received: " + c);

                char result = c;
                if (c >= 'A' && c <= 'Z') {
                    result = (char) ('Z' - (c - 'A'));
                }

                buffer[0] = (byte) result;
                ds.send(packet);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
