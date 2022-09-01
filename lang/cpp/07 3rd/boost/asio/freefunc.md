# free functions

---

## bind_executor

*Header:* `asio/bind_executor.hpp`

*Convenience header:* `asio.hpp`

- 将类型为T的对象与类型为executor的执行程序关联

```c++
template<typename Executor, typename T>
executor_bind< typename decay<T>::type, Executor>  
    bind_executor(const Executor& ex
                  , T && t
                  , typename constraint<is_executor<Executor>::value 
                  || execution::is_executor<Executor>::value>::type = 0);
```

- 将类型T的对象与执行上下文的执行器关联。  

```c++
template<typename ExecutionContext,typename T>
executor_binder< typename decay< T >::type, typenameExecutionContext::executor_type > 
    bind_executor(ExecutionContext & ctx
                  ,T && t
                  ,typename constraint< is_convertible< ExecutionContext &
                  					, execution_context & >::value >::type  = 0);
```

