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
