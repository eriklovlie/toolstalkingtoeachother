# Tools Talking Json

This is a silly experiment where I was trying to find the simplest way
to get command line tools in various languages to talk to each other.

Currently includes two example tools (one C++ and one OCaml) which both
read and append to a JSON file. The schema (data binding) takes care of
validation and serialization/deserialization.

The online protobuf example with the address book was used.

There is one source schema; piqi. The protobuf schema and code is generated
from the piqi schema.

## Building the C++

You need to clone proto3 (in beta atm) somewhere and build it first.
See `build_cpp`, it should be obvious. Build and run thusly:

```
./build_cpp
./addperson_cpp testdata.json
```

## Building the OCaml

You need some libraries first, install using opam.
See `build_ocaml`, it should be obvious. Build and run thusly:

```
./build_ocaml
./addperson_ocaml testdata.json
```