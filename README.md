# clcus

This is a **cluster chat server** project that uses `muduo` as the underlying 
communication module to handle network IO. 

In addition, it uses `MySQL` for data persistence and uses `Redis` publish-subscribe 
function for communication between different servers.

At the same time, `Nginx` is used for load balancing at the transport layer.

In the data transfer module, it uses json to serialize and deserialize the transfer data.

![clcus](https://user-images.githubusercontent.com/45937428/198868315-0bc362db-e2fd-4fb2-99fd-2ecc11902f42.png)

## How to use

Type the following commands to build the project and compile:

```sh
./autobuild.sh
```

Then you can find the executable file in the `bin` directory.
