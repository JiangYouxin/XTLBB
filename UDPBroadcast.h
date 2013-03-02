#pragma once

#include "wininet.h"
#pragma comment(lib, "ws2_32.lib")

class UDPBroadcast
{
public:
	UDPBroadcast(USHORT port)
	{
		m_port = port;

		WSADATA data;
		WSAStartup(MAKEWORD(2,0), &data);

		m_listen_socket = socket(AF_INET, SOCK_DGRAM, 0);
		
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(m_port);

		bind(m_listen_socket, (sockaddr *)&addr, sizeof(addr));

		int so_reuseaddr = 1;
		setsockopt(m_listen_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&so_reuseaddr, sizeof(so_reuseaddr));

		m_event = WSACreateEvent();
		WSAEventSelect(m_listen_socket, m_event, FD_READ);

		m_broadcast_socket = socket(AF_INET, SOCK_DGRAM, 0);
		int so_broadcast = 1;
		setsockopt(m_broadcast_socket, SOL_SOCKET, SO_BROADCAST, (char *)&so_broadcast, sizeof(so_broadcast));
	}
	
	~UDPBroadcast()
	{
		closesocket(m_listen_socket);
		closesocket(m_broadcast_socket);
		CloseHandle(m_event);
	}

	HANDLE GetEventHandle()
	{
		return m_event;
	}

	void SendBroadcast(char * content, int size)
	{
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(m_port);
		addr.sin_addr.s_addr = INADDR_BROADCAST;

		int n = sendto(m_broadcast_socket, content, size, 0, (sockaddr *)&addr, sizeof(addr));
	}
	int RecvBroadcast(char * content, int size, sockaddr_in * addr)
	{
		int fromlen = sizeof(sockaddr_in);
		return recvfrom(m_listen_socket, content, size, 0, (sockaddr *)addr, &fromlen);
	}
private:
	USHORT m_port;
	HANDLE m_event;
	SOCKET m_listen_socket;
	SOCKET m_broadcast_socket;
};