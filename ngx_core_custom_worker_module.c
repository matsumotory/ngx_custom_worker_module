#include <ngx_config.h>
#include <ngx_core.h>
#include <nginx.h>

static void *ngx_custom_worker_create_conf(ngx_cycle_t *cycle);
static char *ngx_custom_worker_init_conf(ngx_cycle_t *cycle, void *conf);

static char *ngx_worker_process_factor(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static char *ngx_worker_process_spare(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

typedef struct {
  ngx_int_t factor;
  ngx_int_t spare;
} ngx_custom_worker_conf_t;

static ngx_command_t ngx_core_custom_worker_commands[] = {
    {ngx_string("worker_processes_factor"), NGX_MAIN_CONF | NGX_DIRECT_CONF | NGX_CONF_TAKE1, ngx_worker_process_factor,
     0, 0, NULL},
    {ngx_string("worker_processes_spare"), NGX_MAIN_CONF | NGX_DIRECT_CONF | NGX_CONF_TAKE1, ngx_worker_process_spare,
     0, 0, NULL},
    ngx_null_command};

static ngx_core_module_t ngx_core_custom_worker_module_ctx = {
    ngx_string("core_custom_worker"), ngx_custom_worker_create_conf, ngx_custom_worker_init_conf};

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

static void *ngx_custom_worker_create_conf(ngx_cycle_t *cycle)
{
  ngx_custom_worker_conf_t *cwcf;

  cwcf = ngx_pcalloc(cycle->pool, sizeof(ngx_custom_worker_conf_t));
  if (cwcf == NULL) {
    return NULL;
  }
  cwcf->factor = NGX_CONF_UNSET;
  cwcf->spare = NGX_CONF_UNSET;

  return cwcf;
}

static char *ngx_custom_worker_init_conf(ngx_cycle_t *cycle, void *conf)
{
  ngx_int_t worker_processes;
  ngx_core_conf_t *ccf;
  ngx_custom_worker_conf_t *cwcf;

  ccf = (ngx_core_conf_t *)ngx_get_conf(cycle->conf_ctx, ngx_core_module);
  cwcf = (ngx_custom_worker_conf_t *)conf;

  if (cwcf->factor == NGX_CONF_UNSET && cwcf->spare == NGX_CONF_UNSET) {
    return NGX_CONF_OK;
  }

  worker_processes = ccf->worker_processes;
  if (cwcf->factor != NGX_CONF_UNSET) {
    worker_processes = worker_processes * cwcf->factor;
  }

  if (cwcf->spare != NGX_CONF_UNSET) {
    if (worker_processes - cwcf->spare < 1) {
      ngx_log_error(NGX_LOG_WARN, cycle->log, 0,
                    "worker_processes is smaller than ngx_worker_process_spare. worker_processes is set to be 1");
      worker_processes = 1;
    } else {
      worker_processes = worker_processes - cwcf->spare;
    }
  }
  ccf->worker_processes = worker_processes;
  return NGX_CONF_OK;
}

static char *ngx_worker_process_factor(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
  ngx_str_t *value;
  ngx_int_t factor;
  ngx_core_conf_t *ccf;
  ngx_custom_worker_conf_t *cwcf;

  ccf = (ngx_core_conf_t *)ngx_get_conf(cf->cycle->conf_ctx, ngx_core_module);
  if (ccf->worker_processes == NGX_CONF_UNSET) {
    return "don't set worker process";
  }

  value = cf->args->elts;
  factor = ngx_atoi(value[1].data, value[1].len);

  if (factor < 1) {
    return "worker_processes_factor shoud be positive number";
  }

  cwcf = (ngx_custom_worker_conf_t *)conf;
  cwcf->factor = factor;
  return NGX_CONF_OK;
}

static char *ngx_worker_process_spare(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
  ngx_str_t *value;
  ngx_int_t spare;
  ngx_core_conf_t *ccf;
  ngx_custom_worker_conf_t *cwcf;

  ccf = (ngx_core_conf_t *)ngx_get_conf(cf->cycle->conf_ctx, ngx_core_module);
  if (ccf->worker_processes == NGX_CONF_UNSET) {
    return "don't set worker process";
  }

  value = cf->args->elts;
  spare = ngx_atoi(value[1].data, value[1].len);

  cwcf = (ngx_custom_worker_conf_t *)conf;
  cwcf->spare = spare;
  return NGX_CONF_OK;
}