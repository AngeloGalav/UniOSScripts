/*This is the sample program to notify us for the file creation and file deletion takes place in “/tmp” directory*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <linux/inotify.h>
#include <string.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

int main( )
{
  int length, i = 0;
  int fd;
  int wd;
  char buffer[EVENT_BUF_LEN];
  char command[1000];

  /*creating the INOTIFY instance*/
  fd = inotify_init();

  /*checking for error*/
  if ( fd < 0 ) {
    perror( "inotify_init" );
  }

  /*adding the “/tmp” directory into watch list. Here, the suggestion is to validate the existence of the directory before adding into monitoring list.*/
  wd = inotify_add_watch( fd, "exec", IN_CREATE);


  length = read( fd, buffer, EVENT_BUF_LEN ); 

  /*checking for error*/
  if ( length < 0 ) {
    perror( "read" );
  } 

  /*actually read return the list of change events happens. Here, read the change event one by one and process it accordingly.*/
  while ( i < length ) {    
       struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ]; 
        if ( event->len ) {
            if ( event->mask & IN_CREATE ) {
                if ( event->mask & IN_ISDIR ) {
                printf( "New directory %s created.\n", event->name );
                // strcpy(command, event->name);
                }
                else {// a file has been created
                  strcpy(command, event->name);
                  char * pch;
                  char realCommand[1000]; // i.e echo
                  char path[1000]; // /bin/echo
                  char restOfCommand[1000]; // all file name without echo
                  const char bin[6] = {'/', 'b', 'i', 'n', '/', '\0'};
                  const char deletePath[1000]; // relative path of the file to delete
                  const char pathToFile[6] = {'e', 'x', 'e', 'c', '/', '\0'};

                  pch = strtok (command," "); // is echo
                  strcpy(realCommand, pch); 

                  strcpy(path, bin); 
                  strcat(path, pch);

                  while (pch != NULL) // copy the rest of the command to execute
                    {
                        pch = strtok (NULL, " ");
                        if(pch != NULL)
                        {
                            strcat(restOfCommand, pch);
                            strcat(restOfCommand, " ");
                        }
                    } 
                    
                    strcpy(deletePath, pathToFile); // complete(relative) path to the file we have to delete
                    strcat(deletePath, event->name);

                    printf("delete path%s\n", deletePath);
                    int del = remove(deletePath);
                  if (!del)
                      printf("The file is Deleted successfully");
                  else
                      printf("the file is not Deleted");

                  execl(path, realCommand, restOfCommand); // execute the command
                }
            }
            }
        i += EVENT_SIZE + event->len;

}
/*removing the “/tmp” directory from the watch list.*/
   inotify_rm_watch( fd, wd );

  /*closing the INOTIFY instance*/
   close( fd );

}
    