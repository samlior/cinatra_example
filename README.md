# cinatra_example

This is a example project for [cinatra](https://github.com/qicosmos/cinatra)

## Build on Linux and macOs

```sh
git clone https://github.com/samlior/cinatra_example.git
cd cinatra_example
mkdir build && cd build
cmake .. -DCMAKE_C_FLAGS=-mavx2 -DCMAKE_CXX_FLAGS=-mavx2 -DCMAKE_BUILD_TYPE=Release -G Ninja
ninja
```

## Build on Windows

```sh
git clone https://github.com/samlior/cinatra_example.git
cd cinatra_example
mkdir build && cd build
cmake ..
```

## License

[GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.en.html)
