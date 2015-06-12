#include <ngx_config.h>  
#include <ngx_core.h>  
#include <ngx_http.h>  
  
static ngx_int_t   
ngx_http_mytest1_handler(ngx_http_request_t *r);  
  
static char *   
ngx_http_mytest1(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);  
  
static void*  
ngx_http_mytest1_create_loc_conf(ngx_conf_t *cf);  
  
static char*  
ngx_http_mytest1_merge_loc_conf(ngx_conf_t *cf,void *parent,void *child);  
  
//存储配置项参数的结构体  
typedef struct{  
        ngx_str_t arg_str;//保存一个字符串类型的参数  
        ngx_int_t arg_num;  
        ngx_flag_t arg_flag;  
        size_t arg_size;  
        ngx_array_t* arg_str_array;  
        ngx_array_t* arg_keyval;  
        off_t arg_off;  
        ngx_msec_t arg_msec;  
        time_t arg_sec;  
        ngx_bufs_t arg_bufs;  
        ngx_uint_t arg_enum_seq;  
        ngx_uint_t arg_bitmask;  
        ngx_uint_t arg_access;  
        ngx_path_t* arg_path;  
}ngx_http_mytest1_loc_conf_t;  
  
//设置配置项的解析方式  
static ngx_command_t ngx_http_mytest1_commands[] = {  
        {  
                //test_str配置项  
                ngx_string("test_str"),  
        NGX_HTTP_LOC_CONF|NGX_HTTP_LMT_CONF|NGX_CONF_TAKE1,  
        ngx_conf_set_str_slot,//预设的配置项解析方法  
                NGX_HTTP_LOC_CONF_OFFSET,  
                offsetof(ngx_http_mytest1_loc_conf_t,arg_str),  
                NULL  
        },  
        {  
                //test_flag配置项  
                ngx_string("test_flag"),  
        NGX_HTTP_LOC_CONF|NGX_HTTP_LMT_CONF|NGX_CONF_TAKE1,  
                ngx_conf_set_flag_slot,//预设的配置项解析方法  
        NGX_HTTP_LOC_CONF_OFFSET,  
                offsetof(ngx_http_mytest1_loc_conf_t,arg_flag),  
                NULL  
        },  
        {  
                //test_num配置项  
                ngx_string("test_num"),  
        NGX_HTTP_LOC_CONF|NGX_HTTP_LMT_CONF|NGX_CONF_TAKE1,  
                ngx_conf_set_num_slot,//预设的配置项解析方法  
        NGX_HTTP_LOC_CONF_OFFSET,  
                offsetof(ngx_http_mytest1_loc_conf_t,arg_num),  
                NULL  
        },  
        {  
                //test_size配置项  
                ngx_string("test_size"),  
        NGX_HTTP_LOC_CONF|NGX_HTTP_LMT_CONF|NGX_CONF_TAKE1,  
                ngx_conf_set_size_slot,//预设的配置项解析方法  
        NGX_HTTP_LOC_CONF_OFFSET,  
                offsetof(ngx_http_mytest1_loc_conf_t,arg_size),  
                NULL  
        },  
        {  
                //mytest1配置项  
                ngx_string("mytest1"),  
        NGX_HTTP_LOC_CONF|NGX_HTTP_LMT_CONF|NGX_CONF_NOARGS,  
        ngx_http_mytest1,  
                NGX_HTTP_LOC_CONF_OFFSET,  
                0,  
                NULL  
        },  
    ngx_null_command  
};  
  
//模块上下文定义  
static ngx_http_module_t ngx_http_mytest1_module_ctx = {  
    NULL,  
    NULL,  
    NULL,  
    NULL,  
    NULL,  
    NULL,  
    ngx_http_mytest1_create_loc_conf,//创建数据结构存储loc级别的配置项的回调方法  
    ngx_http_mytest1_merge_loc_conf//合并loc级别的配置项  
};  
  
//模块定义  
ngx_module_t ngx_http_mytest1_module = {  
    NGX_MODULE_V1,  
    &ngx_http_mytest1_module_ctx,  
    ngx_http_mytest1_commands,  
    NGX_HTTP_MODULE,  
    NULL,  
    NULL,  
    NULL,  
    NULL,  
    NULL,  
    NULL,  
    NULL,  
    NGX_MODULE_V1_PADDING  
};  
  
//模块的回调方法  
static char *   
ngx_http_mytest1(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)  
{  
    ngx_http_core_loc_conf_t *clcf;  
  
    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);  
    clcf->handler = ngx_http_mytest1_handler;  
        //ngx_conf_set_str_slot(cf,cmd,conf);//预设的配置项处理方法  
  
    return NGX_CONF_OK;  
}  
  
//模块真正完成处理工作的handler  
static ngx_int_t ngx_http_mytest1_handler(ngx_http_request_t *r)  
{  
        ngx_http_mytest1_loc_conf_t *elcf;//存储配置项参数的结构体  
        elcf = ngx_http_get_module_loc_conf(r,ngx_http_mytest1_module);  
  
    if (!(r->method & (NGX_HTTP_GET | NGX_HTTP_HEAD | NGX_HTTP_POST))) {  
        return NGX_HTTP_NOT_ALLOWED;  
    }  
  
    ngx_int_t rc = ngx_http_discard_request_body(r);  
    if (rc != NGX_OK) {  
        return rc;  
    }  
  
    ngx_str_t type = ngx_string("text/html");  
        ngx_str_t str_format = ngx_string("test_str=%V,test_flag=%i,test_num=%i,test_size=%z");  
        ngx_str_t test_str = elcf->arg_str;  
        ngx_flag_t test_flag = elcf->arg_flag;  
        ngx_int_t test_num = elcf->arg_num;  
        size_t test_size = elcf->arg_size;  
        int data_len = str_format.len + test_str.len + 1;  
    r->headers_out.status = NGX_HTTP_OK;  
    r->headers_out.content_length_n = data_len;//响应包包体内容长度  
    r->headers_out.content_type = type;  
  
    rc = ngx_http_send_header(r);  
    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only) {  
        return rc;  
    }  
  
    ngx_buf_t *b;  
    b = ngx_create_temp_buf(r->pool,data_len);  
        if (b == NULL) {  
        return NGX_HTTP_INTERNAL_SERVER_ERROR;  
    }  
  
        ngx_snprintf(b->pos,data_len,(char *)str_format.data,&test_str,test_flag,test_num,test_size);  
        b->last = b->pos + data_len;  
    b->last_buf = 1;  
  
    ngx_chain_t out;  
    out.buf = b;  
    out.next = NULL;  
  
    return ngx_http_output_filter(r, &out);  
}  
  
static void*  
ngx_http_mytest1_create_loc_conf(ngx_conf_t *cf){  
        ngx_http_mytest1_loc_conf_t *mycf;  
        mycf = ngx_pcalloc(cf->pool,sizeof(ngx_http_mytest1_loc_conf_t));  
        if(NULL == mycf){  
                return NGX_CONF_ERROR;  
        }  
        mycf->arg_str.len = 0;  
        mycf->arg_str.data = NULL;  
  
        //注意一下设定必不可少，否则会出错  
        mycf->arg_flag = NGX_CONF_UNSET;  
        mycf->arg_num = NGX_CONF_UNSET;  
        mycf->arg_str_array = NGX_CONF_UNSET_PTR;  
        mycf->arg_keyval = NULL;  
        mycf->arg_off = NGX_CONF_UNSET;  
        mycf->arg_msec = NGX_CONF_UNSET_MSEC;  
        mycf->arg_sec = NGX_CONF_UNSET;  
        mycf->arg_size = NGX_CONF_UNSET_SIZE;  
  
        return mycf;  
}  
  
  
static char*  
ngx_http_mytest1_merge_loc_conf(ngx_conf_t *cf,void *parent,void *child){  
        ngx_http_mytest1_loc_conf_t *prev = parent;  
        ngx_http_mytest1_loc_conf_t *conf = child;  
        ngx_conf_merge_str_value(conf->arg_str,prev->arg_str,"");  
  
        return NGX_CONF_OK;  
} 
