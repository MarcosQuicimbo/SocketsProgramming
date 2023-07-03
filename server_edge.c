// Include the necessary header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// Define some constants
#define PORT 8080 // The port number to listen on
#define MAX_CLIENTS 10 // The maximum number of concurrent clients
#define BUFFER_SIZE 1024 // The size of the message buffer

// A function to handle errors and exit the program
void error(const char *msg)
{
    perror(msg); // Print the error message
    exit(1); // Exit with a non-zero status
}

// A function to handle each client connection
void handle_client(int client_socket)
{
    char buffer[BUFFER_SIZE]; // A buffer to store the messages
    int n; // A variable to store the number of bytes read or written

    // Send a welcome message to the client
    n = write(client_socket, "Hello, this is Bing. How can I help?\n", 36);
    if (n < 0) // Check for errors
        error("ERROR writing to socket");

    // Loop until the client disconnects or sends "quit"
    while (1)
    {
        // Clear the buffer
        memset(buffer, 0, BUFFER_SIZE);

        // Read a message from the client
        n = read(client_socket, buffer, BUFFER_SIZE - 1);
        if (n < 0) // Check for errors
            error("ERROR reading from socket");

        // Print the message to the standard output
        printf("Client: %s\n", buffer);

        // Check if the client wants to quit
        if (strncmp(buffer, "quit", 4) == 0)
            break;

        // TODO: Process the message and generate a response

        // Write a response to the client
        n = write(client_socket, "I'm sorry, I don't know how to answer that.\n", 42);
        if (n < 0) // Check for errors
            error("ERROR writing to socket");
    }

    // Close the client socket
    close(client_socket);
}

// The main function of the server program
int main()
{
    int server_socket; // A variable to store the server socket file descriptor
    int client_socket; // A variable to store the client socket file descriptor
    struct sockaddr_in server_address; // A structure to store the server address information
    struct sockaddr_in client_address; // A structure to store the client address information
    socklen_t client_length; // A variable to store the length of the client address structure

    // Create a server socket using IPv4, TCP and default protocol
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) // Check for errors
        error("ERROR opening socket");

    // Clear the server address structure
    memset(&server_address, 0, sizeof(server_address));

    // Set the server address fields
    server_address.sin_family = AF_INET; // Use IPv4
    server_address.sin_addr.s_addr = INADDR_ANY; // Use any available interface
    server_address.sin_port = htons(PORT); // Use the defined port number

    // Bind the server socket to the server address
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) // Check for errors
        error("ERROR on binding");

    // Listen for incoming connections on the server socket
    if (listen(server_socket, MAX_CLIENTS) < 0) // Check for errors
        error("ERROR on listening");

    printf("Server is listening on port %d\n", PORT); // Print a message to indicate that the server is ready

    // Loop forever to accept and handle client connections
    while (1)
    {
        // Accept a new connection from a client and create a new socket for it
        client_length = sizeof(client_address); // Set the length of the client address structure
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_length);
        if (client_socket < 0) // Check for errors
            error("ERROR on accept");

        printf("A new client has connected\n"); // Print a message to indicate that a new client has connected

        handle_client(client_socket); // Call the function to handle the client connection

        printf("The client has disconnected\n"); // Print a message to indicate that the client has disconnected
    }

    return 0; // Return a zero status
}
