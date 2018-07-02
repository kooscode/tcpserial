/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: koos
 *
 * Created on June 30, 2018, 4:40 PM
 */



#include <cstdlib>
#include <iostream>
#include <cstring>
#include <unistd.h>

//socket stuff
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//serial comms
#include <libserialport.h>
#include "xkserial.hpp"

#define PORT 50505
#define BACKLOG 10

int main(int argc, char** argv) 
{
    
    sp_port* ttyport;
    
    if  ( sp_get_port_by_name("/dev/ttyUSB0", &ttyport) != SP_OK)
    {
        std::cout << "Error getting serial device.";
        return -1;
    }
    
    sp_set_baudrate(ttyport, 9600);
  
    if (sp_open(ttyport, SP_MODE_READ_WRITE) != SP_OK)
    {
        std::cout << "Error opening serial device.";
        return -1;
    }
    
    usleep(2000000);

    //read initial buffer
    char ttybuff[1024];
    //get serial return & send back to client
    std::cout << "<SERIAL START>" << std::endl;
    std::string instr = "[NA]\n\r";
    memset(&ttybuff, 0x00, sizeof(ttybuff));
    int readsize = sp_blocking_read_next(ttyport, &ttybuff, sizeof(ttybuff), 1000);
    if (readsize > 0)
    {
        instr =  std::string(ttybuff);
        std::cout << "SERIAL: " << instr << std::endl;
    }

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
    { 
        perror("socket bind failed");
    }

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
        std::cout << "start listen" << std::endl;
        
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
            
            while(true) 
            {   
                //Get data sent from client
                std::cout << "<TCP RECV>" << std::endl;
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
                std::cout << "TCP: " << msg << std::endl;

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
                        std::cout << "<SERIAL WRITE>" << std::endl;
                        sp_blocking_write(ttyport, msg.c_str(), msg.length(), 1000);

                        //get serial return & send back to client
                        std::cout << "<SERIAL READ>" << std::endl;
                        instr = "[NA]\n\r";
                        memset(&ttybuff, 0x00, sizeof(ttybuff));
                        int readsize = sp_blocking_read_next(ttyport, &ttybuff, sizeof(ttybuff), 1000);
                        if (readsize > 0)
                        {
                            instr =  std::string(ttybuff);
                            //Echo serial msg to terminal..
                            std::cout << "SERIAL: " << instr << std::endl;
                        }

                        //send serial msg to client
                        std::cout << "<TCP WRITE>" << std::endl;
                        write(client_fd, instr.c_str(), instr.length());
                    
                }
                
            }        
    
            close(client_fd);   
        }

        
   }
    close(socket_fd);   
    sp_close(ttyport);

    
    return 0;
}
