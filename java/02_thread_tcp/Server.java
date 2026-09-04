import java.io.*;
import java.net.*;

public class Server {
    public static void main(String[] args) {
        try (ServerSocket serverSocket = new ServerSocket(10000)) {
            System.out.println("Server listening on localhost:10000");

            while (true) {
                Socket clientSocket = serverSocket.accept();

                // pre-Java 21
                new Thread(() -> handleClient(clientSocket)).start();

                // Java 21 a novejsi
                //Thread.startVirtualThread(() -> handleClient(clientSocket));
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void handleClient(Socket clientSocket) {
        try (InputStream inputStream = clientSocket.getInputStream();
             OutputStream outputStream = clientSocket.getOutputStream()) {

            int input;
            while ((input = inputStream.read()) != -1) {
                char c = (char) input;
                System.out.println("Received: " + c);

                char result = c;
                if (c >= 'A' && c <= 'Z') {
                    result = (char) ('Z' - (c - 'A'));
                }

                outputStream.write((byte) result);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
