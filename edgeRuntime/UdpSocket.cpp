//
//  UdpSocket.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 06/11/13.
//  Copyright (c) 2013 Abdelrahaman Aly. All rights reserved.


//Generic Headers
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

//Custom Headers
#include "UdpSocket.h"
#include "Constants.h"

namespace Sockets 
{
    
    Sockets::UdpSocket::UdpSocket(Players::StandardPlayer * player,int type)
    {

        //socket to exit with listener port
       // int buffsize = 65536; // 65536
        if (type==Utilities::Constants::LISTENING_SOCKET) {
            int handleIn = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
            
            //setsockopt(handleIn, SOL_SOCKET, SO_RCVBUF, (const void*)&buffsize, sizeof(buffsize));
            
            if ( handleIn <= 0 )
            {
                printf( "failed to create socket\n" );
                return;
            }
            
            sockaddr_in in_address; //adress of the socket
            in_address.sin_family = AF_INET; //is an inet comm
            in_address.sin_addr.s_addr = INADDR_ANY;
            in_address.sin_port = htons(player->getPort());        //port configuration -->htons changes the configuration of the bits in the 16 bit integer form value order to network order  big-endian.. for 32 bits integer we could use  htonl
            
            if ( bind( handleIn, (const sockaddr*) &in_address, sizeof(sockaddr_in) ) < 0 ) //it binds the socket with the newly determined port
            {
                printf( "failed to bind socket\n" );
                return;
            }
            
            int nonBlocking = 1;
            if ( fcntl( handleIn, F_SETFL, O_NONBLOCK, nonBlocking ) == -1 ) // fcntl utility funcion that solves the issue of nonBLocking in the handle which is the binded socket
            {
                printf( "failed to set non-blocking socket\n" );
                return;
            }
            handleIn_=handleIn;    
  
        }
        else if (type==Utilities::Constants::EXIT_SOCKET)
        {
            //socket to enter with random available port
            
            int handleOut = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
            //setsockopt(handleOut, SOL_SOCKET, SO_RCVBUF, (const void*)&buffsize, sizeof(buffsize));
            
            if ( handleOut <= 0 )
            {
                printf( "failed to create socket\n" );
                return;
            }
            
            
            
            sockaddr_in out_address; //adress of the socket
            out_address.sin_family = AF_INET; //is an inet comm
            out_address.sin_addr.s_addr = INADDR_ANY;
            out_address.sin_port = htons( (unsigned short) 0 ); //port configuration -->htons changes the configuration of the bits in the 16 bit integer form value order to network order  big-endian.. for 32 bits integer we could use  htonl
            
            if ( bind( handleOut , (const sockaddr*) &out_address, sizeof(sockaddr_in) ) < 0 ) //it binds the socket with the newly determined port
            {
                printf( "failed to bind socket\n" );
                return;
            }
            
            int nonBlocking = 1;
            
            if ( fcntl( handleOut, F_SETFL, O_NONBLOCK, nonBlocking ) == -1 ) // fcntl utility funcion that solves the issue of nonBLocking in the handle which is the binded socket
            {
                printf( "failed to set non-blocking socket\n" );
                return ;
            }
            handleOut_=handleOut;
        }  
     
        
        player_=player;     
     
    };
    
    Sockets::UdpSocket::~UdpSocket()
    {
    
    };

    int Sockets::UdpSocket::sendTo(Players::StandardPlayer *player, std::string data, bool alerts)
    {
        

        //creates correct form of ip address
        unsigned int a = player->getIpAdress()[0];
        unsigned int b = player->getIpAdress()[1];
        unsigned int c = player->getIpAdress()[2];
        unsigned int d = player->getIpAdress()[3];
        
        //instantiate data needed for communication
        unsigned short port = player->getPort();
        unsigned int destination_address = ( a << 24 ) | ( b << 16 ) | ( c << 8 ) | d;
        unsigned short destination_port = port;
        
        //creates socket
        sockaddr_in out_address;
        out_address.sin_family = AF_INET;
        out_address.sin_addr.s_addr = htonl( destination_address );
        out_address.sin_port = htons( destination_port );

        //message is prepared to be sent
        std::strcat((char *)data.c_str(),"\0");
        char  content [256]="";
        std::strcpy( (char *)content,data.c_str());

        //data is sent using edited data
        int sent_bytes = sendto( handleOut_,(const char *) content, sizeof content,
                            0, (sockaddr*)&out_address, sizeof(sockaddr_in) );
    
        //control instructions in case of error
        if ( sent_bytes != sizeof(content))
        {
            printf( "failed to send packet: return value = %d\n", sent_bytes );
            return 0;
        }
        if (alerts) {
                    std::cout<< "Packet sent: " <<content<< "\n";
        }

        return 1;
    };


    char * Sockets::UdpSocket::receiveFrom(Players::StandardPlayer *player, bool alerts)
    {
    
       
        unsigned char packet_data[256]="";// change this for an actual size -> revise future miss behavior

        //main loop to retreive information from media
        while ( true )
        {
            //data for recolection being prepared
            unsigned int maximum_packet_size = sizeof( packet_data );
            sockaddr_in from;
            socklen_t fromLength = sizeof( from );
            
            //retreival of data
            int received_bytes =  recvfrom( handleIn_, (char*)packet_data, maximum_packet_size,
                                       0, (sockaddr*)&from, &fromLength );
        
            //checks on break loop in case data has been retreived
            if ( received_bytes == 1 )
                break;
            else if (received_bytes>1) 
            {
                if (alerts) {
                    std::cout <<" Data received is is: "<< packet_data <<"\n"; 
                }
                break;
            } 
        }
        //returns information obtained
        
        return (char*)packet_data;
    };
}