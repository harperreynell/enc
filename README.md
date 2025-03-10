# enc

### Build
**Make sure you have installed openssl**

```
sudo apt install openssl
sudo apt install libssl-dev
```

Now you are ready to go.

```
clang++ main.cpp -o enc -lssl -lcrypto
```

### Ececute
```
enc <path to file or directory> <key> <operation>
```

There are two possible operations

`enc` To encrypt 

`dec` To decrypt


## Algorithm
Idea is simple, realisation of DES algorithm

Read the file character by character, then convert the hashed key to a single integer by converting each character of the string and adding them up. Add this integer to the read character for encryption and subtract it for decryption.

Encryption and decryption occur in place, with no additional files created.