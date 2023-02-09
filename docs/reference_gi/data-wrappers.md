Title: Data-Wrappers

# Overview of Data Wrappers
Data wrappers are very simple. A GMimeDataWrapper object contains both a stream
and an encoding-type. The encoding-type (such as GMIME\_PART\_ENCODING\_BASE64)
is used by `g_mime_data_wrapper_write_to_stream()` in order to decode the data
into its unencoded form. This means that you, the application programmer, do
not need to worry about decoding the content stream yourself.
