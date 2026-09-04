import java.io.*;
import java.net.*;

class Client
{
    public static void main(String argv[]) throws Exception
    {
        Socket socket = new Socket("127.0.0.1", 10000);

        OutputStream outputStream = socket.getOutputStream();
            InputStream inputStream = socket.getInputStream();
            for (char inputChar = 'A'; inputChar <= 'C'; inputChar++)
            {
                outputStream.write((byte)inputChar);

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
