#ifndef _RTTTL_H_INCLUDED
#define _RTTTL_H_INCLUDED

/**
 * process song in rtttl format
 *  - use a pointer to keep track of the position of current note
 *  - procedure: process header; advance/parse one note at a time
 *
 * @note rtttl ring tones tend to use high octave;
 *       converting them to oct 3 or 4 may get better effect
 */
class Rtttl {
public:
   int default_dur;
   int default_oct;
   int whole_note_interval;
   int current_oct;
   int current_note;
   int current_interval;

   /* methods */
   /**
    * Constructor.
    *  - process header
    *  - use a pointer to keep track of the curret position
    *  - parse one note at a time
    *
    * @note parse the header of default song
    */
   Rtttl();
   ~Rtttl();       // not used

   /**
    * process the header of an rtttl song
    *
    * @param song pointer to a string in rtttl format
    * @note extract default octave/note/duration and beats per minute
    * @note set the pointer to the first note of the song
    */
   void parse_header(char* song);

   /**
    * advance to next note; process the note;
    * update current_oct/note/duration
    *
    */
   int parse_next_note();

   /**
    * retrieve octave of current note
    * @return current octave
    *
    */
   int read_oct();

   /**
    * retrieve note of current note
    * @return current note (0 to 11 for C, C#, ... B)
    * @note -1 for silent
    *
    */
   int read_note();

   /**
    * retrieve interval of current note
    * @return interval of current note in term of ms
    *
    */
   int read_interval();

   /**
    * set pointer to first note (i.e., rewind the song)
    *
    */
   void rewind();

   /**
    * check whether reaching the end of song
    *
    * @return 1 if reaching end; 0 otherwise
    */
   int is_end();

private:
   char* p;                // current point position of rtttl string
   char* p_start;          //start position of 1st note
   int is_digit(char ch);
};

#endif  //_RTTTL_H_INCLUDED

