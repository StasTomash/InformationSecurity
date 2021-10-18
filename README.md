All algorithm testing is performed on a machine with Intel(R) Core(TM) i7-9750H CPU @ 2.60GHz, 32.0GB RAM, Windows 10. <br>
Benchmarking results for different ciphers (enciphering):

| Cipher | Additional parameters | Data size | Time consumed |
| ------ | --------------------- | --------- | ------------- |
| AES ECB mode | 128 bits key | 100MB | 20.5s |
| AES CBC mode | 128 bits key | 100MB | 23.5s |
| AES CFB mode | 128 bits key | 100MB | 23.2s |
| AES OFB mode | 128 bits key | 100MB | 20.3s |
| AES CTR mode | 128 bits key | 100MB | 22.9s |
| Kalyna ECB | 128 bits key, 128 bits block | 1MB | 7.8s |
| Kalyna ECB | 512 bits key, 512 bits block | 1MB | 11.8s |
| Kalyna ECB | 128 bits key, 128 bits block | 10MB | 65.6s |
| Kalyna ECB | 512 bits key, 512 bits block | 10MB | 107.9s |
| RC4 | 256 bytes key | 100MB | 3.3s |
| Salsa20 | | 100MB | 1.2s |

Benchmarking results for different ciphers (deciphering):

| Cipher | Additional parameters | Data size | Time consumed |
| ------ | --------------------- | --------- | ------------- |
| AES ECB mode | 128 bits key | 100MB | 214.1s |
| AES CBC mode | 128 bits key | 100MB | 237.2s |
| AES CFB mode | 128 bits key | 100MB | 22.9s |
| AES OFB mode | 128 bits key | 100MB | 20.0s |
| AES CTR mode | 128 bits key | 100MB | 22.5s |
| Kalyna ECB | 128 bits key, 128 bits block | 1MB | 7.5s |
| Kalyna ECB | 512 bits key, 512 bits block | 1MB | 11.3s |
| Kalyna ECB | 128 bits key, 128 bits block | 10MB | 77.3s |
| Kalyna ECB | 512 bits key, 512 bits block | 10MB | 123.2s |
| RC4 | 256 bytes key | 100MB | 3.4s |
| Salsa20 | | 100MB | 1.2s |
