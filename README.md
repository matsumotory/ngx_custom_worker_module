# ngx_custom_worker_module

ngx_custom_worker_module can set the number of processes with any factor or spare of `worker_processes auto`.

## Configuration

- the number of worker processes is __4__ if the number of cpu threads is 4.

```nginx
worker_processes        auto;
```

### worker_processes_factor

- the number of worker processes is __12__ if the number of cpu threads is 4.


```nginx
worker_processes        auto;
worker_processes_factor 3;
```

### worker_processes_spare

- the number of worker processes is __2__ if the number of cpu threads is 4.


```nginx
worker_processes        auto;
worker_processes_spare 2;
```

### set worker_processes_factor and worker_processes_spare

The worker_processes_factor is applied before the worker_processes_spare.

- the number of worker processes is __10__ if the number of cpu threads is 4.


```nginx
worker_processes        auto;
worker_processes_factor 3;
worker_processes_spare 2;
```

- the number of worker processes is __10__ if the number of cpu threads is 4.

```nginx
worker_processes        auto;
worker_processes_spare 2;
worker_processes_factor 3;
```

## Install

- use nginx configure option

```
./configure --add-module=/path/to/ngx_custom_worker_module
```

