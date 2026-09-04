import socket
import time

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect(("127.0.0.1", 10000))
        for input_char in ['A', 'B', 'C']:
            s.send(input_char.encode())
            response = s.recv(1)
            if response:
                output_char = response.decode()
                print("Received:", output_char)

            time.sleep(1)

if __name__ == "__main__":
    main()
