// Include the necessary header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

// Define some constants
#define PORT 8080 // The port number to connect to
#define SERVER_IP "127.0.0.1" // The IP address of the server
#define BUFFER_SIZE 1024 // The size of the message buffer

// A function to handle errors and exit the program
void error(const char *msg)
{
    perror(msg); // Print the error message
    exit(1); // Exit with a non-zero status
}

// The main function of the client program
int main()
{
    int client_socket; // A variable to store the client socket file descriptor
    struct sockaddr_in server_address; // A structure to store the server address information
    char buffer[BUFFER_SIZE]; // A buffer to store the messages
    int n; // A variable to store the number of bytes read or written

    // Create a client socket using IPv4, TCP and default protocol
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) // Check for errors
        error("ERROR opening socket");

    // Clear the server address structure
    memset(&server_address, 0, sizeof(server_address));

    // Set the server address fields
    server_address.sin_family = AF_INET; // Use IPv4
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP); // Use the defined IP address of the server
    server_address.sin_port = htons(PORT); // Use the defined port number of the server

    // Connect to the server socket using the server address information
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) // Check for errors
        error("ERROR connecting");

    printf("Connected to the server\n"); // Print a message to indicate that the connection is established

    // Loop until the user enters "quit"
    while (1)
    {
        // Clear the buffer
        memset(buffer, 0, BUFFER_SIZE);

        // Read a message from the server
        n = read(client_socket, buffer, BUFFER_SIZE - 1);
        if (n < 0) // Check for errors
            error("ERROR reading from socket");

        // Print the message to the standard output
        printf("Server: %s\n", buffer);

        // Check if the server wants to quit
        if (strncmp(buffer, "quit", 4) == 0)
            break;

        // Clear the buffer
        memset(buffer, 0, BUFFER_SIZE);

        // Read a message from the user input
        printf("Enter a message: ");
        fgets(buffer, BUFFER_SIZE - 1, stdin);

        // Write the message to the server
        n = write(client_socket, buffer, strlen(buffer));
        if (n < 0) // Check for errors
            error("ERROR writing to socket");

        // Check if the user wants to quit
        if (strncmp(buffer, "quit", 4) == 0)
            break;
    }

    // Close the client socket
    close(client_socket);

    return 0; // Return a zero status
}
