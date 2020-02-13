# haru

Erlang libharu implementation uses Erlang ports to isolate the
creation of different data-structure.

## Build

This project require a C compiler (clang or gcc) and GNU Make to build
sources present in `c_src`. Erlang and its libraries are also required.

```sh
rebar3 compile
```

## Documentation

The documentation is generated with `edoc` and `doxygen`.

```sh
rebar3 edoc
```

## Test

Testing is executed for Erlang with `eunit` and `ct`, and `minunit`
for the C side.

```sh
rebar3 eunit
rebar3 ct
```

## Why this project?

I wrote an article some years ago about ETF and Erlang. It was a
series, but, I never had time to end every articles. This project was
made to continue those articles and create concrete example.

Another motivation is to create a better way to manage ports in
Erlang, by creating a kind of framework/library to secure the C code
outside the BEAM. (you can see that in `c_src/haru_comm.c` and
`c_src/haru_comm.h`).

## What is the idea?

This project is a PoC. The idea behind it is to create a dynamic
relation between Erlang and C ports by creating on demand different
linked processes outside the BEAM. Two methods will be used:

 1. The first method uses the KISS (Keep It Simple, Stupid) and the
    DOTDIW (Do One Thing, Do It Well) principles. Many processes are
    created outside the BEAM with the erlang:port interface. Each
    processes are working as FSM (Finite State Machine), retrieving
    the object, doing one action on it, and return the updated objet.
    
 2. The second method is more complex. Each processes are storing one
    object and receive instruction to modify it by the erlang:port
    interface. When everything is done, a command is sent by the
    erlang node to retrieve the full object.

## Thanks

This project uses:

 * Erlang/OTP for the main interface
 * minunit for unit testing in C
 * Doxygen for generating documentation
 * libharu for generating PDF

Many thanks for all these open-source software. <3

## Resources

 * https://github.com/libharu/libharu
 * https://github.com/libharu/libharu/wiki/Usage-examples
