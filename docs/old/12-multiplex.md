```C
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>

#define BUF_SIZE 32

int main (int argc, char **argv) {
    if ( argc < 2 ) {
        puts("too few arguments");
        return 1;
    }

    static fd_set rset;
    FD_ZERO (&rset);
    int i, max = 0, param[argc];

    for ( i = 1; i < argc; ++i ) {
        // Will read stdin on not decimal arguments.
        param[i] = strtol( argv[i], (char**)NULL, 10 );

        if ( param[i] > max ) max = param[i];

        FD_SET( param[i], &rset );
    }

    char buf[BUF_SIZE];
    int done = 0, bytes;

    while ( done < argc - 1 ) {
        if ( select( max + 1, &rset, NULL, NULL, NULL ) == -1 ) {
            puts("bad descriptors");
            return 2;
        }

        for ( i = 1; i < argc; ++i )
            if ( FD_ISSET( param[i], &rset ) ) {
                //do {
                    bytes = read( param[i], buf, BUF_SIZE * sizeof(char) );

                    if ( bytes < 1 ) {
                        FD_CLR( param[i], &rset );
                        ++done;
                    }
                    else
                        write( 1, buf, bytes );

                //} while ( bytes == BUF_SIZE );

                //FD_CLR( param[i], &rset );
                //++done;
            }
    }

    return 0;
}
```


разобрать https://github.com/onestraw/epoll-example/blob/master/epoll.c
