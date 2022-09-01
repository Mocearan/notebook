# networking

---







## ip

```c++
namespace asio {
    // socket
    class socket_base;
    class basic_stream_socket<typename Protocol, typename Executor>;
    
    // ip
    namespace ip {
        // address
        class address;
        /// v4
        class address_v4;
        class address_v4_iterator;
        class address_v4_range;
        /// v6
        class address_v6;
        class address_v6_iterator;
        class address_v6_range;
        ///
        class bad_address_cast;
        
        // icmp
        class icmp;
        
        // network
        class network_v4;
        class network_v6;
        
        // resolver
        class resolver_base;
        class resolver_query_base;
        
        // tcp
    	class tcp;
        
        // udp
        class udp;
    }
}
```



### resolver

#### resolver_base

```c++
namespace asio {
namespace ip {

    /// The resolver_base class is used as a base for the basic_resolver class
    /// templates to provide a common place to define the flag constants.
    class resolver_base;
}
```

#### basic_resolver

​     Provides endpoint resolution functionality.

```c++
namespace asio {
namespace ip {

    // Forward declaration with defaulted arguments.
    template <typename InternetProtocol, typename Executor = any_io_executor>
    class basic_resolver;

    /**
     * The basic_resolver class template provides the ability to resolve a query
     * to a list of endpoints.
     */
    template <typename InternetProtocol, typename Executor>
    class basic_resolver : public resolver_base;
    
    /*
     * @par Thread Safety
     * @e Distinct @e objects: Safe.@n
     * @e Shared @e objects: Unsafe.
    */
}
}
```

##### member func

###### resolve

​		Perform forward resolution of a query to a list of entries.

- Deprecated: Use overload with separate host and service parameters

  ```c++
  results_type resolve(const query & q);
  
  results_type resolve(const query & q, asio::error_code & ec);
  ```

  ```c++
  tcp::resolver::query query(argv[1], "daytime");
  tcp::resolver::results_type endpoints = resolver.resolve(query);
  ```

  

- ```c++
  results_type resolve(string_view host, string_view service);
  ```

  ```c++
  tcp::resolver::results_type endpoints =
        resolver.resolve(argv[1], "daytime");
  ```

  



### resolver_iterator

#### basic_resolver_iterator

```c++
namespace asio {
namespace ip {

    /// An iterator over the entries produced by a resolver.
    /**
     * The asio::ip::basic_resolver_iterator class template is used to define
     * iterators over the results returned by a resolver.
     *
     * The iterator's value_type, obtained when the iterator is dereferenced, is:
     * @code const basic_resolver_entry<InternetProtocol> @endcode
     *
     * @par Thread Safety
     * @e Distinct @e objects: Safe.@n
     * @e Shared @e objects: Unsafe.
     */
    template <typename InternetProtocol>
    class basic_resolver_iterator;
    
} // ip
} // asio
```



### resolver_query

​		包装主机名和服务名， 生成一个用于生成网络通讯地址信息的请求。

#### resolver_query_base

```c++
namespace asio {
namespace ip {

    /// The resolver_query_base class is used as a base for the
    /// basic_resolver_query class templates to provide a common place to define
    /// the flag constants.
    class resolver_query_base : public resolver_base
    {
    protected:
      /// Protected destructor to prevent deletion through this type.
      ~resolver_query_base(){}
    };

} // namespace ip
} // namespace asio
```

#### basic_resolver_query

```c++
namespace asio {
namespace ip {

    /// An query to be passed to a resolver.
    /**
     * The asio::ip::basic_resolver_query class template describes a query
     * that can be passed to a resolver.
     */
    template <typename InternetProtocol>
    class basic_resolver_query  : public resolver_query_base;
    
    /*
     *
     * @par Thread Safety
     * @e Distinct @e objects: Safe.@n
     * @e Shared @e objects: Unsafe.
    */

} // ip
} // asio
```

##### member func

###### constructor

- 

  ```c++
  basic_resolver_query(
      const std::string & host,
      const std::string & service,
      resolver_query_base::flags resolve_flags = address_configured);
  ```

  

  

### tcp

​		封装了`tcp`套接字所需的标志

```c++
namespace asio {
namespace ip {
/**
 * @par Thread Safety
 * @e Distinct @e objects: Safe.@n
 * @e Shared @e objects: Safe.
 *
 * @par Concepts:
 * Protocol, InternetProtocol.
 */
class tcp
{
private:
  // Construct with a specific family.
  explicit tcp(int protocol_family) ASIO_NOEXCEPT: family_(protocol_family) {}

  int family_;
public:
  /// The type of a TCP endpoint.
  typedef basic_endpoint<tcp> endpoint;
    
  /// The TCP socket type.
  typedef basic_stream_socket<tcp> socket;

  /// The TCP acceptor type.
  typedef basic_socket_acceptor<tcp> acceptor;

  /// The TCP resolver type.
  typedef basic_resolver<tcp> resolver;

#if !defined(ASIO_NO_IOSTREAM)
  /// The TCP iostream type.
  typedef basic_socket_iostream<tcp> iostream;
#endif // !defined(ASIO_NO_IOSTREAM)

  /// Socket option for disabling the Nagle algorithm.
  /**
   * Implements the IPPROTO_TCP/TCP_NODELAY socket option.
   *
   * @par Examples
   * Setting the option:
   * @code
   * asio::ip::tcp::socket socket(my_context);
   * ...
   * asio::ip::tcp::no_delay option(true);
   * socket.set_option(option);
   * @endcode
   *
   * @par
   * Getting the current option value:
   * @code
   * asio::ip::tcp::socket socket(my_context);
   * ...
   * asio::ip::tcp::no_delay option;
   * socket.get_option(option);
   * bool is_set = option.value();
   * @endcode
   *
   * @par Concepts:
   * Socket_Option, Boolean_Socket_Option.
   */
#if defined(GENERATING_DOCUMENTATION)
  typedef implementation_defined no_delay;
#else
  typedef asio::detail::socket_option::boolean<
    ASIO_OS_DEF(IPPROTO_TCP), ASIO_OS_DEF(TCP_NODELAY)> no_delay;
#endif
    
public:

  /// Construct to represent the IPv4 TCP protocol.
  static tcp v4() ASIO_NOEXCEPT { return tcp(ASIO_OS_DEF(AF_INET4)); }
    
  /// Construct to represent the IPv6 TCP protocol.
  static tcp v6() ASIO_NOEXCEPT { return tcp(ASIO_OS_DEF(AF_INET6)); }

  /// Obtain an identifier for the type of the protocol.
  int type() const ASIO_NOEXCEPT { return ASIO_OS_DEF(SOCK_STREAM); }

  /// Obtain an identifier for the protocol.
  int protocol() const ASIO_NOEXCEPT { return ASIO_OS_DEF(IPPROTO_TCP); }

  /// Obtain an identifier for the protocol family.
  int family() const ASIO_NOEXCEPT { return family_; }

  /// Compare two protocols for equality.
  friend bool operator==(const tcp& p1, const tcp& p2) { return p1.family_ == p2.family_; }

  /// Compare two protocols for inequality.
  friend bool operator!=(const tcp& p1, const tcp& p2) { return p1.family_ != p2.family_; }
};

} // namespace ip
} // namespace asio
```

   0  

