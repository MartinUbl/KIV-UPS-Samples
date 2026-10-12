import java.io.*;
import java.net.*;

public class Server {
    public static void main(String[] args) {
        // analogie socket(AF_INET, SOCK_STREAM, 0) z C
        // v Jave je trida ServerSocket automaticky TCP
        try (ServerSocket serverSocket = new ServerSocket(10000)) {
            System.out.println("Server listening on localhost:10000");

            while (true) {
                // analogie accept() z C
                try (Socket clientSocket = serverSocket.accept()) {
                    // v Jave ziskavame vstupni a vystupni proudy ze socketu - pres ty komunikujeme
                    BufferedReader inputStream = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
                    PrintWriter outputStream = new PrintWriter(clientSocket.getOutputStream(), true);

                    String input;
                    // analogie recv() z C
                    while ((input = inputStream.readLine()) != null) {
                        char c = input.charAt(0);
                        System.out.println("Received: " + c);

                        char result = c;
                        if (c >= 'A' && c <= 'Z') {
                            result = (char) ('Z' - (c - 'A'));
                        }

                        // analogie send() z C
                        outputStream.println(result);
                    }
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
