// afp.ino - Arduino Flash Programmer (for the 39SF010, 39SF020, and 39SF040)
#ifndef _AFP_H_
#define _AFP_H_


// Application version (and its history)
#define AFP_VERSION "0.8" // see also AFP_DATE
// 20260221  0.9  Fixed help 'flash' command
// 20260218  0.8  Fixed bug in elapsed time
// 20260218  0.7  Added 'write' command, unified ERROR msgs, added more @-support
// 20260218  0.6  Erase map improved, added LED to drv for mofifications
// 20260217  0.5  Added 'erase' command
// 20260217  0.4  Improved 'flash' (now has rom io group)
// 20260216  0.3  Added 'read' command
// 20260216  0.2  Added 'flash' command
// 20260215  0.1  Created (help, echo, info command)


// Application date
#define AFP_DATE      "2026 feb 18"


// Application long name
#define AFP_LONGNAME  "Arduino Flash Programmer"


// Application author
#define AFP_AUTHOR    "Maarten Pennings"


// Application banner
#define AFP_BANNER    "\n\n\n\n"\
  "      _       ________  _______\n"\
  "     / \\     |_   __  ||_   __ \\\n"\
  "    / _ \\      | |_ \\_|  | |__) |\n"\
  "   / ___ \\     |  _|     |  ___/\n"\
  " _/ /   \\ \\_  _| |_     _| |_\n"\
  "|____| |____||_____|   |_____|\n"\
  // https://patorjk.com/software/taag/#p=display&f=Varsity&t=AFP


#endif
