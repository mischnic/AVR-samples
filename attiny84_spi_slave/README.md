# ATtiny84 as an SPI slave

- SPIMaster contains the code for the master (any Arduino compatible)
- SPISlave contains the slave code (for ATtiny84, change registers according to manual for other chips)

Things to look out for:
- Don't run the SPI too fast (bus speed) and wait (length depends on the code in the interrupt, ~10-300µs) between transfers or you'll get garbage
- You can only transfer one byte at a time
- Knowing how SPI works: master and slave swap their buffers on every transfer, so reading out a byte from the slave requires 2 transfers:

|   | Master        | Slave (in interrupt) |
|---| ------------- |---------------|
| 1.| `SPI.transfer(cmd)`| |
| 2.| | got `cmd`, set USIDR to response |
| 3.| response = `SPI.transfer(anything)` |  |

