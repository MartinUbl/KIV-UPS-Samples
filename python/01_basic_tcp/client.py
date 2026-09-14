import socket
import time

def main():
    # analogie socket(AF_INET, SOCK_STREAM, 0) z C
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        # analogie connect() z C
        s.connect(("127.0.0.1", 10000))
        for input_char in ['A', 'B', 'C']:
            # odeslani dat skrz TCP socket (jako send() v C)
            s.send(input_char.encode())
            # prijem dat skrz TCP socket (jako recv() v C)
            response = s.recv(1)
            if response:
                output_char = response.decode()
                print("Received:", output_char)

            time.sleep(1)

if __name__ == "__main__":
    main()
