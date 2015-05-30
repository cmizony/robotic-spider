#include "terminal.h"

/**
 * permet de passer le terminal en mdoe raw
 * autorise la lecture des frape clavier sans buffer
 * Linux Only
 **/
int mode_raw( int activer )
{
  static struct termios cooked;
  static int raw_actif = 0;
  if ( raw_actif == activer )
    return activer;
  if ( activer )
    {
      struct termios raw;
      tcgetattr(STDIN_FILENO, &cooked);
      raw = cooked;
      cfmakeraw(&raw);
      tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    }
  else
    tcsetattr( STDIN_FILENO, TCSANOW, &cooked);
  raw_actif = activer;
  return activer;
}
