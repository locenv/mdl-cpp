# Module Development Library for C++

This is a C++ library to aid on module development for locenv.

## Why C++, not C?

We don't recommend C due to how Lua error works. Lua has 2 mechanism for trigger errors:

- Using `setjmp` and `longjmp`.
- Using C++ exception.

We cannot use the first one due to it will cause resource leak on Rust, so the only choices available for us is C++. If you use C to create a module keep in mind that you don't have a chance to free the resources when Lua error is triggered. So with C you are going to run into resource leak for sure.

## License

MIT
