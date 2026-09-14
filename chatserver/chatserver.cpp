#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <time.h>
#include <errno.h>
#include <arpa/inet.h>

#include <set>
#include <vector>
#include <iostream>
#include <map>
#include <array>
#include <sstream>
#include <numeric>
#include <iomanip>

/**
 * POZOR: toto je server napsany pred cvicenim v ramci "par minut", a tedy je v nem spousty prasarnen z pohledu
 *        jazyka C++ nebo algoritmizace obecne. Pro Vasi semestralku pochopitelne zvolte korektni konstrukce
 *        a pokuste se pouzit dostupne prvky prog. jazyka, ktery pouzivate
 **/

enum class ChatterState
{
	CONN,
	CHAT
};

int main(int argc, char** argv)
{
	int serversocket;
	int res;
	std::vector<char> buffer;

	buffer.resize(1024);

	serversocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serversocket < 0)
	{
		std::cerr << "Nelze vytvorit serverovy socket" << std::endl;
		return 1;
	}

	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(4242);
	serveraddr.sin_addr.s_addr = INADDR_ANY;

	int param = 1;
	res = setsockopt(serversocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&param, sizeof(int));

	res = bind(serversocket, (sockaddr*)&serveraddr, sizeof(sockaddr_in));
	if (res < 0)
	{
		std::cerr << "Nelze nabindovat" << std::endl;
		return 2;
	}

	res = listen(serversocket, 10);
	if (res < 0)
	{
		std::cerr << "Nelze vytvorit frontu spojeni" << std::endl;
		return 3;
	}

	fd_set rdset;
	FD_ZERO(&rdset);
	FD_SET(serversocket, &rdset);

	fd_set tests;

	int maxsocket = serversocket;

	sockaddr_in inaddr;
	socklen_t addrlen = sizeof(sockaddr_in);

	std::set<int> chatClients;
	int a2read;

	struct clientrec
	{
		std::array<char, 1024> buffer;
		int ptr = 0;
		int expectLen = -1;

		time_t lastCom = 0;

		ChatterState state = ChatterState::CONN;

		std::string nickname = "";
	};

	std::map<int, clientrec> chatters;

	auto clearClient = [&chatters, &chatClients, &rdset](int skt) {
		close(skt);
		FD_CLR(skt, &rdset);
		chatClients.erase(skt);
		chatters.erase(skt);
	};

	auto broadcast = [&chatters](const std::string& str) {
		for (auto& cl : chatters)
		{
			std::cout << "SEND("<<cl.first<<"): " << str << std::endl;
			send(cl.first, str.c_str(), str.length(), 0);
		}
	};

	struct timeval tv;

	while (true)
	{
		tests = rdset;

		tv.tv_sec = 1;
		tv.tv_usec = 1;

		res = select(maxsocket+1, &tests, nullptr, nullptr, &tv);
		if (res < 0)
		{
			std::cerr << "Fatal error on select(): " << errno << std::endl;
			break;
		}
		else if (res == 0)
		{
			// timeout
			//
			std::set<int> toTerminate;

			for (auto& cl : chatters)
			{
				if (cl.first != serversocket && cl.second.state == ChatterState::CONN && cl.second.lastCom + 5 < time(NULL))
				{
					toTerminate.insert(cl.first);
				}
			}

			for (auto skt : toTerminate)
			{
				std::cout << "Terminating: " << skt << " (timeout)" << std::endl;
				clearClient(skt);
			}
		}
		else
		{
			for (int skt = 3; skt <= maxsocket; skt++)
			{
				if (FD_ISSET(skt, &tests))
				{
					if (skt == serversocket)
					{
						res = accept(serversocket, (sockaddr*)&inaddr, &addrlen);
						if (res > 0)
						{
							chatClients.insert(res);
							chatters[res] = {{}, 0, -1, time(NULL), ChatterState::CONN, ""};
							FD_SET(res, &rdset);
							std::cout << "CONNECTED " << res << std::endl;
							if (res > maxsocket)
								maxsocket = res;
						}
					}
					else
					{
						ioctl(skt, FIONREAD, &a2read);
						if (a2read <= 0 || a2read > buffer.size())
						{
							std::cout << "DISCONNECTED" << std::endl;
							clearClient(skt);
						}
						else
						{
							auto& rec = chatters[skt];

							std::cout << "IN " << a2read << std::endl;
							int expectRecvLen = (rec.ptr < 14) ? 14 : rec.expectLen;

							std::cout << "Expecting " << expectRecvLen << std::endl;

							int rcvd = recv(skt, rec.buffer.data() + rec.ptr, expectRecvLen, 0);
							if (rcvd < 0)
							{
								std::cout << "DISCONNECTED (2)" << std::endl;
								clearClient(skt);
								continue;
							}
							rec.buffer[rec.ptr + rcvd] = '\0';
							rec.ptr += rcvd;
							rec.lastCom = time(NULL);

							if (rec.ptr >= 14)
							{
								std::string r = rec.buffer.data();
								std::cout << "Buffer: " << r << std::endl;
								if (rec.ptr - rcvd < 14 && r.length() >= 14)
								{
									std::string magic = r.substr(0, 6);

									std::string opcode = r.substr(6, 4);

									std::cout << "MAGIC: " << r.substr(0, 6) << std::endl;
									std::cout << "Opcode: " << r.substr(6, 4) << std::endl;

									if (magic != "KIVUPS")
									{
										std::cout << "Neplatny identifikator paketu, DISCONNECT" << std::endl;
										clearClient(skt);
										continue;
									}
									if (opcode != "nick" && opcode != "chat" && opcode != "exit")
									{
										std::cout << "Neplatny opkod, DISCONNECT" << std::endl;
										clearClient(skt);
										continue;
									}

									try
									{
										rec.expectLen = std::stoi(r.substr(10, 4));
									}
									catch (...)
									{
										rec.expectLen = -1;
									}

									std::cout << "Expected total body length: " << rec.expectLen << std::endl;

									if (rec.expectLen <= 0 || rec.expectLen >= 9999)
									{
										std::cout << "Neplatna delka paketu, DISCONNECT" <<std::endl;
										clearClient(skt);
										continue;
									}
								}

								if (rec.expectLen > 0 && rec.ptr - 14 == rec.expectLen)
								{
									std::cout << "Zpracovavam..." << std::endl;

									std::string opcode = r.substr(6, 4);

									if (opcode == "nick")
									{
										int nlen = -1;
										try
										{
											nlen = std::stoi(r.substr(14, 2));
										}
										catch (...)
										{
										}

										if (nlen < 4)
										{
											std::cout << "Neplatna delka nicku" << std::endl;
											clearClient(skt);
											continue;
										}

										std::string newNick = r.substr(16, nlen);
										// TODO: validace alfanum znaku

										std::string oldNick = rec.nickname;
										rec.nickname = newNick;

										std::ostringstream oss;
										oss << std::setw(2) << std::setfill('0') << newNick.size() << newNick;

										std::ostringstream osspkt;
										if (rec.state == ChatterState::CONN)
										{
											std::cout << "Vitej, " << newNick << "!" << std::endl;

											rec.state = ChatterState::CHAT;
											osspkt << "KIVUPSconn" << std::setw(4) << std::setfill('0') << oss.str().length() << oss.str();
										}
										else
										{
											std::cout << "ZMENA: " << oldNick << " na " << newNick << std::endl;
											oss << std::setw(2) << std::setfill('0') << oldNick.length() << oldNick;

											osspkt << "KIVUPSnick" << std::setw(4) << std::setfill('0') << oss.str().length() << oss.str();
										}

										broadcast(osspkt.str());
									}
									else if (opcode == "chat")
									{
										if (rec.state != ChatterState::CHAT)
										{
											std::cout << "Chat zprava ve stavu bez prezdivky, DISCONNECT" << std::endl;
											clearClient(skt);
											continue;
										}
										else
										{

										int nlen = -1;
										try
										{
											nlen = std::stoi(r.substr(14, 3));
										}
										catch (...)
										{
										}

										std::string chatMsg = r.substr(17, nlen);

										std::cout << "[[" << rec.nickname << "]]: " << chatMsg << std::endl;

										std::ostringstream oss;
										oss << std::setw(2) << std::setfill('0') << rec.nickname.size() << rec.nickname;
										oss << std::setw(3) << std::setfill('0') << chatMsg.size() << chatMsg;

										std::ostringstream osspkt;
										osspkt << "KIVUPSchat" << std::setw(4) << std::setfill('0') << oss.str().length() << oss.str();

										broadcast(osspkt.str());
										}
									}
									else if (opcode == "exit")
									{
										std::cout << "ODCHOD " << rec.nickname << "!" << std::endl;

										int nlen = -1;
										try
										{
											nlen = std::stoi(r.substr(14,2));
										}
										catch (...)
										{
										}

										if (nlen <= 0)
										{
											std::cout << "Neplatna delka duvodu, DISCONNECT" << std::endl;
											clearClient(skt);
											continue;
										}

										std::string reason = r.substr(16, nlen);

										std::ostringstream oss;
                                                                                oss << std::setw(2) << std::setfill('0') << rec.nickname.size() << rec.nickname;
										oss << std::setw(2) << std::setfill('0') << reason.size() << reason;

                                                                                std::ostringstream osspkt;
                                                                                osspkt << "KIVUPSdisc" << std::setw(4) << std::setfill('0') << oss.str().length() << oss.str();

                                                                                broadcast(osspkt.str());

									}

									rec.ptr = 0;
									rec.expectLen = -1;
									std::cout << "Hotovo" << std::endl;
								}
							}
						
						}
					}
				}
			}
		}
	}

	return 0;
}
