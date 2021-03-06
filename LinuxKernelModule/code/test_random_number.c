#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define BUFFER_LENGTH 256               ///< The buffer length (crude but fine)
static char receive[BUFFER_LENGTH];     ///< The receive buffer from the LKM

int main(){
   int ret, fd;

   printf("Starting device test random number by Le Thanh Cong...\n");
   fd = open("/dev/random_number", O_RDONLY);             // Open the device with read only access
   if (fd < 0){
      perror("Failed to open the device...");
      return errno;
   }
   printf("Press ENTER to read back from the device...\n");
   getchar();

   printf("Reading from the device...\n");
   ret = read(fd, receive, BUFFER_LENGTH);        // Read the response from the LKM
   if (ret < 0){
      perror("Failed to read the message from the device.");
      return errno;
   }
   printf("The random nummber is: [%s]\n", receive);
   printf("End of the program\n");
   return 0;
}
