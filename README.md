# libxid

[![license](https://img.shields.io/badge/license-MIT-green)](https://raw.githubusercontent.com/uatuko/libxid/main/LICENSE)
[![codecov](https://codecov.io/gh/uatuko/libxid/branch/main/graph/badge.svg?token=cLI6mChQ6V)](https://codecov.io/gh/uatuko/libxid)
[![build](https://github.com/uatuko/libxid/actions/workflows/build.yaml/badge.svg?branch=main)](https://github.com/uatuko/libxid/actions/workflows/build.yaml)
[![release](https://img.shields.io/github/v/release/uatuko/libxid)](https://github.com/uatuko/libxid/releases)

A globally unique id generator.

> This is a c++ implementation of the Golang package found at: [https://github.com/rs/xid](https://github.com/rs/xid)

Xid uses MongoDB Object ID algorighm[^1] to generate globally unique ids with base32 serialzation to produce shorter strings.


## Binary representation

```
+---+----+----+----+----+----+----+----+----+----+-----+-----+
| 0 |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 |  10 |  11 |
+---+----+----+----+----+----+----+----+----+----+-----+-----+
 \----- time -----/ \ machine id / \- pid -/ \--- counter ---/
```

- 4-byte value representing the seconds since the Unix epoch,
- 3-byte machine identifier,
- 2-byte process id, and
- 3-byte counter, starting with a random value.

The binary representation of the id is compatible with MongoDB's 12 bytes Object IDs.
The string representation is using base32hex (w/o padding)[^2] for better space efficiency when stored in string form (20 bytes). The hex variant of base32 is used to retain the
sortable property of the id.


## Features

- Size: 12 bytes (96 bits), smaller than UUID, larger than Twitter Snowflake[^3]
- Base32hex encoded by default (20 chars when transported as printable string, still sortable)
- Configuration free: there is no need to set a unique machine and/or data center id
- K-ordered
- Embedded time with 1 second precision
- Unicity guaranteed for 16,777,216 (24 bits) unique ids per second and per host/process
- Lock-free (unlike UUIDv1 and v2)


## Comparison

| Name        | Binary Size | String Size    | Features
|-------------|-------------|----------------|----------------
| [UUID]      | 16 bytes    | 36 chars       | configuration free, not sortable
| [shortuuid] | 16 bytes    | 22 chars       | configuration free, not sortable
| [Snowflake] | 8 bytes     | up to 20 chars | needs machine/DC configuration, needs central server, sortable
| [MongoID]   | 12 bytes    | 24 chars       | configuration free, sortable
| xid         | 12 bytes    | 20 chars       | configuration free, sortable

[UUID]: https://en.wikipedia.org/wiki/Universally_unique_identifier
[shortuuid]: https://github.com/stochastic-technologies/shortuuid
[Snowflake]: https://blog.twitter.com/2010/announcing-snowflake
[MongoID]: https://www.mongodb.com/docs/manual/reference/method/ObjectId/


## Install
### CMake

```cmake
include(FetchContent)

#libxid
FetchContent_Declare(libxid
  URL      https://github.com/uatuko/libxid/archive/refs/tags/v0.1.0.tar.gz
  URL_HASH SHA256=31589bb5274c9d25a8b6c49ee04a6c76151f10082e7feb13314be02a4b2d58c8
)
FetchContent_MakeAvailable(libxid)
```

```cmake
target_link_libraries(<target>
  PRIVATE
    libxid::xid
)
```


## Usage

```c++
#include <xid/xid.h>

auto id = xid::next(); // generate the next id
std::string s = xid::next(); // generate the next id and convert it to a string
```

```c++
// Get embedded info
auto id = xid::next();

id.time();
id.machine();
id.pid();
id.counter();
```

```c++
// Decode id strings
auto id = xid::id("cc0a2mn6i1e6brmdbip0");
auto bytes = xid::id::decode("cc0aar76i1e6jr6no620");
```


[^1]: https://www.mongodb.com/docs/manual/reference/method/ObjectId/
[^2]: https://datatracker.ietf.org/doc/html/rfc4648#section-7
[^3]: https://blog.twitter.com/2010/announcing-snowflake
