#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>

//
// Instanciate dummy interupt handlers
//

#define TINY_MAX_SIZE 256

#define TINY_size(z, n, unused)                                 \
  void BOOST_PP_CAT(__interrupt_handler,n)                    \
  (void) { return; }

BOOST_PP_REPEAT(TINY_MAX_SIZE, TINY_size, ~)

#undef TINY_size
