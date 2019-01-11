/*  parallel.h -- Macros for parallel processing
    Written Jan 7, 2019 by Eric Olson */

#ifndef _PARALLEL_H
#define _PARALLEL_H

#ifdef _OPENMP
#  include <omp.h>
#  define spawn(X) _Pragma("omp task default(shared)") X
#  define sync _Pragma("omp taskwait")
#  define get_ncpu(X) omp_get_num_procs(X)
#  define workers(X) \
    if(X) omp_set_num_threads(X); \
    else omp_set_num_threads(1); \
    _Pragma("omp parallel") \
    _Pragma("omp single")
#else
#  ifdef _CILKPLUS
#    include <cilk/cilk.h>
#    include <cilk/cilk_api.h>
#    define spawn(X) X cilk_spawn
#    define sync cilk_sync
#    define get_ncpu(X) __cilkrts_get_nworkers(X)
#    define workers(X) \
    __cilkrts_end_cilk(); \
    if(X){ \
        char buf[32]; \
        sprintf(buf,"%d",X); \
        int r=__cilkrts_set_param("nworkers",buf); \
        if(r!=__CILKRTS_SET_PARAM_SUCCESS){ \
            fprintf(stderr,"Error: unable to set nworkers to %d!\n",X); \
            exit(1); \
        } \
        __cilkrts_init(); \
    }
#  else
#    define spawn(X) X
#    define sync
#    define get_ncpu(X) 1
#    define workers(X)
#  endif
#endif

#endif
