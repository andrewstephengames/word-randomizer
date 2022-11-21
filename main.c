/*
     word-randomizer
     Andrew Stephen
     2022-04

          Please consult the comments from within the
     printUsage function for usage examples and additional
     information in order to better understand the way the
     program was designed.

          The Program uses size_t boolean-like variables 
     in order to determine the command line flags that are 
     to be used by the user. In order to generate the words,
     by default a `words.txt` list of words is included with
     the source code, including the top 500 most commonly used
     English words, according to 
     https://www.smart-words.org/500-most-commonly-used-english-words.html. 
     A custom input file may be provided as well, but the feature
     is experimental currently as it generates issues to do
     with file opening and closing.

     (TODO)
          The plans are to provide a graphical GTK interface,
     as well as to make the program cross platform. It currently
     compiles under Windows just fine, however some features are
     harder to use due to how the command line is handled there.
*/

#include "headers/main.h"

//FIXME: 100: assertion execution
//FIXME: Segmentation fault when choosing input file in spite of it being created.
//FIXME: text file does not have 500 lines
//TODO: Fix output in custom file with multiple switches

void printUsage ()
{
     printf ("\nUsage:\n");
     printf ("-i: input file\n");
     printf ("-o: output file\n");
     printf ("-a: append mode (requires -o): Appends text to the output file on repeated execution\n");
     printf ("-r: number of words to output\n");
     printf ("-x: output non-colored plain text\nText color in this mode depends on your desktop.\n");
     printf ("Note: The default color mode only works if no output file is specified, or the output file specified is stdout.\n");
     /*
     printf ("Examples:\nGNOME uses a light terminal by default, so the text color is black.\n");
     printf ("KDE, most standalone window managers and the tty: use a black background, so the text color is white.\n");
     printf ("Windows: cmd uses a black background and a white foreground, so the text is white.\n");
     */
     printf ("-h: print usage and exit\n");
     printf ("\nDefaults: -r variable -o stdout\n\n");
     printf ("\nExamples:\n");
     printf ("./main: Prints a variable number of words from 1 to WORD_COUNT from words.txt\n");
     printf ("./main -r 5: Prints 5 random words from words.txt\n");
     printf ("./main -r 15 -o poem.txt: Adds 15 random words to poem.txt\n");
     printf ("On repeated execution, the file is cleared, thus only one sentence is included at one time\n");
     printf ("./main -o poem.txt -a: Appends a variable number of words to poem.txt\n");
     printf ("On repeated execution, the file will contain more sentences\n\n");
}

int main (int argc, char **argv)
{
     srand (clock());
     size_t randSize = 0, inputSignal = 0, outputSignal = 0;
     size_t commentSignal = 0, colorSignal = 1, appendSignal = 0;
     char outputFilename[255] = "", inputFilename[255] = "";
     char nouns[WORD_COUNT][MAX_LEN], verbs[WORD_COUNT][MAX_LEN], words[WORD_COUNT][MAX_LEN];
     FILE *nounFile= fopen ("nouns.txt", "r"), *out, *in, *verbFile = fopen ("verbs.txt", "r");
     while ((argc > 1) && (argv[1][0] == '-'))
     {
          switch (argv[1][1])
          {
               case 'i':
                    //user defined input file
                    //FIXME: stat mechanism
                    inputSignal = 1;
                    struct stat buffer;
                    size_t status;
                    if (stat(strcat("./", argv[3]), &buffer))
                         printf ("%s\n", argv[3]);
                    strcpy (inputFilename, argv[3]);
                    in = fopen (inputFilename, "r");
                    status = stat(strcat ("./", argv[3]), &buffer);
                    assert (status == 0 && "We're sorry, but there is no such file in the current directory. Please check your files or nag the dev to fix it if you consider the Program is at fault.");
                    break;
               case 'o':
                    //output in a user defined file
                    outputSignal = 1;
                    strcpy (outputFilename, argv[2]);
                    break;
               case 'h':
                    //print helpful information and usage
                    printUsage ();
                    exit (EXIT_SUCCESS);
                    break;
               case 'a':
                    //append to existing file on multiple executions
                    appendSignal = 1;
                    break;
               case 'r':
                    //sentence size
                    randSize = atoi (argv[2]);
                    break;
               case 'u':
                    //use words that start with '#' from the input file
                    commentSignal = 1;
                    break;
               case 'x':
                    //exclude colors (default: include colors)
                    colorSignal = 0;
                    break;
               case '-':
                    if (strcmp ("help", argv[1]+2) == 0)
                    {
                         printUsage ();
                         exit (EXIT_SUCCESS);
                    }
                    else 
                    {
                         printf ("Invalid argument!\n");
                         printf ("Type `%s -h` for help.\n", argv[0]);
                         exit (EXIT_FAILURE);
                    }
                    break;
               default:
                    printf ("Invalid argument!\n");
                    printf ("Type `%s -h` for help.\n", argv[0]);
                    exit (EXIT_FAILURE);
                    break;
          }
          ++argv;
          --argc;
     }
     if (inputSignal)
     {
          assert (strlen (inputFilename) == 0 &&
                    "Please input a valid filename!\n");
          assert (strlen (inputFilename) > 255 &&
                    "Please input a filename of smaller size!\n");
          in = fopen (inputFilename, "r");
     }
     for (size_t i = 0; i < WORD_COUNT; i++)
     {
          if (strlen (inputFilename))
               fscanf (in, "%s", words[i]);
          else
          {
               fscanf (nounFile, "%s", nouns[i]);
               nouns[i][0] += 'A' - 'a';
               fscanf (verbFile, "%s", verbs[i]);
          }
     }
     if (strlen (outputFilename) == 0)
          strcpy (outputFilename, "stdout");
     if (outputSignal)
     {
          if (appendSignal)
               out = fopen (outputFilename, "a");
          else 
               out = fopen (outputFilename, "w");
     }
     if (randSize)
     {
          for (size_t i = 0; i < randSize; i++)
               if (strcmp (outputFilename, "stdout") != 0)
               {
                         if (words[rand()%WORD_COUNT][0] == '#')
                         {
                              if (commentSignal)
                                   if (strlen (inputFilename))
                                        fprintf (out, "%s ", words[rand()%WORD_COUNT]+1);
                                   else fprintf (out, "%s %s.\n", nouns[rand()%WORD_COUNT]+1, verbs[rand()%WORD_COUNT]+1);
                              else continue;
                              //printf ("\n%s\n", outputFilename);
                         }
                         else 
                         {
                              if (strlen (inputFilename))
                                   fprintf (out, "%s ", words[rand()%WORD_COUNT]);
                              else fprintf (out, "%s %s.\n ", nouns[rand()%WORD_COUNT], verbs[rand()%WORD_COUNT]);
                              //printf ("\n%s\n", outputFilename);
                         }
               }
               else
               {
                         if (words[rand()%WORD_COUNT][0] == '#')
                         {
                              if (commentSignal)
                              {
                                   if (colorSignal)
                                        printf (colors[rand()%16]);
                                   if (strlen (inputFilename))
                                        fprintf (stdout, "%s ", words[rand()%WORD_COUNT]+1);
                                   else fprintf (stdout, "%s %s.\n ", nouns[rand()%WORD_COUNT]+1, verbs[rand()%WORD_COUNT]+1);
                              }
                              else continue;
                              //printf ("\n%s\n", outputFilename);
                         }
                         else 
                         {
                              if (colorSignal)
                                   printf (colors[rand()%16]);
                              if (strlen (inputFilename))
                                   fprintf (stdout, "%s ", words[rand()%WORD_COUNT]);
                              else fprintf (stdout, "%s %s.\n ", nouns[rand()%WORD_COUNT], verbs[rand()%WORD_COUNT]);
                              //printf ("\n%s\n", outputFilename);
                         }
               }
     }
     else
     {
          for (int i = 0; i < rand()%(WORD_COUNT); i++)
               if (strcmp (outputFilename, "stdout") == 0)
               {
                    if (words[rand()%WORD_COUNT][0] == '#')
                    {
                         if (commentSignal)
                         {
                              if (colorSignal)
                                   printf (colors[rand()%16]);
                              if (strlen(inputFilename))
                                   fprintf (stdout, "%s ", words[rand()%WORD_COUNT]+1);
                              else fprintf (stdout, "%s %s.\n ", nouns[rand()%WORD_COUNT]+1, verbs[rand()%WORD_COUNT]+1);
                         }
                         else continue;
                    }
                    else 
                    {
                         //printf ("\n%s\n", outputFilename);
                         if (colorSignal)
                              printf (colors[rand()%16]);
                         if (strlen (inputFilename))
                              fprintf (stdout, "%s ", words[rand()%WORD_COUNT]);
                         else fprintf (stdout, "%s %s.\n ", nouns[rand()%WORD_COUNT], verbs[rand()%WORD_COUNT]);
                    }
               }
               else 
               {
                    if (words[rand()%WORD_COUNT][0] == '#')
                         if (commentSignal)
                              if (strlen (inputFilename))
                                   fprintf (stdout, "%s ", words[rand()%WORD_COUNT]+1);
                              else fprintf (stdout, "%s %s.\n ", nouns[rand()%WORD_COUNT]+1, verbs[rand()%WORD_COUNT]+1);
                         else continue;
                    else 
                    {
                         //printf ("\n%s\n", outputFilename);
                         if (strlen (inputFilename))
                              fprintf (out, "%s ", words[rand()%WORD_COUNT]);
                         else fprintf (out, "%s %s.\n ", nouns[rand()%WORD_COUNT], verbs[rand()%WORD_COUNT]);
                    }
               }
     }
     if (strcmp (outputFilename, "stdout") == 0)
          fprintf (stdout, "\n");
     else 
     {
          fprintf (out, "\n");
          fclose (out);
     }
     fclose (nounFile);
     fclose (verbFile);
     if (strlen(inputFilename)) fclose (in);
     if (strlen(outputFilename)) fclose (out);
     return 0;
}
