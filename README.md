critnib
=======

This library provides a lock-free implementation of *critnib* — a cross
between radix trees and D.J. Bernstein's critbit.  The structure offers
searches for equality and for next neighbour.

This implementation is restricted to fixed sized keys — an intptr_t in this
library — even though the abstract data structure can support arbitrary key
lengths.  Lock-free mutations haven't been implemented either, although
they're relatively easy to add.  On the other hand, reads are strongly
optimized, and do not require any kind of bus synchronizations (which are
very costly on a many-core multi-socket machine).

That is, you will want this library if you need reads inside tight
performance-critical loops but writes don't require this level of
optimization.  


Copyright
=========

This code has been developed at Intel as an internal part of PMDK, then
extracted by the author and adapted for use as a stand-alone library.
Thus, while the copyright and praise belong to Intel, this library is not
a product supported or validated by Intel.
