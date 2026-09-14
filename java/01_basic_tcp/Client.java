import java.io.*;
import java.net.*;

class Client
{
    public static void main(String argv[]) throws Exception
    {
        // analogie socket(AF_INET, SOCK_STREAM, 0) z C
        // v Jave je trida Socket automaticky TCP
        Socket socket = new Socket("127.0.0.1", 10000);

        // Java ziskava vstupni a vystupni proudy ze socketu - pres ty komunikujeme
        OutputStream outputStream = socket.getOutputStream();
        InputStream inputStream = socket.getInputStream();
        for (char inputChar = 'A'; inputChar <= 'C'; inputChar++)
        {
            // zapis do streamu = odeslani dat skrz socket (jako send() v C)
            outputStream.write((byte)inputChar);

            // cteni ze streamu = prijem dat skrz socket (jako recv() v C)
            int response = inputStream.read();
            if (response != -1)
            {
                char outputChar = (char)response;
                System.out.println("Received: " + outputChar);
            }

            Thread.sleep(1000);
        }
        socket.close();
    }
}
