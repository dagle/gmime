Title: Tutorial

# Intro

GMime is a library for parsing and creating messages using the Multipurpose
Internet Mail Extension (MIME) format. It is licensed under the
[LGPL-2.1](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.txt)  so you
are free to develop your Free Software applications using GMime without
having to spend anything for licenses or royalties.

GMime is essentially an object-oriented application programmers interface
(API). Although written completely in C, it is implemented using the idea of
classes and callback functions (pointers to functions).
    
GMime is built upon another library called GLib which also
serves as the foundation for such libraries as the GIMP ToolKit
(Gtk+). GLib is mostly a portability layer but also contains
additional functionality such as hash tables, linked lists, etc. For
more information on GLib, you should see the API reference at [glib](https://docs.gtk.org/glib/)

# Install

The first thing you need to do, of course, is download the GMime source and
install it. You can always get the latest version from
[github]("https://github.com/jstedfast/gmime"). GMime uses meson for building.
Once untar'd, type **meson configure** to see a list of options.

For more information see [meson](https://mesonbuild.com/Quick-guide.html#compiling-a-meson-project)

# Basics

The first thing you need to learn how to do is compile your program with the
proper compiler flags so that your program will include the correct GMime
headers and linker flags.

To compile and link a simple program, you'll want to do the following:
```bash 
  gcc -g -Wall -o simple simple.c `pkg-config --cflags --libs gmime-3.0`
```


## GMimeStream

Before we get too deep into using GMime, it is important to understand how to
use the underlying I/O classes since GMime is so very heavily dependent upon
them.

If you've looked at the API at all already, you will have probably noticed that
the stream functions work very much like those of the standard low-level UNIX
I/O functions (those that use file descriptors) but with a few extras taken
from the higher-level Standard C I/O API.

Let's take a moment to think back to our early days of programming where
we learned how to write "Hello World!" on the console:
```c
#include <stdio.h>

int main (int argc, char **argv)
{
	fprintf (stdout, "Hello World!\n");
	fflush (stdout);
	
	return 0;
}
```

Everyone should recognize what that program does. The above program, rewritten
to use GMime's stream classes would look something like this:

```c
#include <stdio.h>
#include <gmime/gmime.h>

int main (int argc, char **argv)
{
	GMimeStream *stream;
	
	/* initialize GMime */
	g_mime_init ();
	
	/* create a stream around stdout */
	stream = g_mime_stream_file_new (stdout);
	
	/* 'printf' */
	g_mime_stream_printf (stream, "Hello World!\n");
	
	/* flush stdout */
	g_mime_stream_flush (stream);
	
	/* free/close the stream */
	g_object_unref (stream);
	
	return 0;
}
```

Hopefully, the only thing that may be new to you in either of the above
examples is the flushing of the stream after writing to it. Most likely, in
both examples, it is an unneeded call, however it is there for completeness and
you should probably get into the habit of flushing a stream after you've
finished writing to it. Like `fflush()`, `g_mime_stream_flush()` will flush any
write-buffers that the previous write-calls may have left.

The first function called in the second example is `g_mime_init()`. If you haven't
guessed, `g_mime_init()` initializes the GMime library.

The only other line that should need explaining might be:
```c
stream = g_mime_stream_file_new (stdout);
```

This line creates a new object of type GMimeStreamFile which takes a `FILE*`
argument. Once the GMimeStreamFile is created, it takes ownership of the `FILE*`
so be careful if you want to be able to ever use that `FILE*` handle again later
in your program or if you do not wish for it to be closed when the
GMimeStreamFile is closed later.

One way of working around this is to do something like the
following example:

```c
#include <stdio.h>
#include <unistd.h>
#include <gmime/gmime.h>

int main (int argc, char **argv)
{
	GMimeStream *stream;
	
	/* initialize GMime */
	g_mime_init ();
	
	/* create a stream around stdout */
	stream = g_mime_stream_fs_new (dup (fileno (stdout)));
	
	/* 'printf' */
	g_mime_stream_printf (stream, "Hello World!\n");
	
	/* flush stdout */
	g_mime_stream_flush (stream);
	
	/* free/close the stream */
	g_object_unref (stream);
	
	return 0;
} 
```
      
Here we have made a duplicate copy of stdout to give to `g_mime_stream_fs_new()`.
GMimeStreamFs is the second type of stream meant for basic I/O, but instead of
using a `FILE*` handle, it instead uses an integer file descriptor. The fileno()
function returns the integer file descriptor for a given `FILE*` handle. The
`dup()` function makes a duplicate of the file descriptor passed to it. More
information can be read about these 2 functions by using man on your local UNIX
system or by reading the Reference Manual for your libc.

There are also some functions to tell GMimeStreamFile, GMimeStreamFs and
GMimeStreamMem that they are not the owners of the backend storage and so when
they are destroyed, they should not close the file or free the memory buffer
(respectively). These functions are:

```c
void g_mime_stream_file_set_owner (GMimeStreamFile *stream, gboolean owner);
void g_mime_stream_fs_set_owner (GMimeStreamFs *stream, gboolean owner);
void g_mime_stream_mem_set_owner (GMimeStreamMem *stream, gboolean owner);
```  

Next, let's examine some of the other stream
functions.


```c
g_mime_stream_eos (stream);
```

This function is useful for finding out if the
End-Of-Stream has been reached. This is similar in functionality
to Standard C's feof() function.

```c
g_mime_stream_reset (stream);
```

This function will reset the state of a stream. Usually
this only means 'rewinding' to the beginning of the file. For
more complex streams, such as GMimeStreamFilter, however, this
will also reset the state of all of the filters that have been
attached to it (more on this later).

```c
g_mime_stream_length (stream);
```

This function will return the length of the stream if
known, or -1 otherwise. For the most part, this function should
be avoided unless you absolutely need to know the stream length
and there is no other way to get it. The reason to avoid using
it is that it may be inaccurate if any filters are to be applied
as well as possibly being slow depending on the underlying
storage device.

```c
g_mime_stream_substream (stream, start, end);
```

This function will return a substream of the original
stream, where the beginning of the new substream is the start
offset and the end is the end offset. These start and end
offsets MUST be within the bounds of the original
stream. Substreams can be useful if you want to only allow
reading and writing to a subsection of the original
stream.

```c
g_mime_stream_read (stream, buf, n);
```

Like POSIX read(), this function will
try to read n bytes from the stream
into buf, but be warned that it is not
guaranteed to read the full requested buffer size if that much
data is not currently available.

```c
g_mime_stream_write (stream, buf, n);
```

Like POSIX `write()` and standard C's `fwrite()`, this function will write a buffer
of the specified length to the underlying stream. However, unlike the POSIX
write() function, it will only fail if an irrecoverable error has occurred and
so it is not necessary to loop write attempts until the entire buffer is
written.

```c
g_mime_stream_seek (stream, offset, GMIME_STREAM_SEEK_SET);
g_mime_stream_seek (stream, offset, GMIME_STREAM_SEEK_CUR);
g_mime_stream_seek (stream, offset, GMIME_STREAM_SEEK_END);
```

This function works exactly like the POSIX `lseek()` or standard C's `fseek()`
functions.

## Stream Class Overview
      
There are a number of stream classes included with GMime, but we are only going
to go over the more widely useful stream classes. You should be able to figure
out the others on your own.

We've already seen GMimeStreamFile and GMimeStreamFs in action in the previous
chapter, so let's skip them and start with GMimeStreamMem.

GMimeStremMem is a stream abstraction that reads and writes to a memory buffer.
Like any other stream, the basic stream functions (read, write, seek,
substream, eos, etc) apply here as well. Internally, GMimeStreamMem uses the
GLib GByteArray structure for storage so you may want to read up on that.

There are several ways to instantiate a GMimeStreamMem object. You will
probably use `g_mime_stream_mem_new()` most of the time. There may be times,
however, when you will already have a memory buffer that you'd like to use as a
stream. There are several ways to create a GMimeStreamMem object to use this
buffer (or a copy of it).

The first is `g_mime_stream_mem_new_with_byte_array()`. This assumes that you are
already using a GByteArray and want to use it as a stream. As explained in the
previous chapter about GMimeStreamFile and ownership, the same applies here.
When the GMimeStreamMem is destroyed, so is the GByteArray structure and the
memory buffer it contained. To get around this, create a new GMimeStreamMem
object using `g_mime_stream_mem_new()` and then use
`g_mime_stream_mem_set_byte_array()` to set the GByteArray as the memory buffer.
This will make it so that GMimeStreamMem does not own the GByteArray, so when
the GMimeStremMem object is destroyed, the GByteArray will remain.

Also at your disposal for creating GMimeStreamMem objects with an initial
buffer is `g_mime_stream_mem_new_with_buffer()`. This function, however, will
duplicate the buffer passed to it so if you have memory quotas you are trying
to keep, you may wish to find a way to use one of the above methods.

That pretty much sums up how to use GMimeStreamMem. The next most widely used
stream class is probably GMimeStreamBuffer. This stream class actually wraps
another stream object adding additional functionality such as read and write
buffering and a few additional read methods.

As you may or may not know, buffering reads and writes is a great way to
improve I/O performance in applications. The time it takes to do a lot of small
reads and writes accumulates fast.

When using a GMimeStreamBuffer in `GMIME_STREAM_BUFFER_BLOCK_READ` mode, a block
of 4K (4096 bytes) will be read into an intermediate buffer. Each time your
application performs a read on this GMimeStreamBuffer stream, a chunk of that
intermediate buffer will be copied to your read buffer until all 4K have been
read, at which point GMimeStreamBuffer will pre-scan the next 4K and so on.

Similarly, using mode `GMIME_STREAM_BUFFER_BLOCK_WRITE` will copy each of your
application write-buffers into an intermediate 4K buffer. When that 4K buffer
fills up, it will be flushed to the underlying stream. You may also use
`g_mime_stream_flush()` to force the intermediate buffer to be written to the
underlying stream.

Note that the intermediate buffer size is 4096 bytes. You should be aware that
if you will mostly be reading and writing blocks of larger than 4K, it is
probably best to avoid using GMimeStreamBuffer as it will not likely gain
you any performance and may decrease performance instead.

GMimeStreamBuffer also adds 2 convenience functions for reading. While they
will both work with any stream class, they are obviously much faster if used
with a GMimeStreamBuffer in mode `GMIME_STREAM_BUFFER_BLOCK_READ`. These
functions are:

```c
ssize_t g_mime_stream_buffer_gets (GMimeStream *stream, char *buf, size_t max);

void    g_mime_stream_buffer_readln (GMimeStream *stream, GByteArray *buffer);
```


The first function is similar to Standard C's
fgets() function (although the arguments are
in a slightly different order). It reads up to the first
max - 1 bytes, stopping after a
\n character, if found. buf
will always be nul-terminated.

The second function,
`g_mime_stream_buffer_readln()`, has no
Standard C equivalent that I am aware of, but you should get the
idea of what it does based on the function name (I hope). It
reads exactly one (1) line (including the \n
character) and appends it to the end of
buffer.

The last stream class you really need to know (and the
last one I have the patience to explain) is
GMimeStreamFilter. This is a special stream class which you can
attach GMimeFilters to so that reading/writing to this stream
will automagically convert the stream from one form to
another. GMime uses this stream internally for converting base64
encoded attachments into their raw form and vice versa.


As previously mentioned in the last chapter concerning
`g_mime_stream_reset()`, resetting a
GMimeStreamFilter stream will also reset all of the filters
applied.


A great example usage of GMimeStreamFilter can be found in
the src/uuencode.c source file found in the
source distribution. Here's a clip of that source file
illustrating how to use stream filters:


```c
GMimeStream *istream, *ostream, *fstream;
GMimeFilter *filter;
int fd;

// ...

if (g_mime_stream_printf (ostream, "begin %.3o %s\n", st.st_mode & 0777, name) == -1) {
	fprintf (stderr, "%s: %s\n", progname, strerror (errno));
	g_object_unref (ostream);
	exit (1);
}

istream = g_mime_stream_fs_new (fd);

fstream = g_mime_stream_filter_new (ostream);

filter = g_mime_filter_basic_new (GMIME_CONTENT_ENCODING_UUENCODE, TRUE);
g_mime_stream_filter_add ((GMimeStreamFilter *) fstream, filter);
g_object_unref (filter);

if (g_mime_stream_write_to_stream (istream, fstream) == -1) {
	fprintf (stderr, "%s: %s\n", progname, strerror (errno));
	g_object_unref (fstream);
	g_object_unref (istream);
	g_object_unref (ostream);
	exit (1);
}

g_mime_stream_flush (fstream);
g_object_unref (fstream);
g_object_unref (istream);

if (g_mime_stream_write_string (ostream, "end\n") == -1) {
	fprintf (stderr, "%s: %s\n", progname, strerror (errno));
	g_object_unref (ostream);
	exit (1);
}

g_object_unref (ostream);
```

The above snippet of code will read the contents of the input stream (istream)
and write it to our output stream (ostream), but only after it has passed
through our filter-stream (fstream). The filter attached to fstream is one of
the basic MIME filters that encodes data in the traditional UUCP format. You
have probably run a program to do this many times in the past using the UNIX
command uuencode. Never thought writing a replacement for uuencode could be so
easy, did you? Well, it is. And not only is it that easy, but it also runs
faster than the uuencode shipped with GNU Sharutils (at least up to and
including the 4.2.1 release).

## Filter Class Overview

GMime comes pre-bundled with a number of stream filters for your convenience
and more may be added in the future. For now, let's breeze through a summary of
some of the more important ones:

GMimeFilterBasic is used quite a lot internally in GMime for encoding and
decoding the content of MIME parts. This class contains a mode for encoding and
decoding each of Base64, Quoted-Printable, and Uuencode.

If you are interested in converting between charsets for your users, you will
likely want to become familiar with GMimeFilterCharset which provides a
convenient way to convert text streams of one charset into another charset.

GMimeFilterUnix2Dos will likely become very useful to you if you are
implementing any internet standards or DOS/UNIX compatibility. This filter is
meant for converting line endings from the traditional UNIX sequence (LF) to
the internet standard (and DOS) sequence. There's also a GMimeFilterDos2Unix to
perform the opposite conversion.

GMimeFilterFrom is one you will likely need to use if ever you need to write to
an mbox-formatted mail spool. At present, it has 2 modes:
`GMIME_FILTER_FROM_MODE_ESCAPE` and `GMIME_FILTER_FROM_MODE_ARMOR`. If you are
writing to an mbox-formatted spool, you will always want to use the ESCAPE mode
which will escape lines beginning with "From " by prepending a '\>' character,
resulting in "\>From ". The other mode might come in handy if you are
implementing a multipart/signed method where you are quoted-printable encoding
a text stream and need to special-case From-lines in order to protect against
UNIX systems which will alter the message when writing it to an mbox file such
as the previously mentioned filter mode. The result is something like "=46rom "
which prevents the need to prepend a '\>' character when the message arrives at
a UNIX machine.

Also included are: GMimeFilterBest (which will likely not concern you),
GMimeFilterEnriched (which will convert text/enriched and/or text/rtf to
text/html), and GMimeFilterHTML which will convert text/plain into text/html
with options to wrap strings that appear to be hyperlinks with appropriate
&lt;a href=...&gt; tags; GMimeFilterStrip (again, likely this won't concern
you), and finally GMimeFilterYenc which will encode or decode the YEncode
encoding.

For an example on how to use filters, please see the end of the previous
chapter where it talks about GMimeStreamFilter and provides a snippet from
src/uuencode.c

Note: Since it may be non-obvious, filters are applied to a stream in the same
order that they are added to the GMimeFilterStream. This means that if you add
a base64 encode filter and then add a CRLF filter, the stream will first be
base64 encoded and then the end-of-line formatting will be canonicalised to
CRLF.

# MIME MIME and more MIME
## GMimePart

Since most people seem to want to know how to "save an attachment", let's start
there.

Given a GMimePart object, the first step to saving an attachment is probably
going to be figuring out what the filename is. To do that, you'll likely want
to do something like:

```c
static void
save_attachment (GMimePart *part)
{
	GMimeDataWrapper *content;
	const char *filename;
	GMimeStream *stream;
	int fd;

	filename = g_mime_part_get_filename (part);
// ...
```

The `g_mime_part_get_filename()` function
will first check for a filename parameter in
the Content-Disposition header. If that parameter exists,
it will return the value as the filename. However, if that does
not exist, it will fall back to checking for the
name parameter sometimes found in the
Content-Type header and return that value if it exists
(Microsoft Outlook, for example, will set the name parameter,
but will not set the filename parameter). If neither of these
param values are found, it will simply return
NULL.

Now that you've got a filename for the MIME part (well,
assuming that it isn't NULL - in which case you'll have to
prompt the user or make up your own filename or something), the
next step is to open an output stream and write the MIME part's
content to disk:


```c
// ...
	if ((fd = open (filename, O_CREAT | O_WRONLY, 0666)) == -1)
		return;

	stream = g_mime_stream_fs_new (fd);

	content = g_mime_part_get_content_object (part);
	g_mime_data_wrapper_write_to_stream (content, stream);
	g_mime_stream_flush (stream);
	g_object_unref (stream);
}
```

In order to get the content of a MIME part (eg. the body of a part, not
including the headers), you'll want to use `g_mime_part_get_content_object()`. To
write the content object to a stream, you can use
`g_mime_data_wrapper_write_to_stream()`. On fail, this function will return -1,
otherwise it will return some positive value which will usually equate to the
number of bytes written (but not always, due to filter transformations);
generally it's a good idea to not rely on the returned value for anything other
than error-checking.

