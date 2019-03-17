#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // fork
#include <fcntl.h>  // O_CREAT O_WRONLY O_RDONLY
#include <errno.h>  // read the number of the last error

int main (int argc, char *argv[]) {
  int pipeFds[2];   //use to create two file descriptors, pipeFds[0] is read end, pipeFds[1] is write end
  int fileBytesLength;
  char buffer[100];
  char childBuffer[100];
  char* srcFile = argv[1];
  char* dstFile = argv[2];

  if (argc != 3)
  {
    printf("Input error.\nSample : filecopy.exe [targetFileName] [destinationFileName]. \n");
    exit(1);
  }

  // try to create a pipe.
  if (pipe(pipeFds) < 0)
  {
    printf("Something went wrong creating the pipe! %s\n", strerror(errno));
    exit(1);
  }

  switch(fork())
  {
    // fork error
    case -1:
      printf("Error forking child process. %s\n", strerror(errno));
      exit(1);

    // child process
    // read the file from upstream parent process and write it to a new file.
    case 0:
      close(pipeFds[1]);                                                             // close writing end of pipe upstream.
      ssize_t num_bytes_child = read(pipeFds[0], childBuffer, sizeof(childBuffer));  // read file contents from upstream pipe into childBuffer
      close(pipeFds[0]);                                                             // close reading upstream pipe when we're done with it
      int targetDesc = open(dstFile, O_CREAT | O_WRONLY);                            // open a file for writing, create file descriptor.
      write(targetDesc, childBuffer, num_bytes_child);                               // write contents of buffer to new file descriptor.

    // parent process(case 1)
    // read the file and send it down to the child process to write.
    default:
        close(pipeFds[0]);                                              // close reading end of pipe downstream.
        int fileInDesc = open(srcFile, O_RDONLY);                       // read file into file descriptor
        ssize_t num_bytes = read(fileInDesc, buffer, sizeof(buffer));   // get number bytes to read
        write(pipeFds[1], buffer, num_bytes);                           // write bytes to child process.
        close(pipeFds[1]);                                              // close writing downstream pipe when we're done with it.
    }
    return 0;
}
