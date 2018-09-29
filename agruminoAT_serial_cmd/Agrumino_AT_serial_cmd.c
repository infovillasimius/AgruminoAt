/*
  agruminoAT_serial_cmd - AT commands interpreter project for Agrumino board using the Agrumino library.
  Created by Antonello Meloni & Hicham Lafhouli.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>   /* File Control Definitions */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions */
#include <errno.h>   /* ERROR Number Definitions */


int esiste_file (char *);
void leggi(char *cercato, int fd);

int main(int argc,char *argv[])
{

    FILE *cmd=NULL; /* Pointer to file */
    char buf[1000]; /* buffer for file reading */
    int ret;        /* integer for EOF checking */
    char port[255]; /* buffer for storing port name */

    /* checking parameters */
    if(argc<3) {
        printf("\nInsufficient number of parameters!");
        printf("\nPort and file name must be provided!\n");
        return -1;
    } else {
        if(!esiste_file(argv[2])) {
            printf("\nFile does not exist\n");
            return -1;
        }
    }

    strcpy(port,argv[1]);
    cmd=fopen(argv[2],"r");

    int fd;     /*File Descriptor*/

    fd = open(port,O_RDWR | O_NOCTTY | O_NDELAY);   /* Opening the Serial Port */
    /* O_RDWR Read/Write access to serial port           */
    /* O_NOCTTY - No terminal will control the process   */
    /* O_NDELAY -Non Blocking Mode,Does not care about-  */
    /* -the status of DCD line,Open() returns immediatly */

    printf("\n +----------------------------------+");
    printf("\n |        Serial Port Write         |");
    printf("\n +----------------------------------+");

    /* Error Checking */
    if(fd == -1){
        printf("\n  Error! in Opening %s",port);
        return -1;
    } else {
        printf("\n  %s Opened Successfully ",port);
    }

    struct termios SerialPortSettings;	                        /* Create the structure */
    tcgetattr(fd, &SerialPortSettings);	                        /* Get the current attributes of the Serial port */
    cfsetispeed(&SerialPortSettings,B115200);                   /* Set Read  Speed as 115200 */
    cfsetospeed(&SerialPortSettings,B115200);                   /* Set Write Speed as 115200 */
    SerialPortSettings.c_cflag &= ~PARENB;                      /* Disables the Parity Enable bit(PARENB),So No Parity   */
    SerialPortSettings.c_cflag &= ~CSTOPB;                      /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
    SerialPortSettings.c_cflag &= ~CSIZE;	                    /* Clears the mask for setting the data size */
    SerialPortSettings.c_cflag |=  CS8;                         /* Set the data bits = 8 */
    SerialPortSettings.c_cflag &= ~CRTSCTS;                     /* No Hardware flow Control */
    SerialPortSettings.c_cflag |= CREAD | CLOCAL;               /* Enable receiver,Ignore Modem Control lines  */
    SerialPortSettings.c_iflag &= (IXON | IXOFF | IXANY);       /* Disable XON/XOFF flow control both i/p and o/p */
    SerialPortSettings.c_lflag = ICANON;                        /* Canonical mode */
    SerialPortSettings.c_iflag &= ~( ECHO | ECHOE | ISIG);
    SerialPortSettings.c_oflag &= ~OPOST;                       /*No Output Processing*/

    if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0){		/* Set the attributes to the termios structure*/
        printf("\n  ERROR ! in Setting attributes");
        return -1;
    } else {
        printf("\n  BaudRate = 115200 \n  StopBits = 1 \n  Parity   = none");
    }

    leggi("interpreter",fd);    //look for the right Agrumino output to start sending commands
    ret=fscanf (cmd,"%s",buf);  //read the first line from commands file

    while(ret!=EOF)
    {
        int  bytes_written  = 0;  	                      /* Value for storing the number of bytes written to the port */
        strcat(buf,"\n");
        bytes_written = write(fd,buf,strlen(buf));        /* send data to port */

        printf("\n  %s written to %s",buf,port);
        printf("\n  %d Bytes written to %s", bytes_written, port);
        printf("\n +----------------------------------+\n\n");

        ret=fscanf (cmd,"%s",buf);
    }

    leggi("____________________________________________",fd);

    close(fd);                   /* Close the Serial port */

}


int esiste_file (char *nomefile)
{
    FILE *fp=NULL;
    fp = fopen(nomefile,"r");                           // apertura file
    if (fp!=NULL)                                       // verifica apertura file
    {
        fclose(fp);                                     // se il file esiste lo chiude e restituisce 1
        return 1;
    }
    return 0;                                           // altrimenti restituisce 0
}


void leggi(char *cercato, int fd)                       //serial data output check and sync
{
    printf("\n\nPrinting data received from port \n\n");

    char *f=NULL;
	char buf [100];
	int n = 0;

    while(f==NULL)
    {
        n = read (fd, buf, sizeof buf);
        write (STDOUT_FILENO, buf, n);
        f=strstr(buf, cercato);
    }

    printf("\n\nEnd of data received from port!!\n\n");
}


