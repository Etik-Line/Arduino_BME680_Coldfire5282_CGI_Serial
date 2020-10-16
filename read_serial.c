/*---------------------*/
// Librairie C headers //
/*---------------------*/
#include <stdio.h>
#include <string.h>
#include <time.h>

/*---------------*/
// Linux headers //
/*---------------*/
#include <fcntl.h>   // Contains file controls like O_RDWR
#include <errno.h>   // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h>  // write(), read(), close()

int main(int argc, char *argv[])
{
    int serial_port = open("/dev/ttyS0", O_RDWR | O_NONBLOCK);              //Ouverture du port série, 
                                                                            //à régler en fonction du port utilisé

    struct termios tty;                                                     //Création de la structure termios (termios.h)
                                                                            //nomée tty par convention

    if (tcgetattr(serial_port, &tty) != 0)                                  //Lecture des paramètres existants et gestion des erreurs 
    {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return 1;
    }

    cfmakeraw(&tty);                                                        //Voir cfmakeraw man pour plus d'informations

    cfsetispeed(&tty, B9600);                                               //Baudrate in 9600 bauds
    cfsetospeed(&tty, B9600);                                               //Baudrate out 9600 bauds

    if (tcsetattr(serial_port, TCSANOW, &tty) != 0)                         //Sauvegarde des paramètres tty, et gestion des erreurs
    {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return 1;
    }
    /*------------------------------*/
    // Initialisation des variables //
    /*------------------------------*/
    unsigned char choix;
    char read_buf[10];
    float temp, hum, press, gaz = 0;

    /*---------------------*/
    // Crétaion des Unions //
    /*---------------------*/
    union{                                              //Union température
        float ft;
        char bt[4];
    } u;

    union{                                              //Union humidité
        float fh;
        char bh[4];
    } u1;

    union{                                              //Union pression
        float fp;
        char bp[4];
    } u2;

    union{                                              //Union CO2
        float fg;
        char bg[4];
    } u3;

    switch (*argv[1])
    {
    case 'a':
        tcflush(serial_port, TCIFLUSH);                 //On flush le port série
        memset(read_buf, 0, strlen(read_buf));          //On réinitialise le buffer

        printf("entrée dans le case a : \n");
        choix = 'a';
        write(serial_port, &choix, sizeof(choix));      //Envoi de la sélection à l'Arduino

        usleep(1000 * 1000);                            //Pause 1000 * 1000 µsecondes

        read(serial_port, &read_buf, 4);                // On lit le port série, 4 bytes

        usleep(5000 * 1000);                            //Pause 5000 * 1000 µsecondes, 
                                                        //pour laisser le temps aux données d'arriver

        u.bt[3] = read_buf[0];                          //On copie dans notre union le buffer,
        u.bt[2] = read_buf[1];                          //en commencant par le LSB, qui est arrivé en dernier
        u.bt[1] = read_buf[2];
        u.bt[0] = read_buf[3];
        temp = u.ft;                                    //On accède à l'espace mémoire des bytes arrivés,
                                                        //le float comprends les 4 bytes de l'Union

        printf("température = %.2f", temp);

        printf("\n");
        printf("\n");
        printf("<html>\n");
        printf("<title>\nCGI Station Météo</title>\n");
        printf("<body>\n");
        printf("<h1>Acquisition des donnees</h1>");
        printf("<br>");
        printf("<br>");
        printf("Temperature actuelle : %.2f C\n", temp);
        printf("<br>");
        printf("<br>");
        printf("<br>");
        printf("<br>");
        printf("<a href=\"http://10.7.1.2\">Retour au menu</a>");
        printf("</body>\n");
        printf("</html>\n");
        break;

    case 'b':
        tcflush(serial_port, TCIFLUSH);
        memset(read_buf, 0, strlen(read_buf));

        printf("entrée dans le case b : \n");
        choix = 'b';
        write(serial_port, &choix, sizeof(choix));
        usleep(1000 * 1000);

        read(serial_port, &read_buf, 4);

        usleep(5000 * 1000);

        u1.bh[3] = read_buf[0];
        u1.bh[2] = read_buf[1];
        u1.bh[1] = read_buf[2];
        u1.bh[0] = read_buf[3];
        hum = u1.fh;

        printf("humidité = %.2f", hum);

        printf("\n");
        printf("\n");
        printf("<html>\n");
        printf("<title>\nCGI Station Météo</title>\n");
        printf("<body>\n");
        printf("<h1>Acquisition des donnees</h1>");
        printf("<br>");
        printf("<br>");
        printf("Humidite actuelle : %.2f %%\n", hum);
        printf("<br>");
        printf("<br>");
        printf("<br>");
        printf("<br>");
        printf("<a href=\"http://10.7.1.2\">Retour au menu</a>");
        printf("</body>\n");
        printf("</html>\n");
        ;
        break;

    case 'c':
        tcflush(serial_port, TCIFLUSH);
        memset(read_buf, 0, strlen(read_buf));

        printf("entrée dans le case c : ");
        choix = 'c';
        write(serial_port, &choix, sizeof(choix));

        usleep(1000 * 1000);

        read(serial_port, &read_buf, 4);

        usleep(5000 * 1000);

        u2.bp[3] = read_buf[0];
        u2.bp[2] = read_buf[1];
        u2.bp[1] = read_buf[2];
        u2.bp[0] = read_buf[3];
        press = u2.fp;

        printf("pression = %.2f", press);

        printf("\n");
        printf("\n");
        printf("<html>\n");
        printf("<title>\nCGI Station Météo</title>\n");
        printf("<body>\n");
        printf("<h1>Acquisition des donnees</h1>");
        printf("<br>");
        printf("<br>");
        printf("Pression actuelle : %.2f KPa\n", press);
        printf("<br>");
        printf("<br>");
        printf("<br>");
        printf("<br>");
        printf("<a href=\"http://10.7.1.2\">Retour au menu</a>");
        printf("</body>\n");
        printf("</html>\n");
        break;

    case 'd':
        tcflush(serial_port, TCIFLUSH);
        memset(read_buf, 0, strlen(read_buf));

        printf("entrée dans le case d : ");
        choix = 'd';
        write(serial_port, &choix, sizeof(choix));

        usleep(1000 * 1000);

        read(serial_port, &read_buf, 4);

        usleep(5000 * 1000);

        u3.bg[3] = read_buf[0];
        u3.bg[2] = read_buf[1];
        u3.bg[1] = read_buf[2];
        u3.bg[0] = read_buf[3];
        gaz = u3.fg;

        printf("qualité de l'air = %.2f", gaz);

        printf("\n");
        printf("\n");
        printf("<html>\n");
        printf("<title>\nCGI Station Météo</title>\n");
        printf("<body>\n");
        printf("<h1>Acquisition des donnees</h1>");
        printf("<br>");
        printf("<br>");
        printf("Qualite de l'air actuelle : %.2f \n", gaz);
        printf("<br>");
        printf("<br>");
        printf("<br>");
        printf("<br>");
        printf("<a href=\"http://10.7.1.2\">Retour au menu</a>");
        printf("</body>\n");
        printf("</html>\n");
    }
    close(serial_port);     //Fermeture du port série
    return 0;               //Sucess !!
}
