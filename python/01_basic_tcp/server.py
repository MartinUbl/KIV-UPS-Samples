import socket

def main():
    # analogie socket(AF_INET, SOCK_STREAM, 0) z C
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        # analogie bind() z C
        s.bind(("127.0.0.1", 10000))
        # analogie listen() z C
        s.listen(10)

        print("Server listening on localhost:10000")

        while True:
            # analogie accept() z C
            client, addr = s.accept()
            with client:
                while True:
                    # analogie recv() z C
                    data = client.recv(1)
                    if not data:
                        break

                    # dekodovani prijatych dat (Python implicitne pocita s kodovanim UTF-8)
                    c = data.decode()
                    print("Received:", c)

                    result = c
                    if 'A' <= c <= 'Z':
                        result = chr(ord('Z') - (ord(c) - ord('A')))

                    # analogie send() z C
                    client.send(result.encode())

if __name__ == "__main__":
    main()
