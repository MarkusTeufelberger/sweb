/**
 * @file debug_bochs.cpp
 *
 */

#include "debug_bochs.h"
#include "ports.h"


void writeChar2Bochs( char char2Write )
{
  outportb( 0xE9, char2Write );
}

void writeLine2Bochs( const char * line2Write )
{
  uint8 counter = 0; // the message is cut off at 250 chars 

  while (*line2Write && counter++ < 250)
  {
    writeChar2Bochs( *line2Write );
    ++line2Write;
  }
}
