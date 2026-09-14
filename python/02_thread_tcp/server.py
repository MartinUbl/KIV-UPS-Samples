import socket
import threading

def handle_client(client):
    with client:
        while True:
            data = client.recv(1)

            if not data:
                break

            c = data.decode()
            print("Received:", c)

            result = c

            if 'A' <= c <= 'Z':
                result = chr(ord('Z') - (ord(c) - ord('A')))

            client.sendall(result.encode())

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind(("127.0.0.1", 10000))
        s.listen(10)

        print("Server listening on localhost:10000")

        while True:
            client, addr = s.accept()
            # pro odbaveni klienta vytvorime nove vlakno
            threading.Thread(target=handle_client, args=(client,), daemon=True).start()

if __name__ == "__main__":
    main()
