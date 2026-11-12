# 🏎️ nano-kv-c

<div align="center">

[![C CI](https://github.com/IsmeetPandey/nano-kv-c/actions/workflows/ci.yml/badge.svg)](https://github.com/IsmeetPandey/nano-kv-c/actions/workflows/ci.yml)
[![Language: C99](https://img.shields.io/badge/Language-C99-A8B9CC.svg)](https://en.wikipedia.org/wiki/C99)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

**Minimalist, embeddable append-only key-value storage engine in ANSI C99 with CRC32 checksum integrity.**

</div>

---

## ⚡ Key Highlights

- **Append-Only Log Structured**: Eliminates random disk writes by streaming modifications sequentially.
- **Data Integrity Guarantee**: In-line CRC32 checksums detect and reject corrupted storage blocks.
- **In-Memory Hash Index**: Instant O(1) in-memory key lookup to file byte offset.
- **Embedded & Zero Dependencies**: Compiled with standard C runtime, ideal for microcontrollers and system agents.

---

## 🚀 Quick Start

```bash
git clone https://github.com/IsmeetPandey/nano-kv-c.git
cd nano-kv-c
make
./bin/nanokv
```

---

## 📄 License

MIT License © 2026 [Ismeet Pandey](https://github.com/IsmeetPandey).
