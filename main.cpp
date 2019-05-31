#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include "stdio.h"

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
 
using namespace std;
using namespace cv;

#define PORT "3945"  // the port users will be connecting to

#define BACKLOG 1     // how many pending connections queue will hold
 
CascadeClassifier face_cascade;
string window_name = "Face Detection Demo";
String face_cascade_name = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";
const int BORDER = 8;    /* order between GUI elements to the edge of the image */
 
void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
            return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, const char** argv)
{

    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("Sockets server ready!\nserver: waiting for connections...\n");

    VideoCapture capture;
    Mat frame;
    std::vector<Rect> faces;
    Mat frame_gray;


    if (!face_cascade.load( face_cascade_name ) ) {
        printf("--(!)Error loading training file: haarcascade_frontalface_alt2.xml\n");
        return -1;
    };

    try {
        capture.open(0);
        capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
        capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    }
    catch (cv::Exception &e)
    {
        const char *err_msg = e.what();
        cout << "Exception caught: " << err_msg << endl;
    }

    if ( !capture.isOpened() ) {
        cout << "ERROR: Could not access the camera!" << endl;
        exit(1);
    }

    printf("Pronto para entrar no loop----------------------------------------------\n");

    while(true) {

        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        printf("server: got connection from %s\n", s);

        capture >> frame;

        if (!frame.empty()) {
            printf("Frame is not empty!\n");

            cvtColor(frame, frame_gray, CV_BGR2GRAY);
            equalizeHist(frame_gray, frame_gray);

            face_cascade.detectMultiScale(frame_gray, faces, 1.2, 3, CV_HAAR_DO_CANNY_PRUNING, Size(80, 80));
        } else {
                printf(" --(!) No captured frame");
        }

        if (!fork()) { // this is the child process
            close(sockfd); // child doesn't need the listener
            if (!!faces.size()) {
                if (send(new_fd, "1", 1, 0) == -1)
                    perror("send");
            } else {
                if (send(new_fd, "0", 1, 0) == -1)
                    perror("send");
            }
            close(new_fd);
            exit(0);
        }
        close(new_fd);  // parent doesn't need this

        int c = waitKey(1);
        if ((char)c == 27) {
            break;
        }
    }
    return 0;
}
 
