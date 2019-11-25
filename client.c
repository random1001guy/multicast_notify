#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>


#define PORT 5000
#define COURSE_1 "239.1.0.1"
#define COURSE_2 "239.1.0.2"
#define COURSE_3 "239.1.0.3"
#define COURSE_4 "239.1.0.4"
#define COURSE_5 "239.1.0.5"
#define COURSE_6 "239.1.0.6"
#define COURSE_7 "239.1.0.7"
#define COURSE_8 "239.1.0.8"
#define COURSE_9 "239.1.0.9"
#define COURSE_10 "239.1.0.10"


void send_notification (int sock_send);
void receive_notification (int sock_listen);



char *course_arr[] =
  { COURSE_1, COURSE_2, COURSE_3, COURSE_4, COURSE_5, COURSE_6, COURSE_7,COURSE_8, COURSE_9, COURSE_10 };
struct sockaddr_in addr;
int addrlen, sock_listen, cnt, sock_send;
struct ip_mreq mreq;


int
main ()
{


  fd_set master, rset;
  int maxfd;

  bzero ((char *) &addr, sizeof (addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl (INADDR_ANY);
  addr.sin_port = htons (PORT);
  addrlen = sizeof (addr);
  /* set up socket */

  sock_listen = socket (AF_INET, SOCK_DGRAM, 0);
  if (sock_listen < 0)
    {
      perror ("socket_listen error while cretaing socket");
      exit (1);
    }

  sock_send = socket (AF_INET, SOCK_DGRAM, 0);
  if (sock_send < 0)
    {
      perror ("socket_send error while cretaing socket");
      exit (1);
    }

  if (bind (sock_listen, (struct sockaddr *) &addr, sizeof (addr)) < 0)
    {
      perror ("bind error");
      exit (1);
    }


  int num_courses;


  printf ("\nEnter number of courses you want to register in\n");
  scanf ("%d", &num_courses);
  for (int i = 0; i < 10; i++)
    {
      printf ("\nCourse no : %d ", i + 1);
    }
  printf ("\nEnter the course numbers you want to register in ");

  mreq.imr_interface.s_addr = htonl (INADDR_ANY);
  for (int i = 0; i < num_courses; i++)
    {
      int index;
      printf ("\n\tCourse %d : ", i + 1);
      scanf ("%d", &index);

      mreq.imr_multiaddr.s_addr = inet_addr (course_arr[index - 1]);


      if (setsockopt (sock_listen, IPPROTO_IP, IP_ADD_MEMBERSHIP,
		      &mreq, sizeof (mreq)) < 0)
	{
	  perror ("setsockopt mreq");
	  exit (1);
	}
    }

  FD_ZERO (&master);
  FD_ZERO (&rset);
  FD_SET (sock_listen, &master);
  FD_SET (STDIN_FILENO, &master);

  maxfd = sock_listen;


  for (;;)
    {
      rset = master;

      printf ("\nPress any key to send notification\n");

      if (select (maxfd + 1, &rset, NULL, NULL, NULL) == -1)
	{
	  perror ("select() failed");
	  exit (0);
	}


      for (int i = 0; i <= maxfd; i++)
	{

	  if (FD_ISSET (i, &rset))
	    {

	      if (i == STDIN_FILENO)
		{
		  send_notification (sock_send);
		}
	      else if (i == sock_listen)
		{
		  receive_notification (sock_listen);
		}
	      else
		{
		  printf ("unknown socket in select");
		}
	    }
	}
    }



}

void
send_notification (int sock_send)
{

  struct sockaddr_in sendaddr;
  int length = sizeof (sendaddr);
  bzero ((char *) &sendaddr, sizeof (sendaddr));
  sendaddr.sin_family = AF_INET;
  sendaddr.sin_addr.s_addr = htonl (INADDR_ANY);
  sendaddr.sin_port = htons (PORT);




  for (int i = 0; i < 10; i++)
    {
      printf ("\tPress %d to send notification for Course %d\n\t", i + 1,
	      i + 1);
    }

  int course;

  char c;
  read (0, &c, 1);
  //scanf("%c",&c);
  fflush (stdin);

  scanf ("%d", &course);
  sendaddr.sin_addr.s_addr = inet_addr (course_arr[course - 1]);
  char message[10];
  bzero (message, 10);
  sprintf (message, "%d", course);
  //printf("sending: %s\n", message);

  cnt = sendto (sock_send, message, 10, 0,
		(struct sockaddr *) &sendaddr, length);
  if (cnt == -1)
    perror ("sendto");
  printf ("\n\tMessage sent!\n\n");
  fflush (stdin);
  return;
}

void
receive_notification (int sock_listen)
{

  struct sockaddr_in recvaddr;
  char message[10];
  bzero (message, 10);
  int length = sizeof (recvaddr);

  cnt = recvfrom (sock_listen, message, sizeof (message), 0,
		  (struct sockaddr *) &recvaddr, &length);
  if (cnt < 0)
    {
      perror ("recvfrom");
      exit (1);
    }
  else if (cnt == 0)
    {
      perror ("received return 0 bytes");
      exit (0);
    }
  printf ("\n\t%s: test in course no : \"%s\"\n",
	  inet_ntoa (recvaddr.sin_addr), message);
  return;



}
