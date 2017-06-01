#include <ngx_config.h>
#include <ngx_core.h>
#include <nginx.h>

static char *ngx_worker_process_factor(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static ngx_command_t ngx_core_custom_worker_commands[] = {

    {ngx_string("worker_processes_factor"), NGX_MAIN_CONF | NGX_DIRECT_CONF | NGX_CONF_TAKE1, ngx_worker_process_factor,
     0, 0, NULL},

    ngx_null_command};

static ngx_core_module_t ngx_core_custom_worker_module_ctx = {ngx_string("core_custom_worker"), NULL, NULL};

ngx_module_t ngx_core_custom_worker_module = {NGX_MODULE_V1,
                                              &ngx_core_custom_worker_module_ctx, /* module context */
                                              ngx_core_custom_worker_commands,    /* module directives */
                                              NGX_CORE_MODULE,                    /* module type */
                                              NULL,                               /* init master */
                                              NULL,                               /* init module */
                                              NULL,                               /* init process */
                                              NULL,                               /* init thread */
                                              NULL,                               /* exit thread */
                                              NULL,                               /* exit process */
                                              NULL,                               /* exit master */
                                              NGX_MODULE_V1_PADDING};

static char *ngx_worker_process_factor(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
  ngx_str_t *value;
  ngx_int_t factor;
  ngx_core_conf_t *ccf;

  ccf = (ngx_core_conf_t *)ngx_get_conf(cf->cycle->conf_ctx, ngx_core_module);

  if (ccf->worker_processes == NGX_CONF_UNSET) {
    return "don't set worker process";
  }

  value = cf->args->elts;
  factor = ngx_atoi(value[1].data, value[1].len);

  if (factor < 1) {
    return "worker_processes_factor shoud be positive number";
  }

  ccf->worker_processes = ccf->worker_processes * factor;

  return NGX_CONF_OK;
}
