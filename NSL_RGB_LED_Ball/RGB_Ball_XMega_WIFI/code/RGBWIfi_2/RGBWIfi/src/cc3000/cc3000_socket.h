/*

Copyright (c) 2013, Embedded Adventures, www.embeddedadventures.com
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
 
- Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

- Neither the name of Embedded Adventures nor the names of its contributors
  may be used to endorse or promote products derived from this software
  without specific prior written permission.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
THE POSSIBILITY OF SUCH DAMAGE.

Contact us at admin@embeddedadventures.com

*/

#ifndef __cc3000_socket_h
#define __cc3000_socket_h

#include "user_board.h"
#include "cc3000\cc3000.h"
#include "cc3000\cc3000_hci.h"

// Socket domains / families

#define  AF_INET                2
#define  AF_INET6               23

// Socket type

#define  SOCK_STREAM            1
#define  SOCK_DGRAM             2
#define  SOCK_RAW               3           // Raw sockets allow new IPv4 protocols to be implemented in user space. A raw socket receives or sends the raw datagram not including link level headers
#define  SOCK_RDM               4
#define  SOCK_SEQPACKET         5

// Socket protocol

#define IPPROTO_IP              0           // dummy for IP
#define IPPROTO_ICMP            1           // control message protocol
#define IPPROTO_IPV4            IPPROTO_IP  // IP inside IP
#define IPPROTO_TCP             6           // tcp
#define IPPROTO_UDP             17          // user datagram protocol
#define IPPROTO_IPV6            41          // IPv6 in IPv6
#define IPPROTO_NONE            59          // No next header
#define IPPROTO_RAW             255         // raw IP packet

#define htons(a) a >> 8 | a << 8

// socket options - must find better define names for these. SOCK_ON is not representative of what it does!

#define  SOL_SOCKET             0xffff		//  socket level
#define  SOCKOPT_RECV_NONBLOCK         	0	// recv non block mode, set SOCK_ON or SOCK_OFF (default block mode)
#define  SOCKOPT_RECV_TIMEOUT			1	// optname to configure recv and recvfromtimeout
#define  SOCKOPT_ACCEPT_NONBLOCK		2	// accept non block mode, set SOCK_ON or SOCK_OFF (default block mode)
#define  SOCK_ON                		0	// socket non-blocking mode	is enabled		
#define  SOCK_OFF               		1	// socket blocking mode is enabled



struct in_addr {
    unsigned long s_addr;                 // load with inet_aton()
};


struct sockaddr_in {
    short            sin_family;   ///< Socket family, e.g. AF_INET, AF_INET6
    unsigned short   sin_port;     ///< Port, e.g. htons(3490)
    struct in_addr   sin_addr;     ///< Address, see struct in_addr, above
    char             sin_zero[8];  ///< zero this if you want to, see http://silviocesare.wordpress.com/2007/10/22/setting-sin_zero-to-0-in-struct-sockaddr_in/
};

struct socket_accept_params_t
{
    long            sd;
    long            status;
    struct sockaddr_in		socket_address;
    
};


typedef struct socket_select_params
{
    long					status;
	unsigned long 			read_sd;
	unsigned long 			write_sd;
	unsigned long 			ex_sd;
} tBsdSelectRecvParams;


typedef struct _bsd_getsockopt_return_t
{
	unsigned char			ucOptValue[4];
	char						iStatus;
} tBsdGetSockOptReturnParams;

typedef struct _bsd_gethostbyname_return_t
{
    long             retVal;
    long             outputAddress;
} tBsdGethostbynameParams;

#define MDNS_DEVICE_SERVICE_MAX_LENGTH 32

/**
	\brief Create a new socket
	
	\param domain The domain in which to create the socket, currently only AF_INET is supported
	\param type The socket type - either SOCK_STREAM, SOCK_DGRAM,  SOCK_RAW
	\param protocol The protocol - either IPPROTO_TCP, IPPROTO_UDP or IPPROTO_RAW
	
	\returns The socket descriptor, or -1 if error
*/

int8_t cc3000_socket(uint8_t domain, uint8_t type, uint8_t protocol);

/** 
 
    \brief Connect a socket to a given port and address
 
    \param sd Socket descriptor
    \param port Port to connect to
    \param addr Address to connect to
    
	\returns 0 on success, -1 on errror
*/

hci_status_t cc3000_connect(uint8_t sd, uint16_t port, ip_addr_t addr);


/**

	\brief Send data to the other end of a connected TCP socket
	
	\pre The socket has already been created with \ref cc3000_socket
	
	\param sd Socket descriptor
	\param buf Pointer to data
	\param len Length of data
	\param flags Socket flags - not used at present
	
	\returns Number of bytes returned, -1 if error, -2 for no free buffers
*/

int16_t cc3000_send(uint8_t sd, uint8_t *buf, uint16_t len, uint8_t flags);

/**

	\brief Receive data to the other end of a connected TCP socket

	\pre The socket has already been connected with \ref cc3000_connect
	
	\param sd Socket descriptor
	\param buf Pointer to data
	\param len Length of data
	\param flags Socket flags - not used at present
	
	\pre The socket is already connected.
	
	\returns Number of bytes returned, -1 if error
*/

int16_t cc3000_recv(uint8_t sd, uint8_t *buf, uint16_t len, uint8_t flags);

/**

	\brief Close socket
	
	\param sd Socket descriptor
	
	\returns 0 for success, -1 for error
*/

hci_status_t cc3000_socket_close(uint8_t sd);

/**

	\brief Start or stop advertising the MDNS service on the CC3000
	
	The MDNS service is used primarily for signalling to the iphone app that 
	smart config has actually completed. Of course it could be used for other things 
	as well, since it makes it easy to find out the device's IP address.
	
	\param mdns_enabled Set to 0 to stop advertising, 1 to start advertising
	\param service_name Null terminated string indicating the service name (must match what is in the iphone app)
	
	\returns 0 for success, -1 for error
*/

hci_status_t cc3000_socket_mdns_advertise(uint8_t mdns_enabled, uint8_t *service_name);


#endif
