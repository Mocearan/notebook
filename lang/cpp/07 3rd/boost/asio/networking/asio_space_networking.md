# asio space



## free func

### buffer

#### read

​		The `read` function is a composed operation that attempt to reads a certain amount of data from a stream before returning.

#### write

​		The `write` function is a composed operation that writes a certain amount of data to a stream before returning.

```c++
// 参数
// SyncWriteStream & s
//		The stream to which the data is to be written. The type must support the SyncWriteStream concept.
// const ConstBufferSequence & buffers
//		One or more buffers containing the data to be written. The sum of the buffer sizes indicates the maximum number of bytes to write to the stream.
// ec
//		Set to indicate what error occurred, if any.


// Exceptions
// asio::system_error
//		Thrown on failure.

// Return Value
//		The number of bytes transferred.
```



- Write all of the supplied data to a stream before returning.

   	write a certain number of bytes of data to a stream. The call will block until one of the following conditions is true:

  - All of the data in the supplied buffers has been written. That is, the bytes transferred is equal to the sum of the buffer sizes.
  - An error occurred.

  

  ​		This operation is implemented in terms of zero or more calls to the**stream's write_some** function.

```c++
template<typename SyncWriteStream,typename ConstBufferSequence>
std::size_t write(
    SyncWriteStream & s,
    const ConstBufferSequence & buffers,
    typename constraint< is_const_buffer_sequence< ConstBufferSequence >::value >::type  = 0);

template<typename SyncWriteStream,typename ConstBufferSequence>
std::size_t write(
    SyncWriteStream & s,
    const ConstBufferSequence & buffers,
    asio::error_code & ec,
    typename constraint< is_const_buffer_sequence< ConstBufferSequence >::value >::type  = 0);
```

> ```c++
> // example
> asio::write(s, asio::buffer(data, size));
> asio::write(s, asio::buffer(data, size), ec);
> 
> // This overload is equivalent to calling:
> asio::write(s, buffers,asio::transfer_all());
> asio::write(s, buffers,asio::transfer_all(), ec);
> ```



#### async_read

​		`async_read`是一个组合的异步操作，在完成之前异步的从流中读取一定数量的数据。





#### async_write

​		`async_write`函数是一个组合的异步操作，它在完成之前将一定数量的数据写入流。

```c++
// 参数
// AsyncWriteStream & s
// 		The stream to which the data is to be written. The type must support the AsyncWriteStream concept.
// const ConstBufferSequence & buffers
//		One or more buffers containing the data to be written. Although the buffers object may be copied as necessary, ownership of the underlying memory blocks is retained by the caller, which must guarantee that they remain valid until the handler is called.
// WriteHandler && handler
//		The handler to be called when the write operation completes. Copies will be made of the handler as required. The function signature of the handler must be:
void handler(
  const asio::error_code& error, // Result of operation.

  std::size_t bytes_transferred           // Number of bytes written from the
                                          // buffers. If an error occurred,
                                          // this will be less than the sum
                                          // of the buffer sizes.
);
// 无论异步操作是否立即完成，都不会从这个函数中调用处理程序。 在立即完成时，处理程序的调用将以与使用post相同的方式执行。  

```



- start an asynchronous operation to write all of the supplied to a stream

  ​	将一定数量的数据异步写入流，函数调用总是立即返回。异步操作将继续，直到：

  - buffer中的所有数据都已写入（传输的字节数等于缓冲区大小的总和）
  - 发生错误

  ```c++
  template<typename AsyncWriteStream,typename ConstBufferSequence,typename WriteHandler = DEFAULT>
  DEDUCED async_write(
      AsyncWriteStream & s,
      const ConstBufferSequence & buffers,
      WriteHandler && handler = DEFAULT,
      typename constraint< is_const_buffer_sequence< ConstBufferSequence >::value >::type  = 0);
  ```

  ​		对流的`async_write_some`函数的0到多次调用来实现，称为组合操作。

  ​		程序必须确保此操作完成之前不执行任何其他写操作(`async_write, 流的async_write_some`或其他任何执行写的组合操作)。

  ​		This asynchronous operation supports cancellation for the following [`cancellation_type`](https://think-async.com/Asio/asio-1.20.0/doc/asio/reference/cancellation_type.html) values:

  - `cancellation_type::terminal`
  - `cancellation_type::partial`

  if they are also supported by the `AsyncWriteStream` type's `async_write_some` operation.





### networking

#### connect

​		此函数尝试将套接字连接到一系列端点中的一个。 它通过重复调用套接字的connect成员函数来实现这一点，对序列中的每个端点调用一次，直到成功建立连接。  

```c++
// 参数
// basic_socket< Protocol, Executor > & s
//		The socket to be connected. If the socket is already open, it will be closed.

// const EndpointSequence & endpoints
//		A sequence of endpoints.

// Iterator begin
//		An iterator pointing to the start of a sequence of endpoints.
// Iterator end
//		An iterator pointing to the end of a sequence of endpoints.

// ConnectCondition connect_condition
//		A function object that is called prior to each connection attempt. The signature of the function object must be:
	bool  // The function object should return true if the next endpoint should be tried, and false if it should be skipped.
    connect_condition(
        const asio::error_code& ec, //contains the result from the most recent connect operation. Before the first connection attempt
                                 	// , ec is always set to indicate success.
        const typename Protocol::endpoint& next); // The next parameter is the next endpoint to be tried. 

// asio::error_code & ec
//		Set to indicate what error occurred, if any. If the sequence is empty, set to asio::error::not_found. Otherwise, contains the error from the last connection attempt.
```

```c++
// 返回值
// Iterator 
//		On success, an iterator denoting the successfully connected endpoint. Otherwise, the end iterator.
// Protocol::endpoint
//		The successfully connected endpoint.
```

```c++
// 异常
// asio::system_error
//		Thrown on failure. If the sequence is empty, the associated error_code is 
//				asio::error::not_found. 
//		otherwise, contains the error from the last connection attempt.
```



- endpoint of sequence.

  ```c++
  template<typename Protocol, typename Executor, typename EndpointSequence>
  Protocol::endpoint connect(
      basic_socket< Protocol, Executor > & s,
      const EndpointSequence & endpoints,
      typename constraint< is_endpoint_sequence< EndpointSequence >::value >::type  = 0);
  ```

  ```c++
  template< typename Protocol, typename Executor, typename EndpointSequence>
  Protocol::endpoint connect(
      basic_socket< Protocol, Executor > & s,
      const EndpointSequence & endpoints,
      asio::error_code & ec,
      typename constraint< is_endpoint_sequence< EndpointSequence >::value >::type  = 0);
  ```

  ```c++
  template<typename Protocol,typename Executor,typename EndpointSequence,typename ConnectCondition>
  Protocol::endpoint connect(
      basic_socket< Protocol, Executor > & s,
      const EndpointSequence & endpoints,
      ConnectCondition connect_condition,
      typename constraint< is_endpoint_sequence< EndpointSequence >::value >::type  = 0);
  ```

  ```c++
  template<typename Protocol,typename Executor,typename EndpointSequence,typename ConnectCondition>
  Protocol::endpoint connect(
      basic_socket< Protocol, Executor > & s,
      const EndpointSequence & endpoints,
      ConnectCondition connect_condition,
      asio::error_code & ec,
      typename constraint< is_endpoint_sequence< EndpointSequence >::value >::type  = 0);
  ```

  

- begin endpoint iterator of sequence.  Deprecated: Use range overload.

  ​		这个重载隐式的认为默认构造的`Iterator`表示sequence的`end`，这对`asio::ip::tcp::resolver::iterator`是可用的。

  ```c++
  template<typename Protocol,typename Executor,typename Iterator>
  Iterator connect(
      basic_socket< Protocol, Executor > & s,
      Iterator begin,
      typename constraint<!is_endpoint_sequence< Iterator >::value >::type  = 0);
  ```

  ```c++
  template<typename Protocol,typename Executor,typename Iterator>
  Iterator connect(
      basic_socket< Protocol, Executor > & s,
      Iterator begin,
      asio::error_code & ec,
      typename constraint<!is_endpoint_sequence< Iterator >::value >::type  = 0);
  ```

  ```c++
  template<typename Protocol,typename Executor,typename Iterator,typename ConnectCondition>
  Iterator connect(
      basic_socket< Protocol, Executor > & s,
      Iterator begin,
      ConnectCondition connect_condition,
      typename constraint<!is_endpoint_sequence< Iterator >::value >::type  = 0);
  ```

  ```c++
  template<typename Protocol,typename Executor,typename Iterator,typename ConnectCondition>
  Iterator connect(
      basic_socket< Protocol, Executor > & s,
      Iterator begin,
      ConnectCondition connect_condition,
      asio::error_code & ec,
      typename constraint<!is_endpoint_sequence< Iterator >::value >::type  = 0);
  ```

- (range) begin/end endpoint iterator of sequence

  ```c++
  template<typename Protocol,typename Executor,typename Iterator>
  Iterator connect(
      basic_socket< Protocol, Executor > & s,
      Iterator begin,
      Iterator end);
  ```

  ```c++
  template<typename Protocol,typename Executor,typename Iterator>
  Iterator connect(
      basic_socket< Protocol, Executor > & s,
      Iterator begin,
      Iterator end,
      asio::error_code & ec);
  ```

  ```c++
  template<typename Protocol,typename Executor,typename Iterator,typename ConnectCondition>
  Iterator connect(
      basic_socket< Protocol, Executor > & s,
      Iterator begin,
      Iterator end,
      ConnectCondition connect_condition);
  ```

  ```c++
  template<typename Protocol,typename Executor,typename Iterator,typename ConnectCondition>
  Iterator connect(
      basic_socket< Protocol, Executor > & s,
      Iterator begin,
      Iterator end,
      ConnectCondition connect_condition,
      asio::error_code & ec);
  ```

  

#### async_connect

#### host_name

#### make_address

#### make_address_v4

#### make_address_v6

#### make_network_v4

#### make_network_v6



## basic class template

### socket

#### socket_base

```c++
namespace asio {

    /// The socket_base class is used as a base for the basic_stream_socket and
    /// basic_datagram_socket class templates so that we have a common place to
    /// define the shutdown_type and enum.
    class socket_base;
}
```



#### basic_socket

​		 Provides socket functionality.

```c++
namespace asio {
    // Forward declaration with defaulted arguments.
    template <typename Protocol, typename Executor = any_io_executor>
    class basic_socket;

    /**
     * The basic_socket class template provides functionality that is common to both
     * stream-oriented and datagram-oriented sockets.
     */
    template <typename Protocol, typename Executor>
    class basic_socket : public socket_base;

    /*
    - @par Thread Safety
    - @e Distinct @e objects: Safe.@n
    - @e Shared @e objects: Unsafe.
    */
}
```



#### basic_stream_socket

​    	Provides stream-oriented socket functionality.

```c++
namespace asio {
    // Forward declaration with defaulted arguments.
    template <typename Protocol, typename Executor = any_io_executor>
    class basic_stream_socket;
    
    /**
     * The basic_stream_socket class template provides asynchronous and blocking
     * stream-oriented socket functionality.
     */
    template <typename Protocol, typename Executor>
	class basic_stream_socket : public basic_socket<Protocol, Executor>;
    
    /*
     *
     * @par Thread Safety
     * @e Distinct @e objects: Safe.@n
     * @e Shared @e objects: Unsafe.
     *
     * Synchronous @c send, @c receive, and @c connect operations are thread safe
     * with respect to each other, if the underlying operating system calls are
     * also thread safe. This means that it is permitted to perform concurrent
     * calls to these synchronous operations on a single socket object. Other
     * synchronous operations, such as @c open or @c close, are not thread safe.
     *
     * @par Concepts:
     * AsyncReadStream, AsyncWriteStream, Stream, SyncReadStream, SyncWriteStream.
    */
}
```

##### member func

###### constructor

- without opening socket.

  ​		creates a stream socket without opening it. The socket needs to be opened and then connected or accepted before data can be sent or received on it.

  ```c++
  explicit basic_stream_socket(const executor_type & ex);
  
  // ex:
  //  	默认情况下，套接字将使用的I/O executor 来为在套接字上执行的任何异步操作分派handler。  
  ```

  ```c++
  template<typename ExecutionContext>
  explicit basic_stream_socket(
      ExecutionContext & context,
      typename constraint< is_convertible< ExecutionContext &, 
      execution_context & >::value >::type  = 0);
  
  // context:
  // 		提供I/O executor，套接字将在默认情况下使用它来为在套接字上执行的任何一部操作分派handler。
  ```

###### read_some

​		read data from the stream socket.  The function call will block until one or more bytes of data has been read successfully, or until an error occurs.

​		read_some操作可能不会读取所有请求的字节数。 如果您需要确保在阻塞结束后读取到请求的数据量，请考虑使用read函数。  

```c++
// 参数
// buffers
//		One or more buffers into which the data will be read.

// 返回值
// 		The number of bytes read.
```

- exception close

  ```c++
  template<typename MutableBufferSequence>
  std::size_t read_some(const MutableBufferSequence & buffers);
  // 异常
  // asio::system_error
  //		Thrown on failure. An error code of 
  //				asio::error::eof 
  //		indicates that the connection was closed by the peer.
  ```

  ```c++
  socket.read_some(asio::buffer(data, size));
  // See the buffer documentation for information on reading into multiple buffers in one go, and how to use it with arrays, boost::array or std::vector.
  ```

- error code replace exception to close

  ```c++
  template<typename MutableBufferSequence>
  std::size_t read_some(const MutableBufferSequence & buffers, asio::error_code & ec);
  
  // ec
  //		Set to indicate what error occurred, if any.
  // 			ec == asio::error::eof  
  // 		connection was closed by the peer.
  
  // Returns 0 if an error occurred.
  ```




### acceptor

#### basic_socket_acceptor

​		The basic_socket_acceptor class template is used for accepting new socket connection.

```c++
namespace asio {

    template <typename Protocol, typename Executor = any_io_executor>
    class basic_socket_acceptor;

    
    template <typename Protocol, typename Executor>
    class basic_socket_acceptor : public socket_base;

    /**
     * @par Thread Safety
     * @e Distinct @e objects: Safe.@n
     * @e Shared @e objects: Unsafe.
     *
     * Synchronous @c accept operations are thread safe, if the underlying
     * operating system calls are also thread safe. This means that it is permitted
     * to perform concurrent calls to synchronous @c accept operations on a single
     * socket object. Other synchronous operations, such as @c open or @c close, are
     * not thread safe.
     */
}
```

```c++
 * @par Example
 * Opening a socket acceptor with the SO_REUSEADDR option enabled:

 * @code
 * asio::ip::tcp::acceptor acceptor(my_context);
 * asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), port);
 * acceptor.open(endpoint.protocol());
 * acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
 * acceptor.bind(endpoint);
 * acceptor.listen();
 * @endcode
```

##### member func

###### constructor

```c++
// 参数
// const executor_type & ex
//		The I/O executor that the acceptor will use, by default, to dispatch handlers for any asynchronous operations performed on the acceptor.
// ExecutionContext & context
//		An execution context which provides the I/O executor that the acceptor will use, by default, to dispatch handlers for any asynchronous operations performed on the acceptor.
```



- Construct an acceptor without opening it.

  ​		This constructor creates an acceptor without opening it to listen for new connections. The `open()` function must be called before the acceptor can accept new socket connections.

  - executor

    ```c++
    basic_socket_acceptor(const executor_type & ex);
    ```

  - context

    ```c++
    template<typename ExecutionContext>
    basic_socket_acceptor(
        ExecutionContext & context,
        typename constraint< is_convertible< ExecutionContext &, execution_context & >::value >::type  = 0);
    ```

    

- Construct an acceptor opened on the given endpoint.

  ​		This constructor creates an acceptor and automatically opens it to listen for new connections on the specified endpoint.

  ```c++
  // 参数
  // const endpoint_type & endpoint
  //		An endpoint on the local machine on which the acceptor will listen for new connections.
  // bool reuse_addr = true
  //		Whether the constructor should set the socket option socket_base::reuse_address.
  
  // 异常
  // asio::system_error
  //		Thrown on failure.
  ```

  - executor

    ```c++
    basic_socket_acceptor(
        const executor_type & ex,
        const endpoint_type & endpoint,
        bool reuse_addr = true);
    ```

    > ```c++
    > // This constructor is equivalent to the following code:
    > basic_socket_acceptor<Protocol> acceptor(my_context);
    > acceptor.open(endpoint.protocol());
    > if (reuse_addr)
    >   acceptor.set_option(socket_base::reuse_address(true));
    > acceptor.bind(endpoint);
    > acceptor.listen();
    > ```

  - context

    ```c++
    template<typename ExecutionContext>
    basic_socket_acceptor(
        ExecutionContext & context,
        const endpoint_type & endpoint,
        bool reuse_addr = true,
        typename constraint< is_convertible< ExecutionContext &, execution_context & >::value >::type  = 0);
    ```

    > ```c++
    > // This constructor is equivalent to the following code:
    > basic_socket_acceptor<Protocol> acceptor(my_context);
    > acceptor.open(endpoint.protocol());
    > if (reuse_addr)
    >   acceptor.set_option(socket_base::reuse_address(true));
    > acceptor.bind(endpoint);
    > acceptor.listen();
    > ```

###### opem

###### bind

###### listen

###### accept

- Accept a new connection.

  ```c++
  // 参数
  // basic_socket< Protocol1, Executor1 > & peer
  //		The socket into which the new connection will be accepted.
  ```

  

  ```c++
  template<typename Protocol1,typename Executor1>
  void accept(
      basic_socket< Protocol1, Executor1 > & peer,
      typename constraint< is_convertible< Protocol, Protocol1 >::value >::type  = 0);
  // Exceptions
  // asio::system_error
  //		Thrown on failure.
  ```

  > ```c++
  > // example
  > asio::ip::tcp::acceptor acceptor(my_context);
  > ...
  > asio::ip::tcp::socket socket(my_context);
  > acceptor.accept(socket);
  > ```

  ```c++
  template<typename Protocol1,typename Executor1>
  void accept(
      basic_socket< Protocol1, Executor1 > & peer,
      asio::error_code & ec,
      typename constraint< is_convertible< Protocol, Protocol1 >::value >::type  = 0);
  // asio::error_code & ec
  //		Set to indicate what error occurred, if any.
  ```

  > ```c++
  > // example
  > asio::ip::tcp::acceptor acceptor(my_context);
  > ...
  > asio::ip::tcp::socket socket(my_context);
  > asio::error_code ec;
  > acceptor.accept(socket, ec);
  > if (ec)
  > {
  >   // An error occurred.
  > }
  > ```

- Accept a new connection and obtain the endpoint of the peer.

###### async_accept

​		Start an asynchronous accept.

```c++
// 参数
// basic_socket< Protocol1, Executor1 > & peer
//		The socket into which the new connection will be accepted. Ownership of the peer object is retained by the caller, which must guarantee that it is valid until the handler is called.
// AcceptHandler && handler
//		The handler to be called when the accept operation completes. Copies will be made of the handler as required. The function signature of the handler must be:
	void handler(const asio::error_code& error); // Result of operation
// 无论异步操作是否立即完成，都不会从这个函数中调用处理程序。 在立即完成时，处理程序的调用将以与使用post相同的方式执行。  
```



-  asynchronously accept a new connection into a socket. The function call always returns immediately.

  ```c++
  template<typename Protocol1,typename Executor1,typename AcceptHandler = DEFAULT>
  DEDUCED async_accept(
      basic_socket< Protocol1, Executor1 > & peer,
      AcceptHandler && handler = DEFAULT,
      typename constraint< is_convertible< Protocol, Protocol1 >::value >::type  = 0);
  ```

  > ```c++
  > void accept_handler(const asio::error_code& error)
  > {
  >     if(not error) {/*Aceept succeeded.*/}
  > }
  > 
  > ...
  > asio::ip::tcp::acceptor acceptor(my_context);
  > ...
  > asio::ip::tcp::socket socket(my_context);
  > acceptor.async_accept(socket, accept_handler);
  > ```
  >
  > ​		On POSIX or Windows operating systems, this asynchronous operation supports cancellation for the following [`cancellation_type`](https://think-async.com/Asio/asio-1.20.0/doc/asio/reference/cancellation_type.html) values:
  >
  > - `cancellation_type::terminal`
  > - `cancellation_type::partial`
  > - `cancellation_type::total`

###### async_read_some

​		Start an asynchronous read  from the stream.

​		The function call always returns immediately.

```c++
template<typename MutableBufferSequence,typename ReadHandler = DEFAULT>
DEDUCED async_read_some(
    const MutableBufferSequence & buffers,
    ReadHandler && handler = DEFAULT);


//buffers
// 		One or more buffers into which the data will be read. Although the buffers object may be copied as necessary, ownership of the underlying memory blocks is retained by the caller, which must guarantee that they remain valid until the handler is called.

// handler
//		The handler to be called when the read operation completes. Copies will be made of the handler as required. The function signature of the handler must be:

void handler(
  const asio::error_code& error, // Result of operation.
  std::size_t bytes_transferred           // Number of bytes read.
);
//	Regardless of whether the asynchronous operation completes immediately or not, the handler will not be invoked from within this function. On immediate completion, invocation of the handler will be performed in a manner equivalent to using post.


```

​		The read operation may not read all of the requested number of bytes. Consider using the [`async_read`](https://think-async.com/Asio/asio-1.20.0/doc/asio/reference/async_read.html) function if you need to ensure that the requested amount of data is read before the asynchronous operation completes.

```c++
// example
socket.async_read_some(asio::buffer(data, size), handler);
```

On POSIX or Windows operating systems, this asynchronous operation supports cancellation for the following [`cancellation_type`](https://think-async.com/Asio/asio-1.20.0/doc/asio/reference/cancellation_type.html) values:

- `cancellation_type::terminal`
- `cancellation_type::partial`
- `cancellation_type::total`

###### async_write_some

​		start an asynchronous write to the stream socket.

​		The function call always returns immediately.

```c++
template<typename ConstBufferSequence,typename WriteHandler = DEFAULT>
DEDUCED async_write_some(
    const ConstBufferSequence & buffers,
    WriteHandler && handler = DEFAULT);


// Parameters

//buffers
//One or more data buffers to be written to the socket. Although the buffers object may be copied as necessary, ownership of the underlying memory blocks is retained by the caller, which must guarantee that they remain valid until the handler is called.

//handler
//The handler to be called when the write operation completes. Copies will be made of the handler as required. The function signature of the handler must be:

void handler(
  const asio::error_code& error, // Result of operation.
  std::size_t bytes_transferred           // Number of bytes written.
);
//Regardless of whether the asynchronous operation completes immediately or not, the handler will not be invoked from within this function. On immediate completion, invocation of the handler will be performed in a manner equivalent to using post.
```

​		The write operation may not transmit all of the data to the peer. Consider using the [`async_write`](https://think-async.com/Asio/asio-1.20.0/doc/asio/reference/async_write.html) function if you need to ensure that all data is written before the asynchronous operation completes.

```c++
// example
socket.async_write_some(asio::buffer(data, size), handler);
```

On POSIX or Windows operating systems, this asynchronous operation supports cancellation for the following [`cancellation_type`](https://think-async.com/Asio/asio-1.20.0/doc/asio/reference/cancellation_type.html) values:

- `cancellation_type::terminal`
- `cancellation_type::partial`
- `cancellation_type::total`





