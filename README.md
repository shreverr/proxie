
# Proxie

A simple reverse proxy in C.



## Run Locally

Clone the project

```bash
  git clone https://github.com/shreverr/proxie.git
```

Go to the project directory

```bash
  cd proxie
```

Build project

```bash
  make
```

copy example config

```bash
  mkdir -p /var/proxie/
  cp proxie.conf /var/proxie/proxie.conf
```

Start app

```bash
  make run
```


## Config Reference 

| Parameter | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `listen_address` | `ip/domain` | **Required**. IP addess to listen|
| `listen_port` | `port number (int)` | **Required**. Port to listen|
| `target_address` | `ip/domain` | **Required**. IP addess to forward request|
| `target_port` | `port number (int)` | **Required**. Port to forward request|
