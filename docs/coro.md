# Coroutines in library

- [Comparsion](#comparsion)
- [`Job`](#job)
- [`Task<T>`](#task)
- [`logical_thread`](#logical-thread)
- [`channel<T>`](#channel)

## Comparsion

A "Lazy" coroutine is one whose body won't start executing until explicitly requested.

| Coro             | Lazy     | On unhandled Exception | Short description                 |
|------------------|----------|------------------------|-----------------------------------|
| `job`            | No       | `std::terminate`       | Lightweight coroutine             |
| `task<T>`        | Yes      | Rethrow to caller      | A coroutine similar to a function |
| `async_task<T>`  | No       | Rethrow to caller      | Coroutine with readiness check    |
| `logical_thread` | No       | `std::terminate`       | Coroutine with stop request       |
| `channel<T>`     | Yes      | Rethrow to caller      | Async generator                   |


## Job

A job can be used, for example, to create a coroutine context when you need to perform a co_await. 
Its lightweight nature makes it ideal for scenarios where minimal overhead is desired, allowing you to efficiently manage asynchronous tasks. 
However, the trade-off is that exception handling is limited, and any uncaught exception within the job will result in calling `std::terminate`. 

## Task

`Task<T>` is the most commonly used type of coroutine:

- Uncaught exceptions are preserved and propagated to the caller. This allows for proper exception handling and ensures that errors are not silently discarded.
- It will only be executed when requested. This deferred execution model makes it well-suited for scenarios where you need to control when the coroutine runs, providing flexibility in managing asynchronous operations.
- You can attach additional context to it (for example, a stack trace). This allows you to enrich the coroutine with valuable information, such as debugging or logging details, which can be useful for troubleshooting and monitoring.

## Async Task

- Immediate Execution: Unlike some other coroutine types, async_task starts execution as soon as it is created. There is no need to explicitly request its start.
- Result Readiness Check: It provides an interface to check if the result is ready. This allows non-blocking polling to determine the state of the task without waiting unnecessarily.
- Blocking Wait: If the result is not ready, you have the option to wait for it in a blocking manner. This can be useful when the result is needed immediately, and non-blocking alternatives are not viable.

## Logical Thread



## Channel
