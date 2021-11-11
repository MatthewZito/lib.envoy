# envoy

build evented, multicast notification chains

## Table of Contents

- [Install](#install)
- [Dynamic Linking](#linking)
- [Documentation](#docs)
- [About Notification Chains](#about)
- [Use Case Example](#example)

## <a name="install"></a> Install

Via [clib](https://github.com/clibs/clib/):

```bash
clib install MatthewZito/lib.cartilage
```

From Source:
```bash
git clone https://github.com/MatthewZito/lib.cartilage
cd lib.cartilage && make
```

## <a name="linking"></a> Dynamic Linking

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

## <a name="docs"></a> API and Documentation

### Op Codes

- `ENVOY_UNKNOWN`,
- `ENVOY_SUB`,
- `ENVOY_CONFIRM_SUBSCRIBE`,
- `ENVOY_UNSUBSCRIBE`,
- `ENVOY_MOD`,
- `ENVOY_DEL`

### API Methods

**Subscriber callback type**

```c
typedef void* (*envoy_emitter)(void*, size_t, envoy_event_t, uint32_t);
```

**Allocate and initialize a new Envoy with a given name**

```c
envoy_t* envoy_init(char* envoy_name);
```

**Register a new node as a subscriber to a given Envoy**

```c
void envoy_subscribe(envoy_t* envoy, envoy_node_t* node);
```

**invoke an envoy, initiating event emission upon all subscribers to `key`**

```c
void envoy_invoke(
	envoy_t* envoy,
	void* arg,
	size_t arg_size,
	char* key,
	size_t key_size,
	envoy_event_t op_code);
```

**Purge, delete, and deallocate all nodes of the given envoy**

```c
void envoy_purge(envoy_t* envoy);
```

## <a name="about"></a> About Notification Chains

This library implements a generic notification chain API that can be utilized to construct multicast event systems.

### Where are notification chains used in the wild?

Notification chains of this variety are commonly found in the Linux kernel and are used for networking. In my experience, notification chains are often employed in the management of routing tables across a routing subsystem or greater network topology.

### So what, really, is a notification chain?

A notification chain is a data structure that plays the role of a generic publisher in a pubsub model. By generic, we mean the chain does not care about its subscribers, only that they have satisfied the base interface for being a subscriber. Here, this means providing a subscriber id, and a void function pointer that should be invoked when a subscribable event occurs.

### How does it work?

The notification chain is at its core a linked list data structure. Each chain represents a single, observable (it can be subscribed to) piece of data, denoted here by a 64 bit character array key.

Each node in the linked list represents a subscription, along with the required data for notifying the subscriber.

When the notification chain's delegated data has changed, or an observable event has transpired, we iterate the linked list and check each subscription (node). If the subscriber has provided the correct subscription ID (key), we invoke the callback at the void function pointer that was provided to us.

Similarly, if no key was provided, we presume the subscriber is interested in all events and we notify them as well.

### Why not just pass the data?

There's a reason we use the void pointer in lieu of passing data around. A notification chain is often part of a multi-threaded system. To transfer data across threads is of greater expense to the OS / kernel than dereferencing a function pointer. And so we elect a strategy called 'Transfer of Computation'. Have a look at this gist to see a demo of this strategy [here](https://gist.github.com/MatthewZito/e187ce42eadaaf2a1eeabb0d36fe9604)

### Optimizing with GLUE lists

There's a fairly novel albeit obscure concept called a 'GLUE Linked List'. The idea here is we implement a classical linked list, but each node is without a data pointer.

Instead, we use the memory format of the node structure itself to calculate an *offset* at which the data represented by that node resides. That is, the offset is said to be 'glued' to the node at no additional expense.

A considerable benefit here is we no longer incur *any* cost of carrying around data, even a pointer. `envoy` implements notification chains using this variety of linked list; you can find the source code in both this library and `lib.cartilage`

Resources and further reading:

- [Notification Chains in Linux](https://www.oreilly.com/library/view/understanding-linux-network/0596002556/ch04.html)

## <a name="example"></a> Example: Routing Tables

[Here's a simplified example](./examples/routing_table) of what a real use case for notification chains might look like.

Notice the publisher thread is never actually aware of subscribers - instead, the data source (in this case a routing table) implements an API that allows the transference of events between the two data structures.

The data source also handles the initialization of subscription nodes before they are passed on to the envoy.

In the example, we generate *N* subscriber threads which each subscribe to a determinant number of routing table entries of *X* ip address.

We then spawn the publisher thread, which updates *N* random entries of the routing table.

### Invoking the simulation

Use the Makefile to compile, then execute the program to see the verbose output of the simulation.

To run, invoke `make` in the root directory, then in examples/routing_table. Finally, you may need to copy the shared object file libenvoy.so into the examples/routing_table directory.

Notice we first spawn *N* subscriber threads; each subscriber chooses a random integer between 0 - 9 and begins subscribing to IP addresses ending with that integer.

Each subscriber will be notified with the `ENVOY_CONFIRM_SUBSCRIBE` opcode immediately.

Next, we spawn the publisher thread. This thread similarly begins choosing ip addresses at random and edits their routing table entries, causing subscribers to be notified of changes.

In a real system these threads would most likely be detached (i.e. the main thread / process would be indefinite), but for our purposes we end the program by joining all threads and ensuring our main thread has awaited them.
