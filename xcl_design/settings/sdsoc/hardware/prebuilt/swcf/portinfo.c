/* File: /proj/xsjhdstaff4/xd-relman/work/Rodin/REL/2015.4_sdsoc/regression/products/sdsoc/platforms/prebuilt/generate/zybo/TEST_WORK_lnx64/add_gen_prebuilt/apf/_sds/p0/.cf_work/portinfo.c */
#include "cf_lib.h"
#include "cf_request.h"
#include "devreg.h"

#include "portinfo.h"

#include "stdio.h"  // for printf

#include "xlnk_core_cf.h"
#include "accel_info.h"


void _p0_cf_open_port (cf_port_base_t *port)
{
  port->open_i(port, NULL);
}

void _p0_cf_framework_open(int first)
{
  cf_context_init();
  xlnkCounterMap();
  _p0_cf_register(first);
  cf_get_current_context();
}

void _p0_cf_framework_close(int last)
{
  _p0_cf_unregister(last);
}

#define TOTAL_PARTITIONS 1
int current_partition_num = 0;
struct {
  void (*open)(int);
  void (*close)(int);
}

_ptable[TOTAL_PARTITIONS]  = {
    {.open = &_p0_cf_framework_open, .close= &_p0_cf_framework_close}, 
};

void switch_to_next_partition(int partition_num)
{
#ifdef __linux__
  if (current_partition_num != partition_num) {
    _ptable[current_partition_num].close(0);
    char buf[128];
    sprintf(buf, "cat /mnt/_sds/_p%d_.bin > /dev/xdevcfg", partition_num);
    system(buf);
    _ptable[partition_num].open(0);
    current_partition_num = partition_num;
  }
#endif
}

void init_first_partition() __attribute__ ((constructor));
void close_last_partition() __attribute__ ((destructor));
void init_first_partition()
{
    current_partition_num = 0;
    _ptable[current_partition_num].open(1);

    sds_trace_setup();
}


void close_last_partition()
{
#ifdef PERF_EST
    apf_perf_estimation_exit();
#endif
    sds_trace_cleanup();
    _ptable[current_partition_num].close(1);
    current_partition_num = 0;
}


// XSIP watermark, do not delete 67d7842dbbe25473c3c32b93c0da8047785f30d78e8a024de1b57352245f9689
