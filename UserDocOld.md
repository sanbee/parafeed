User reference manual

## Introduction ##

This document describes the user interface of the offline data
analysis package.  The motivation behind developing this user
interface system is to have a simple, uniform interface for all the
programs that we will write.

All applications using this system, when started, will start an
interactive shell which will present the list of keywords to which the
application is sensitive.  The user can then set/reset the values of
these keywords before starting the application.  The initial values of
any of these keywords can also be supplied as command-line arguments
in the format {\tt 

&lt;KeyWord&gt;

=[

&lt;Val0&gt;

[,

&lt;Val1&gt;

[...]]]}, or loaded from a
file by a single command.

Numeric values of any of the keywords can be any algebraic expression.

## The User Interface Commands ##

Upon startup, applications will present a list of parameters to which it
is sensitive.  This will be in the form of various keywords.  The
values of all these keywords will be empty, unless the keywords was
set on the command-line in the format
`<KeyWord>=[<Val0>[,<Val1>[...]]]`

Following are the commands available from this interactive shell:

```
 cd       : Change working directory
 inp      : To see the various keywords and their values
 go       : To run the application
 gob      : To run the application in background
 help     : This help
 ?        : Information on the type of the keywords
 edit     : Use an editor to (un)set values
 save <F> : Save the values in the file <F>
 load <F> : Load the values from the file <F>
 explain  : Help on the meaning of the various keywords
 quit     : Quit the application
```

These commands form the standard set of shell commands that all
applications will normally understand.  There may be a few extra
commands which may be application dependant and will change from
application to application.  However, the ``help'' command is always
expected to show all the commands that the application can understand.

The keywords can be set/reset from this shell by commands of the type
`<KeyWord>=<Val>`.  For recognizing the names of the keywords in
such commands, the shell uses the minimum match algorithm - minimum
\label{UI\_CMDS}

The user interface of all application programs by default presents a
list of keywords to which the application is sensitive.  This is
presented as a list of variables of an embedded interactive shell.  The
user can set, reset, load, save, etc. the values of these keywords
using the basic shell commands in an interactive shell.  In general,
keywords can be assigned a list of comma-separated values.  The number
of values that a keyword expects can be found via the shell-command
`'?'`.  The value of a keyword can be set by a command of type
`<KeyWord>=[<Val0>[,<Val1>[...]]]`  and can be reset by omitting
the values in such commands (command `<KeyWord>=`).  The
shell-command `help` provides the basic help about the shell
itself and prints following on the screen:

```
   Commands in the interactive mode:

   Use <Key>=<Val1,Val2,..> to set value(s) for a keyword
   Use <Key>=<RETURN> to unset value(s) for a keyword

    inp         : To see the various keywords and their values
    go          : To run the application
    gob         : To run the application in background
    cd          : Change working directory
    help        : This help
    ?           : Information on the type of the keyword
    explain     : Detailed help, optionally of keywords/task [[Key][:Task]]
    save        : Save the values, optionally in a file
    load        : Load the values, optionally from a file
    edit        : Use an editor to (un)set the values
    quit        : Quit the application

   Any other input will be passed to the system shell
```

These commands form the standard set of shell-commands available from
all applications.  However, the user interface libraries provide
mechanisms for application programmers to add application specific
shell-commands as well as alter the behavior of the basic
shell-commands.  Hence, there may be a few extra commands which
may be application dependent and will change from application to
application.  However, the `help` command will automatically
always show all the commands available for a given application.

Following is a short description of the shell commands:

  * Command `inp` displays the current setting of the keywords.

  * Command `go` signifies the end of the interactive session.  After this commands is issued, the values of the keywords set by the users are used by the application internally and the execution of the main program proceeds.

  * Command {{gob}}} is similar to `go` except that it will run the application in the background and print the process ID (PID) number of the background process on the screen (also see section (section **Customization**).

  * Command `cd` changes the current working directory within the interactive shell.

  * Command `'?'` gives some basic information about the type and the number of values the keywords expects.

  * Command `explain` gives as detailed a help about the application and the keywords as the author of the application has cared to write in the help file.  These help files are located in the directory specified by the environment variable `GDOC` (see section \ref{CUSTOMIZATION}).

  * Command `save` saves the current setting of the keywords in a file.  By default, these values are saved in a file named `./<Application Name>.def`.  If a file name is supplied as an argument of this command, the values will be saved in the file.

  * Command `load` is the conjugate of `save` - by default it loads the settings for the keywords from the file `./<Application Name>.def`.  If this file is already present when the application is started, it is loaded automatically. Alternatively, it can load settings from a file provided as an argument to the `load` command.

  * Command `edit` allows user to edit the keyword values in an editor of choice specified by the environment variable `EDITOR` (see section \ref{CUSTOMIZATION}).

  * Command {\tt quit} quits the interactive session without executing the application.

All inputs to the interactive shell, which are not any of the above
mentioned commands or any of the application specific commands, are
passed to the underlying Operating System (OS) user shell.  Hence,
most of the native OS commands shall still be available from this
shell.  (Users however must be aware that certain OS shell commands
like ```{{{setenv}}}'' (for {{{csh}}} users) or ````export`'' (for
`bash/sh` users) will seem to work, but will not have the desired
effect).

As mentioned earlier, the values of the keywords can be a list of
comma-separated values.  The expected number of values will be
reported by the shell-command `'?'` along with the type of the
values, enclosed in '`[`' and '` ] `' pairs.  A blank set of
square brackets (``` []`'') will be printed for keywords which can
accept any number of values.  Some keywords may accept values of mixed
types (e.g., string, floats, integers).  For such keywords, the
reported type will be ```Mixed`''.

Comma is treated as a separator in a list of values for a keyword.  To
suppress its interpretation as a separator, it can to be ``escaped''
using the backslash (`'\'`) before the comma.  For example,
if the value of a keyword `key` has to be set to a string {\tt
``Funny, value with a comma''}, it can be done using

```
                key=Funny\, value with a comma
```

The characters '\', '` [`', '` ]`', '` =`', which
are part of the shell-command syntax, must also be escaped similarly
to be used as part of the values of the keywords.

Numeric values of keywords can be arbitrary mathematical expressions.
Following functions and constants can be used in these expression:

  * Functions

> sin,cos,tan,asin,acos,atan,atan2,sinh,cosh,tanh,exp,ln,log,flog10,sqrt,fabs,floor,ceil,rint

  * Constants
    1. `PI:` value of $\pi$
    1. `C:` speed of light in meters per second.
    1. `R2D,D2R:` multiplicative constants for conversion from Radians to Deg. and vice versa
    1. `H2R,R2H:` multiplicative constants for conversion from Hours to Radians and vice versa
    1. `SOL2SID,SID2SOL:` multiplicative constants for conversion from Solar to Sidreal time and vice versa

Numbers in the expression can be in any of the following
representations:

  * Integer format, real (float) format
  * 1.0E-1 or 1.0e-1 (=0.1)
  * 1.0D-1 or 1.0d-1 (=0.1)
  * 1h10m0.1s: the time format - converts the number to seconds before using it.
  * 1d1'1": The angular format - converts to arc seconds before using it.

> (1d1' = 3660.0" and -1d1' = -3660.0")

Add your content here.  Format your content with:
  * Text in **bold** or _italic_
  * Headings, paragraphs, and lists
  * Automatic links to other wiki pages






