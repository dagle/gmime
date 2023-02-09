Title: Overview

GMime is a C/C++ library which may be used for the creation and parsing of
messages using the Multipurpose Internet Mail Extension (MIME) as defined by
[numerous IETF specifications](https://github.com/jstedfast/gmime/blob/master/RFCs.md).

GMime features an extremely robust high-performance parser designed to be able
to preserve byte-for-byte information allowing developers to re-seralize the
parsed messages back to a stream exactly as the parser found them. It also
features integrated GnuPG and S/MIME v3.2 support.

Built on top of GObject (the object system used by the [GNOME desktop](https://www.gnome.org)), 
many developers should find its API design and memory management very familiar.

This manual documents the interfaces of the GMime
library and has some short notes to help get you up to speed
with using the library.

GMIME is released under the terms of the [GNU Library General Public
License][gnu-lgpl], making it flexible to include in other projects.

GMime depends on the following libraries:

 - **GLib**: a general-purpose utility library, not specific to graphical
   user interfaces. GLib provides many useful data types, macros, type
   conversions, string utilities, file utilities, a main loop abstraction,
   and so on. More information available on the [GLib website][glib].
 - **GObject**: A library that provides a type system, a collection of
   fundamental types including an object type, and a signal system. More
   information available on the [GObject website][gobject].
 - **GIO**: A modern, easy-to-use VFS API including abstractions for files,
   drives, volumes, stream IO, as well as network programming and IPC though
   DBus. More information available on the [GIO website][gio].

GMime can be devided into these groups classes: 

 - Parsers: Parses messages and mime parts
 - Streams: Reads data for different sources
 - Filters: Apply functions to data.
 - DataWrapppers: Use a source of data like a stream
 - Internet Addresses: An email address or groups of addresses.
 - Headers: Different header in a message or mime.
 - MimeParts: Different kinds of Mimes a message or mime can contain
 - Cryptography: Cryptographic functions.

[gnu-lgpl]: https://www.gnu.org/licenses/old-licenses/lgpl-2.1.en.html
[glib]: https://developer.gnome.org/glib/stable/
[gobject]: https://developer.gnome.org/gobject/stable/
[gio]: https://developer.gnome.org/gio/stable/
