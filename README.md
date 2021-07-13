# envoy

build evented, multicast notification chains

## About Notification Chains

This library implements a generic notification chain API that can be utilized to construct multicast event systems.

### Where are notification chains used in the wild?

Notification chains of this variety are commonly found in the Linux kernel and are used for networking. In my experience, notification chains are often employed in the management of routing tables across a routing subsystem or greater network topology.

### So what, really, is a notification chain?

A notification chain is a data structure that plays the role of a generic publisher in a pubsub model. By generic, we mean the chain does not care about its subscribers, only that they have satisfied the base interface for being a subscriber. Here, this means providing a subscriber id, and a void function pointer that should be invoked when a subscribable event occurs.

### How does it work?

The notification chain is at its core a linked list data structure. Each chain represents a single, observable (it can be subscribed to) piece of data, denoted here by a 64 bit character array key.

Each node in the linked list is a subscription entry. Whether the notification chain API itself or the caller creates new nodes is discerned by the programmer and not the model - that is, this detail is of negligible consequence provided the node is implemented as expected.

When the notification chain's delegated data has changed, or an observable event has transpired, we iterate the linked list and check each subscription (node). If the subscriber has provided the correct subscription ID (key), we invoke the callback at the void function pointer that was provided to us.

There's a reason we use the void pointer. A notification chain is often part of a multi-threaded system. To pass data across threads is of greater expense to the OS / kernel than dereferencing a function pointer. And so we elect a strategy called ['Transfer of Computation'](https://gist.github.com/MatthewZito/e187ce42eadaaf2a1eeabb0d36fe9604)

If the subscriber has provided NO key at all, we assume the subscriber is interested in *all* data, and we invoke the provided function pointer.

Finally, notification chains may manage other chains. We can recurse and have a data structure that is a network of threads, each managing a set of notification chains (which are in turn detached POSIX threads).

### Optimizing with GLUE lists

There's a fairly novel albeit obscure concept called a 'GLUE Linked List'. The idea here is we implement a classical linked list, but each node is without a data pointer.

Instead, we use the memory format of the node structure itself to calculate an *offset* at which the data represented by that node resides. That is, the offset is said to be 'glued' to the node at no additional expense.

A considerable benefit here is we no longer incur *any* cost of carrying around data, even a pointer. `envoy` implements notification chains using this variety of linked list; you can find the source code in both this library and `lib.cartilage`

## Dynamic Linking

Linking to `lib.envoy`:

```bash
# 1) include and use lib.envoy in your project
# 2) generate object file for your project
gcc -I ../path/to/libenvoy -c main.c -o main.o
# 3) generate shared object file
make
# 4) link your project to lib.envoy
gcc -o main main.o -L../path/to/libenvoy -llibenvoy
# you may need to add the lib location to your PATH
```

Linking to `lib.envoy` on Windows:

```bash
# 1) include and use lib.envoy in your project
# 2) generate object file for your project
gcc -I ../path/to/libenvoy -c main.c -o main.o
# 3) generate shared object file
make win
# 3) link your project to lib.envoy
gcc -o main.exe main.o -L /path/to/lib.envoy -llib_envoy.dll
# you may need to add the lib location to your PATH
```
