# ngx_custom_worker_module

ngx_custom_worker_module can set the number of processes with any factor of `worker_processes auto`.

## Configuration

- the number of worker processes is __4__ if the number of cpu threads is 4.

```nginx
worker_processes        auto;
```


- the number of worker processes is __12__ if the number of cpu threads is 4.


```nginx
worker_processes        auto;
worker_processes_factor 3;
```

## Install

- use nginx configure option

```
./configure --add-module=/path/to/ngx_custom_worker_module
```

