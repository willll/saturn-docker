#include "sega_int.h"

// Boost preprocessor includes for code generation
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>

//
// Instanciate dummy interupt handlers
//

#define FNC_MAX_SIZE BOOST_PP_LIMIT_REPEAT

#define FNC_GET_IDX(get, idx) get(idx)

#define FNC_size(z, n, unused)                                \
  void BOOST_PP_CAT(__interrupt_handler,n)                    \
  (void) {                                                    \
      FNC_GET_IDX(INT_GetScuFunc,n)();                        \
      return; }

BOOST_PP_REPEAT(FNC_MAX_SIZE, FNC_size, ~)

#undef FNC_size
#undef FNC_MAX_SIZE
