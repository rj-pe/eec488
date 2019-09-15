#include "rtttl.h"

Rtttl::Rtttl() {
   char *default_song =
         "JingleBell:d=8,o=5,b=90:32p,a,a,4a,a,a,4a,a,c6,f.,16g,2a,a#,a#,a#.,16a#,a#,a,a.,16a,a,g,g,a,4g,4c6";
   parse_header(default_song);
}

Rtttl::~Rtttl() {
}

void Rtttl::parse_header(char* song) {
   int num = 0;
   int bpm = 60;

   p = song;
   // skip title chars
   while (*p != ':') {
      p++;
   }
   // skip ':'
   p++;
   // extract default duration
   if (*p == 'd') {
      // skip "d="
      p++;
      p++;
      num = 0;
      // convert "number string" to integer
      while (is_digit(*p)) {
         num = (num * 10) + (*p - '0');
         p++;
      }
      default_dur = num;
      // skip ','
      p++;
   }
   // extract default octave
   if (*p == 'o') {
      // skip "o="
      p++;
      p++;
      // convert char to integer
      num = *p - '0';
      p++;
      default_oct = num;
      // skip ','
      p++;
   }
   // extract beats-per-minute (quarter notes per min)
   if (*p == 'b') {
      // skip "b="
      p++;
      p++;
      num = 0;
      // convert "number string" to integer
      while (is_digit(*p)) {
         num = (num * 10) + (*p - '0');
         p++;
      }
      bpm = num;
      // skip ':'
      p++;
   }
   // convert bpm to whole-note interval in ms
   whole_note_interval = (60 * 1000 / bpm) * 4;
   //p points to first note now; record the position
   p_start = p;
}

int Rtttl::parse_next_note() {
   int num = 0;

   // check end of song
   if (p == '\0')
      return (-1);
   // extract optional duration
   num = 0;
   while (is_digit(*p)) {
      num = (num * 10) + (*p - '0');
      p++;
   }
   // update duration
   if (num > 0)
      current_interval = whole_note_interval / num;
   else
      current_interval = whole_note_interval / default_dur;
   // extract note
   switch (*p) {
   case 'c':
      current_note = 0;
      break;
   case 'd':
      current_note = 2;
      break;
   case 'e':
      current_note = 4;
      break;
   case 'f':
      current_note = 5;
      break;
   case 'g':
      current_note = 7;
      break;
   case 'a':
      current_note = 9;
      break;
   case 'b':
      current_note = 11;
      break;
   case 'p':
      current_note = -1;
      break;
   default:
      current_note = -1;
   }
   // skip note
   p++;
   // extract optional '#'
   if (*p == '#') {
      current_note++;
      // skip '#'
      p++;
   }
   // extract optional '.'
   if (*p == '.') {
      current_interval = current_interval + current_interval / 2;
      //skip '.'
      p++;
   }
   // extract optional octave
   if (is_digit(*p)) {
      current_oct = *p - '0';
      // skip octave
      p++;
   } else {
      current_oct = default_oct;
   }
   // end of current note; next char can be ',' or '\0'
   if (*p == ',')
      p++;
   // pointer now for the next note or end of song ('\0')
   return (0);
}

int Rtttl::read_oct() {
   return (current_oct);
}

int Rtttl::read_note() {
   return (current_note);
}

int Rtttl::read_interval() {
   return (current_interval);
}

int Rtttl::is_end() {
   // check whether p points to the ened of string
   return (*p == '\0');
}

void Rtttl::rewind() {
   // set p to the position of first note
   p = p_start;
}

int Rtttl::is_digit(char ch) {
   if (ch >= '0' && ch <= '9')
      return (1);
   else
      return (0);
}

