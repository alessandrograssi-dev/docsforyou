[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![CMake](https://img.shields.io/badge/CMake-3.15+-blue.svg)](https://cmake.org/)
[![Docker](https://img.shields.io/badge/Docker-Supported-blue.svg)](https://www.docker.com/)
[![CI](https://github.com/alessandrograssi-dev/docsforyou/actions/workflows/ci.yml/badge.svg)](https://github.com/alessandrograssi-dev/docsforyou/actions/workflows/ci.yml)

# docsforyou

Small C++ HTTP service built with Boost.Beast + CMake.

The project exposes a minimal REST API to store and retrieve text documents on disk.

## Why this project is useful

- Clean separation of concerns: `http` / `services` / `repository`
- Async HTTP server using Boost.Beast
- Interface-based persistence (`DocumentRepository`) with file implementation
- Easy local build with CMake and Make
- Dockerized runtime for reproducible runs

## Project structure

```text
src/
  http/        # Server, routing, request/response mapping, controllers
  services/    # Business logic (counter, document management)
  repository/  # Persistence abstraction and file implementation
  main.cpp     # Program entrypoint
```

## Requirements

- Linux/macOS
- C++17 compiler
- CMake >= 3.15
- Boost (`system`, `filesystem`)
- nlohmann/json headers

On Ubuntu/Debian (example):

```bash
sudo apt update
sudo apt install -y build-essential cmake libboost-all-dev nlohmann-json3-dev
```

## Build and run (host)

```bash
make clean && make
./build/docsforyou 127.0.0.1 8080
```

## API

Base URL:

```text
http://127.0.0.1:8080
```

### 1) Health-like counters

- `GET /count` → HTML with request counter
- `GET /time` → HTML with current epoch time

Example:

```bash
curl -i http://127.0.0.1:8080/count
curl -i http://127.0.0.1:8080/time
```

### 2) Documents

- `POST /doc` creates a document
- `GET /doc/{id}` fetches one document
- `DELETE /doc/{id}` deletes one document

Create:

```bash
curl -i -X POST http://127.0.0.1:8080/doc \
  -H "Content-Type: application/json" \
  -d '{"author":"alice","content":"hello world"}'
```

Expected response:

```json
{"id":"0"}
```

Read:

```bash
curl -i http://127.0.0.1:8080/doc/0
```

Delete:

```bash
curl -i -X DELETE http://127.0.0.1:8080/doc/0
```

## Docker

Build image:

```bash
make docker-build
```

Run container (maps host `8080` to container `8080`):

```bash
make docker-run
```

Stop container:

```bash
make docker-stop
```

Tail logs:

```bash
make docker-logs
```

Override defaults:

```bash
make docker-build IMAGE_NAME=docsforyou:v1
make docker-run IMAGE_NAME=docsforyou:v1 CONTAINER_NAME=docsforyou-dev PORT=9090
```

## Notes

- Documents are currently stored in the process working directory as `*.DFY` files.
- IDs are numeric and generated incrementally.
- On systems where Docker requires elevated permissions, add your user to the `docker` group or run Docker commands with sudo.

## Suggested next improvements

- Add automated tests (service and routing layers)
- Add request validation and consistent JSON error bodies
- Add configurable data directory via env var or CLI
- Add CI (build + test) workflow
