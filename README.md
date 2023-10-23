# jack-o-scope


# Build From Source

Built on MacOS using the PICO SDK (modified from pico-examples pio/ws2812)

* [PICO SDK](https://github.com/raspberrypi/pico-sdk) Notes 
  * See: https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf
  * For Step `9.1.1. Installing the Toolchain`, install `gcc-arm-embedded` instead of `arm-none-eabi-gcc`
* Run `sh ./configure` for initial cmake configure
* Reconnect pico as needed holding bootselector button
* Run `sh ./deploy.sh` to build and copy to pico