#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <sys/socket.h>

void error(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[])
{
	int sockfd, numbytes;
	struct sockaddr_in my_addr;
	struct sockaddr_in serv_addr;
	struct stat filestat;

	uint32_t payload_size = 10000;
	uint8_t* data_in = (uint8_t*) malloc(payload_size);

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		error("socket");

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(2324);
	my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1)
		error("bind");

	serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(5134);

	if (lstat("123.mp4", &filestat) < 0)
		error("ERROR : get file size");

	printf("The file size is %lu bytes\n\n", filestat.st_size);

	FILE *fp;
	fp = fopen("123.mp4", "r");

	if (sendto(sockfd, &filestat.st_size, sizeof(filestat.st_size), 0, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
		error("Error:write file_size failed");

	while (!feof(fp)) {

		numbytes = fread(data_in, sizeof(char), payload_size, fp);
		printf("fread %d bytes\n", numbytes);

		numbytes = sendto(sockfd, data_in, numbytes, 0, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
		printf("Sending %d bytes\n\n",numbytes);
	} /* end of while (!feof(fp)) */

	printf("client finished\n");

	free(data_in);
	fclose(fp);
	close(sockfd);
	return 0;
}
