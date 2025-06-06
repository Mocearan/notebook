# error

---



​		平台移植宏。

```c++
#if defined(GENERATING_DOCUMENTATION) // internal
/// INTERNAL ONLY.
# 	define ASIO_NATIVE_ERROR(e) implementation_defined
/// INTERNAL ONLY.
# 	define ASIO_SOCKET_ERROR(e) implementation_defined
/// INTERNAL ONLY.
# 	define ASIO_NETDB_ERROR(e) implementation_defined
/// INTERNAL ONLY.
# 	define ASIO_GETADDRINFO_ERROR(e) implementation_defined
/// INTERNAL ONLY.
# 	define ASIO_WIN_OR_POSIX(e_win, e_posix) implementation_defined
#elif defined(ASIO_WINDOWS_RUNTIME) // windows runtime
# 	define ASIO_NATIVE_ERROR(e) __HRESULT_FROM_WIN32(e)
# 	define ASIO_SOCKET_ERROR(e) __HRESULT_FROM_WIN32(WSA ## e)
# 	define ASIO_NETDB_ERROR(e) __HRESULT_FROM_WIN32(WSA ## e)
# 	define ASIO_GETADDRINFO_ERROR(e) __HRESULT_FROM_WIN32(WSA ## e)
# 	define ASIO_WIN_OR_POSIX(e_win, e_posix) e_win
#elif defined(ASIO_WINDOWS) || defined(__CYGWIN__) // windows cygwin
# 	define ASIO_NATIVE_ERROR(e) e
# 	define ASIO_SOCKET_ERROR(e) WSA ## e
# 	define ASIO_NETDB_ERROR(e) WSA ## e
# 	define ASIO_GETADDRINFO_ERROR(e) WSA ## e
# 	define ASIO_WIN_OR_POSIX(e_win, e_posix) e_win
#else	// posix
# 	define ASIO_NATIVE_ERROR(e) e
# 	define ASIO_SOCKET_ERROR(e) e
# 	define ASIO_NETDB_ERROR(e) e
# 	define ASIO_GETADDRINFO_ERROR(e) e
# 	define ASIO_WIN_OR_POSIX(e_win, e_posix) e_posix
#endif

```





## basic_errors

```c++
enum basic_errors
{
  /// Permission denied.
  access_denied = ASIO_SOCKET_ERROR(EACCES),

  /// Address family not supported by protocol.
  address_family_not_supported = ASIO_SOCKET_ERROR(EAFNOSUPPORT),

  /// Address already in use.
  address_in_use = ASIO_SOCKET_ERROR(EADDRINUSE),

  /// Transport endpoint is already connected.
  already_connected = ASIO_SOCKET_ERROR(EISCONN),

  /// Operation already in progress.
  already_started = ASIO_SOCKET_ERROR(EALREADY),

  /// Broken pipe.
  broken_pipe = ASIO_WIN_OR_POSIX(
      ASIO_NATIVE_ERROR(ERROR_BROKEN_PIPE),
      ASIO_NATIVE_ERROR(EPIPE)),

  /// A connection has been aborted.
  connection_aborted = ASIO_SOCKET_ERROR(ECONNABORTED),

  /// Connection refused.
  connection_refused = ASIO_SOCKET_ERROR(ECONNREFUSED),

  /// Connection reset by peer.
  connection_reset = ASIO_SOCKET_ERROR(ECONNRESET),

  /// Bad file descriptor.
  bad_descriptor = ASIO_SOCKET_ERROR(EBADF),

  /// Bad address.
  fault = ASIO_SOCKET_ERROR(EFAULT),

  /// No route to host.
  host_unreachable = ASIO_SOCKET_ERROR(EHOSTUNREACH),

  /// Operation now in progress.
  in_progress = ASIO_SOCKET_ERROR(EINPROGRESS),

  /// Interrupted system call.
  interrupted = ASIO_SOCKET_ERROR(EINTR),

  /// Invalid argument.
  invalid_argument = ASIO_SOCKET_ERROR(EINVAL),

  /// Message too long.
  message_size = ASIO_SOCKET_ERROR(EMSGSIZE),

  /// The name was too long.
  name_too_long = ASIO_SOCKET_ERROR(ENAMETOOLONG),

  /// Network is down.
  network_down = ASIO_SOCKET_ERROR(ENETDOWN),

  /// Network dropped connection on reset.
  network_reset = ASIO_SOCKET_ERROR(ENETRESET),

  /// Network is unreachable.
  network_unreachable = ASIO_SOCKET_ERROR(ENETUNREACH),

  /// Too many open files.
  no_descriptors = ASIO_SOCKET_ERROR(EMFILE),

  /// No buffer space available.
  no_buffer_space = ASIO_SOCKET_ERROR(ENOBUFS),

  /// Cannot allocate memory.
  no_memory = ASIO_WIN_OR_POSIX(
      ASIO_NATIVE_ERROR(ERROR_OUTOFMEMORY),
      ASIO_NATIVE_ERROR(ENOMEM)),

  /// Operation not permitted.
  no_permission = ASIO_WIN_OR_POSIX(
      ASIO_NATIVE_ERROR(ERROR_ACCESS_DENIED),
      ASIO_NATIVE_ERROR(EPERM)),

  /// Protocol not available.
  no_protocol_option = ASIO_SOCKET_ERROR(ENOPROTOOPT),

  /// No such device.
  no_such_device = ASIO_WIN_OR_POSIX(
      ASIO_NATIVE_ERROR(ERROR_BAD_UNIT),
      ASIO_NATIVE_ERROR(ENODEV)),

  /// Transport endpoint is not connected.
  not_connected = ASIO_SOCKET_ERROR(ENOTCONN),

  /// Socket operation on non-socket.
  not_socket = ASIO_SOCKET_ERROR(ENOTSOCK),

  /// Operation cancelled.
  operation_aborted = ASIO_WIN_OR_POSIX(
      ASIO_NATIVE_ERROR(ERROR_OPERATION_ABORTED),
      ASIO_NATIVE_ERROR(ECANCELED)),

  /// Operation not supported.
  operation_not_supported = ASIO_SOCKET_ERROR(EOPNOTSUPP),

  /// Cannot send after transport endpoint shutdown.
  shut_down = ASIO_SOCKET_ERROR(ESHUTDOWN),

  /// Connection timed out.
  timed_out = ASIO_SOCKET_ERROR(ETIMEDOUT),

  /// Resource temporarily unavailable.
  try_again = ASIO_WIN_OR_POSIX(
      ASIO_NATIVE_ERROR(ERROR_RETRY),
      ASIO_NATIVE_ERROR(EAGAIN)),

  /// The socket is marked non-blocking and the requested operation would block.
  would_block = ASIO_SOCKET_ERROR(EWOULDBLOCK)
};
```



### operation_aborted

​		操作被取消。

```c++
/// Operation cancelled.
operation_aborted = ASIO_WIN_OR_POSIX(
    ASIO_NATIVE_ERROR(ERROR_OPERATION_ABORTED),
    ASIO_NATIVE_ERROR(ECANCELED)),
```

```c++
timer.async_wait([this](const auto& error) {
   if(error == asio::error::operation_aborted)
       std::cout << "canceled\n";
    else 
        this->print();
});
```





## netdb_errors

```c++
enum netdb_errors
{
  /// Host not found (authoritative).
  host_not_found = ASIO_NETDB_ERROR(HOST_NOT_FOUND),

  /// Host not found (non-authoritative).
  host_not_found_try_again = ASIO_NETDB_ERROR(TRY_AGAIN),

  /// The query is valid but does not have associated address data.
  no_data = ASIO_NETDB_ERROR(NO_DATA),

  /// A non-recoverable error occurred.
  no_recovery = ASIO_NETDB_ERROR(NO_RECOVERY)
};
```



## addrinfo_errors

```c++
enum addrinfo_errors
{
  /// The service is not supported for the given socket type.
  service_not_found = ASIO_WIN_OR_POSIX(
      ASIO_NATIVE_ERROR(WSATYPE_NOT_FOUND),
      ASIO_GETADDRINFO_ERROR(EAI_SERVICE)),

  /// The socket type is not supported.
  socket_type_not_supported = ASIO_WIN_OR_POSIX(
      ASIO_NATIVE_ERROR(WSAESOCKTNOSUPPORT),
      ASIO_GETADDRINFO_ERROR(EAI_SOCKTYPE))
};
```



## misc_errors

```c++
enum misc_errors
{
  /// Already open.
  already_open = 1,

  /// End of file or stream.
  eof,

  /// Element not found.
  not_found,

  /// The descriptor cannot fit into the select system call's fd_set.
  fd_set_failure
};

```

