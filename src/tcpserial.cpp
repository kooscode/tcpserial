/*
 * Simple application to proxy Serial Port to TCP Server 
 * Copyright (C) 2017 Koos du Preez
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <unistd.h>

//socket stuff
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//serial comms
#include <libserialport.h>

//libterraclear 
//copied and modifed from https://github.com/TerraClear/libterraclear
#include "error_base.hpp"
#include "basicserial.hpp"
#include "thread_rx.hpp"

#define PORT 50505
#define BACKLOG 10

namespace kc = kooscode;

int main(int argc, char** argv) 
{
    kc::basicserial serial1;

    
#ifdef __APPLE__
   std::string serial_path = "/dev/tty.usbmodem14432121";
#else
   std::string serial_path = "/dev/ttyUSB0";
#endif

   //if serial port is supplied use supplied path..
    if (argc > 1)
    {
       serial_path = argv[1];
    }
    
   
    try
    {
        //try to open serial port..
        serial1.open(serial_path, kc::BAUD_115200);
    } catch (kc::error_base err)
    {
        std::cout << "Error Opening Serial Port.. " << std::endl;
        std::cout << "\tSyntax : tcpserial [SERIAL_DEVICE_PATH]" << std::endl;
        std::cout << "\tExample: tcpserial /dev/ttyUSB0" << std::endl << std::endl;
        return -1;
    }

    //Connected..
    std::cout << "Connected Serial Device: " << serial_path << " @ " << kc::BAUD_115200 << "bps" << std::endl;

    struct sockaddr_in server;
    struct sockaddr_in dest;
    int status, socket_fd, client_fd;
    socklen_t size;
    int enable = 1;
    int retval = 0;

    //clear out server & dest
    memset(&server, 0, sizeof(server));
    memset(&dest,0,sizeof(dest));
    
    //Set Server to bind to IP V4 on specific port.
    server.sin_family = AF_INET; //IPV4
    server.sin_port = htons(PORT); //Bind to PORT as #defined
    server.sin_addr.s_addr = INADDR_ANY; //Bind to ALL IP's

    //Buffer to hold data
    char buffer[10241];
    char*buff;
   
    //Open Socket File Descriptor..
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)   
    {
       perror("Socket File Descriptor failed..");
        return -1;
    }
    
    //allow re-use of adddress
    retval = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    if ( retval == -1 )   
    {
         perror("setsockopt(SO_REUSEADDR) failed");

    }
    
    #ifdef SO_REUSEPORT
    retval = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int));
    if (retval == -1)
        perror("setsockopt(SO_REUSEPORT) failed");
    #endif

    //bind server socket
    retval = bind(socket_fd, (struct sockaddr *)&server, sizeof(struct sockaddr ));
    if (retval == -1)
        perror("bind failed");
    
    //Start Listening mode..
    retval = listen(socket_fd, BACKLOG);
    if (retval == -1)
    {
        perror("Socket listen failed");
        return -1;
    }

    //loop and accept all connections and wait for msg..
    while(true) 
    {
        size = sizeof(struct sockaddr_in);  

        //start listen...
        std::cout << "TCP Listening on port: " << PORT << std::endl;
        
        client_fd = accept(socket_fd, (struct sockaddr *)&dest, &size); 
        if (client_fd == -1)
        {
            perror("accept failed");
        }
        else
        {
            std::cout << "Connected: " << inet_ntoa(dest.sin_addr) << std::endl;
            
            //Send connected msg
            std::string msg = "[[ WELCOME TO SERIAL TCP ]]\n";
            write(client_fd, msg.c_str(), msg.length());

            //start serial receive thread..
            kc::thread_rx trx(client_fd, &serial1);
            trx.thread_start("rx");

            while(true) 
            {   
                //Get data sent from client
                int inbytes = recv(client_fd, buffer, 10240,0);
                if (inbytes < 0)
                {
                    perror("recv");
                    break;
                }           
                else if (inbytes == 0) 
                {
                    printf("Connection closed\n");
                    break;
                }
                
                //read TCP msg
                msg = std::string (buffer);
                std::cout << "TCP > TTY: " << msg << std::endl;

                if (msg.substr(0,1) == "!")
                {
                    std::cout << "quit!" << std::endl;
                    break;
                }
                else if ( msg.empty() || (msg.substr(0,1) == "\n") || (msg.substr(0,1) == "\r"))
                {
                    std::cout << "<empty>" << std::endl;
                }
                else
                {                 
                    //write client msg to serial port..
                    serial1.writeString(msg, 500);
                }
                
            }        
    
            close(client_fd);   
            trx.thread_stopwait(); //Stop RX thread..
        }

        
   }
    close(socket_fd);   
    
    return 0;
}
