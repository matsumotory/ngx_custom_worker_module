# ngx_custom_worker_module

An nginx module can set the number of processes set with `worker_processes auto` as any factor.

## Configuration

- the number of worker processes is __4__ if the number of cpu thread is 4

```nginx
worker_processes        auto;
```


- the number of worker processes is __12__ if the number of cpu thread is 4


```nginx
worker_processes        auto;
worker_processes_factor 3;
```

## Install

- use nginx configure option

```
./configure --add-module=/path/to/ngx_custom_worker_module
```

